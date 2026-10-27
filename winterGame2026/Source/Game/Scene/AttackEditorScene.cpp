#include "AttackEditorScene.h"
#include "SceneController.h"
#include "DebugScene.h"
#include "Engine/Core/Application.h"
#include "Engine/Core/ResourceManager.h"
#include "Engine/Animation/AnimationController.h"
#include "Game/Player/Player.h"
#include "Game/Battle/BattleManager.h"
#include "Game/Battle/HitBoxSet.h"
#include "Game/Battle/AttackData.h"
#include <DxLib.h>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <string_view>
#undef min
#undef max

namespace
{
	//キーリピートが始まるまでのフレーム数
	constexpr int kRepeatDelay = 20;
	//キーリピートの間隔
	constexpr int kRepeatInterval = 3;
	//プレビューで使う攻撃の番号(当たったかを見るだけなので固定でいい)
	constexpr int kPreviewAttackID = 1;
	//相手との距離の初期値
	constexpr float kDefaultDistance = 400.0f;
	//相手との距離の最大
	constexpr float kMaxDistance = 2000.0f;
	//再生速度の最小
	constexpr float kMinAnimSpeed = 0.05f;
	//タイムラインに表示する最大フレーム数
	constexpr int kMaxTimelineLength = 300;
	//メッセージを表示する時間
	constexpr int kMessageTime = 120;
	//技の名前の最大文字数
	constexpr int kNameMaxLength = 32;

	//項目一覧の表示位置
	constexpr int kPanelX = 10;
	constexpr int kPanelY = 10;
	constexpr int kPanelWidth = 400;
	constexpr int kParamX = 24;
	constexpr int kParamY = 70;
	constexpr int kParamValueX = 190;
	constexpr int kParamLineHeight = 22;
	//項目のグループの間の隙間
	constexpr int kParamGroupGap = 8;

	//タイムラインの表示位置
	constexpr int kTimelineX = 60;
	constexpr int kTimelineBottom = 60;
	constexpr int kTimelineHeight = 24;
	//グラフの高さ
	constexpr int kGraphHeight = 70;
	//グラフとタイムラインの間隔
	constexpr int kGraphGap = 10;

	//カメラ
	constexpr float kCameraHeight = 300.0f;
	constexpr float kCameraTargetHeight = 250.0f;
	constexpr float kCameraDistance = 1000.0f;
	//左の項目一覧に隠れないように、カメラを左にずらしてキャラを右に寄せる
	constexpr float kCameraShiftX = -300.0f;

	//タイムラインの色
	constexpr unsigned int kStartUpColor = 0x40c040;
	constexpr unsigned int kActiveColor = 0xe04040;
	constexpr unsigned int kRecoveryColor = 0x4070e0;
	//技が終わった後(アニメーションだけ続いている部分)
	constexpr unsigned int kAfterColor = 0x383838;

	//項目の名前
	constexpr const wchar_t* kParamNames[] =
	{
		L"アニメーション",
		L"再生速度",
		L"ブレンド",
		L"判定のボーン",
		L"判定の長さ",
		L"判定の半径",
		L"判定のオフセットY",
		L"発生",
		L"持続",
		L"硬直",
		L"ダメージ",
		L"ヒット硬直",
		L"ガード硬直",
		L"相手との距離(保存しない)",
	};

	//攻撃判定を付けられるボーン
	constexpr const wchar_t* kBoneNames[] =
	{
		L"mixamorig:LeftHand",
		L"mixamorig:RightHand",
		L"mixamorig:LeftForeArm",
		L"mixamorig:RightForeArm",
		L"mixamorig:LeftFoot",
		L"mixamorig:RightFoot",
		L"mixamorig:LeftToeBase",
		L"mixamorig:RightToeBase",
		L"mixamorig:LeftLeg",
		L"mixamorig:RightLeg",
		L"mixamorig:Head",
		L"mixamorig:Spine2",
		L"mixamorig:Hips",
	};

	/// <summary>
	/// "Armature|Jab" や "mixamorig:LeftHand" の前の部分を消して見やすくする
	/// </summary>
	std::wstring StripPrefix(const std::wstring& name)
	{
		size_t pos = name.find_last_of(L"|:");
		return (pos == std::wstring::npos) ? name : name.substr(pos + 1);
	}

	/// <summary>
	/// 項目のグループの切れ目を考えたY座標
	/// </summary>
	int GetParamY(int index)
	{
		//アニメ / 攻撃判定 / 技性能 / ツール の4つに分ける
		int gap = 0;
		if (index >= 3) gap += kParamGroupGap;
		if (index >= 7) gap += kParamGroupGap;
		if (index >= 13) gap += kParamGroupGap;
		return kParamY + index * kParamLineHeight + gap;
	}

	/// <summary>
	/// 配列の中で次(前)の要素の番号を返す。見つからないときは先頭
	/// </summary>
	template<typename T, typename Container>
	size_t NextIndex(const Container& items, const T& current, int dir)
	{
		const size_t count = std::size(items);
		auto it = std::find(std::begin(items), std::end(items), current);
		if (it == std::end(items))
		{
			return 0;
		}
		const size_t index = static_cast<size_t>(std::distance(std::begin(items), it));
		return (index + count + dir) % count;
	}
}

AttackEditorScene::AttackEditorScene(SceneController& controller) :
	SceneBase(controller),
	m_attackIndex(0),
	m_paramIndex(0),
	m_frame(1),
	m_isPlaying(true),
	m_slowLevel(0),
	m_slowCounter(0),
	m_distance(kDefaultDistance),
	m_isHit(false),
	m_isDirty(false),
	m_messageTimer(0),
	m_nameInputHandle(-1),
	m_keyHoldFrame{},
	m_isPrevMouseDown(false),
	m_isScrubbing(false)
{
	static_assert(std::size(kParamNames) == static_cast<size_t>(Param::count), "項目の名前の数が合っていません");

	auto& resourceManager = ResourceManager::GetInstance();
	m_battleManager = std::make_unique<BattleManager>();
	m_attacker = std::make_unique<Player>(PlayerID::P1, resourceManager.GetModel(ModelType::player1), m_battleManager.get());
	m_dummy = std::make_unique<Player>(PlayerID::P2, resourceManager.GetModel(ModelType::player2), m_battleManager.get());
	m_attacker->Init();
	m_dummy->Init();
	//相手は待機の最初のポーズで止めておく
	m_dummy->GetAnimationController().Update();

	//モデルに入っているアニメーションの一覧を作る
	const int model = m_attacker->GetModelHandle();
	const int animNum = MV1GetAnimNum(model);
	for (int i = 0; i < animNum; i++)
	{
		m_animNames.push_back(MV1GetAnimName(model, i));
	}

	PlacePlayers();
	SelectAttack(0);
}

AttackEditorScene::~AttackEditorScene()
{
	if (m_nameInputHandle != -1)
	{
		DeleteKeyInput(m_nameInputHandle);
	}
}

void AttackEditorScene::Update()
{
	UpdateKeyState();

	if (m_messageTimer > 0)
	{
		--m_messageTimer;
	}

	//名前の入力中は他の操作をしない
	if (m_nameInputHandle != -1)
	{
		UpdateNameInput();
		return;
	}

	//デバッグシーンに戻る
	//(ChangeSceneでこのシーンは消えるので、この後は何もしないでreturnする)
	if (IsKeyTriggered(KEY_INPUT_BACK))
	{
		m_controller.ChangeScene(std::make_shared<DebugScene>(m_controller));
		return;
	}

	UpdateEdit();
	UpdatePlayback();
}

void AttackEditorScene::UpdateKeyState()
{
	char keyState[256];
	GetHitKeyStateAll(keyState);
	for (int i = 0; i < 256; i++)
	{
		m_keyHoldFrame[i] = keyState[i] ? m_keyHoldFrame[i] + 1 : 0;
	}
}

bool AttackEditorScene::IsKeyTriggered(int key) const
{
	return m_keyHoldFrame[key] == 1;
}

bool AttackEditorScene::IsKeyRepeated(int key) const
{
	const int hold = m_keyHoldFrame[key];
	//押した瞬間と、長押ししてからは一定間隔
	return hold == 1 || (hold >= kRepeatDelay && (hold - kRepeatDelay) % kRepeatInterval == 0);
}

bool AttackEditorScene::IsKeyPressed(int key) const
{
	return m_keyHoldFrame[key] > 0;
}

void AttackEditorScene::UpdateNameInput()
{
	//0:入力中 1:決定
	if (CheckKeyInput(m_nameInputHandle) == 0)
	{
		return;
	}

	wchar_t buffer[kNameMaxLength + 1] = {};
	GetKeyInputString(buffer, m_nameInputHandle);
	DeleteKeyInput(m_nameInputHandle);
	m_nameInputHandle = -1;

	//コードから名前で探すので半角英数字だけにする
	std::string name;
	for (const wchar_t* c = buffer; *c != L'\0'; ++c)
	{
		if (*c > 0x7f)
		{
			ShowMessage(L"名前は半角英数字にしてください");
			return;
		}
		name += static_cast<char>(*c);
	}

	//空なら取り消し
	if (name.empty())
	{
		ShowMessage(L"技の追加を取り消しました");
		return;
	}

	auto& table = AttackTable::GetInstance();
	if (table.Contains(name))
	{
		ShowMessage(L"同じ名前の技がすでにあります");
		return;
	}

	//今の技をコピーして名前だけ変える
	AttackDef def = Current();
	def.name = name;
	table.Add(def);
	m_isDirty = true;
	SelectAttack(table.GetCount() - 1);
	ShowMessage(L"技を追加しました(Ctrl+Sで保存)");
}

void AttackEditorScene::UpdateEdit()
{
	auto& table = AttackTable::GetInstance();
	const bool isCtrl = IsKeyPressed(KEY_INPUT_LCONTROL) || IsKeyPressed(KEY_INPUT_RCONTROL);
	const bool isShift = IsKeyPressed(KEY_INPUT_LSHIFT) || IsKeyPressed(KEY_INPUT_RSHIFT);

	//Ctrlを押しているときはファイル操作だけ
	if (isCtrl)
	{
		//保存
		if (IsKeyTriggered(KEY_INPUT_S))
		{
			if (table.Save())
			{
				m_isDirty = false;
				ShowMessage(L"保存しました: Data/Attack/AttackData.csv");
			}
			else
			{
				ShowMessage(L"保存に失敗しました");
			}
		}
		//ファイルから読み直す(保存していない変更は消える)
		if (IsKeyTriggered(KEY_INPUT_R))
		{
			if (table.Load())
			{
				m_isDirty = false;
				SelectAttack(m_attackIndex);
				ShowMessage(L"ファイルから読み直しました");
			}
			else
			{
				ShowMessage(L"ファイルが読み込めませんでした");
			}
		}
		return;
	}

	//技の切り替え
	const size_t count = table.GetCount();
	if (IsKeyTriggered(KEY_INPUT_Q))
	{
		SelectAttack((m_attackIndex + count - 1) % count);
	}
	if (IsKeyTriggered(KEY_INPUT_E))
	{
		SelectAttack((m_attackIndex + 1) % count);
	}

	//技の追加(名前の入力を始める)
	if (IsKeyTriggered(KEY_INPUT_N))
	{
		m_nameInputHandle = MakeKeyInput(kNameMaxLength, FALSE, TRUE, FALSE);
		SetActiveKeyInput(m_nameInputHandle);
		return;
	}

	//攻撃側と相手の入れ替え
	if (IsKeyTriggered(KEY_INPUT_T))
	{
		SwapPlayers();
	}

	//項目の選択
	const int paramCount = static_cast<int>(Param::count);
	if (IsKeyRepeated(KEY_INPUT_UP))
	{
		m_paramIndex = (m_paramIndex + paramCount - 1) % paramCount;
	}
	if (IsKeyRepeated(KEY_INPUT_DOWN))
	{
		m_paramIndex = (m_paramIndex + 1) % paramCount;
	}

	//値の変更
	if (IsKeyRepeated(KEY_INPUT_LEFT))
	{
		ChangeParam(-1, isShift);
	}
	if (IsKeyRepeated(KEY_INPUT_RIGHT))
	{
		ChangeParam(1, isShift);
	}
}

void AttackEditorScene::UpdatePlayback()
{
	const int total = Current().GetTotalFrame();
	const int length = GetTimelineLength();
	const bool isCtrl = IsKeyPressed(KEY_INPUT_LCONTROL) || IsKeyPressed(KEY_INPUT_RCONTROL);
	if (!isCtrl)
	{
		//再生・停止
		if (IsKeyTriggered(KEY_INPUT_SPACE))
		{
			m_isPlaying = !m_isPlaying;
		}
		//スロー(等倍→1/2→1/4)
		if (IsKeyTriggered(KEY_INPUT_S))
		{
			m_slowLevel = (m_slowLevel + 1) % 3;
		}
		//コマ送り
		if (IsKeyRepeated(KEY_INPUT_Z))
		{
			m_isPlaying = false;
			--m_frame;
		}
		if (IsKeyRepeated(KEY_INPUT_X))
		{
			m_isPlaying = false;
			++m_frame;
		}
	}
	
	//最初のフレームへ
	if (IsKeyTriggered(KEY_INPUT_HOME))
	{
		m_frame = 1;
	}

	//タイムラインとグラフをクリック・ドラッグしてフレームを選ぶ
	const auto& wsize = Application::GetInstance().GetWindowSize();
	const int timelineW = wsize.w - kTimelineX * 2;
	const int barY = wsize.h - kTimelineBottom;
	const int graphTop = barY - kGraphGap - kGraphHeight;

	int mouseX = 0;
	int mouseY = 0;
	GetMousePoint(&mouseX, &mouseY);
	const bool isMouseDown = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
	const bool isInside =
		mouseX >= kTimelineX && mouseX < kTimelineX + timelineW &&
		mouseY >= graphTop && mouseY < barY + kTimelineHeight;
	//タイムラインの上で押し始めたときだけドラッグにする
	if (isMouseDown && !m_isPrevMouseDown && isInside)
	{
		m_isScrubbing = true;
	}
	if (!isMouseDown)
	{
		m_isScrubbing = false;
	}
	if (m_isScrubbing)
	{
		m_isPlaying = false;
		const float cellW = static_cast<float>(timelineW) / static_cast<float>(length);
		m_frame = 1 + static_cast<int>((mouseX - kTimelineX) / cellW);
	}
	m_isPrevMouseDown = isMouseDown;

	//再生中はフレームを進める(技の全体フレームでループ)
	if (m_isPlaying)
	{
		++m_slowCounter;
		if (m_slowCounter >= (1 << m_slowLevel))
		{
			m_slowCounter = 0;
			++m_frame;
			if (m_frame > total)
			{
				m_frame = 1;
			}
		}
	}

	m_frame = std::clamp(m_frame, 1, length);
	ApplyFrame();
}

void AttackEditorScene::ChangeParam(int dir, bool isFast)
{
	AttackDef& def = Current();
	//フレームなどの整数の変化量
	const int step = isFast ? 5 : 1;
	//長さなどの小数の変化量
	const float floatStep = isFast ? 10.0f : 1.0f;
	//技の情報を変えたかどうか(距離はツールの設定なので保存しない)
	bool isAttackParam = true;

	switch (static_cast<Param>(m_paramIndex))
	{
	case Param::anim:
		if (m_animNames.empty())
		{
			return;
		}
		def.animName = m_animNames[NextIndex(m_animNames, def.animName, dir)];
		SetupAttackAnimation();
		break;
	case Param::animSpeed:
	{
		//小数の誤差がたまらないように0.01単位で丸める
		const float speed = def.animSpeed + dir * (isFast ? 0.25f : 0.05f);
		def.animSpeed = std::max(kMinAnimSpeed, std::round(speed * 100.0f) / 100.0f);
		break;
	}
	case Param::blendTime:
		def.blendTime = std::max(0.0f, def.blendTime + dir);
		break;
	case Param::bone:
		def.bone = kBoneNames[NextIndex(kBoneNames, std::wstring_view(def.bone), dir)];
		break;
	case Param::length:
		def.length = std::max(0.0f, def.length + dir * floatStep);
		break;
	case Param::radius:
		def.radius = std::max(1.0f, def.radius + dir * floatStep);
		break;
	case Param::offsetY:
		def.offsetY += dir * floatStep;
		break;
	case Param::startUp:
		def.data.startUp = std::max(1, def.data.startUp + dir * step);
		break;
	case Param::active:
		def.data.active = std::max(1, def.data.active + dir * step);
		break;
	case Param::recovery:
		def.data.recovery = std::max(0, def.data.recovery + dir * step);
		break;
	case Param::damage:
		def.data.damage = std::max(0, def.data.damage + dir * step);
		break;
	case Param::hitStun:
		def.data.hitStun = std::max(0, def.data.hitStun + dir * step);
		break;
	case Param::blockStun:
		def.data.blockStun = std::max(0, def.data.blockStun + dir * step);
		break;
	case Param::distance:
		m_distance = std::clamp(m_distance + dir * (isFast ? 100.0f : 10.0f), 0.0f, kMaxDistance);
		PlacePlayers();
		isAttackParam = false;
		break;
	default:
		break;
	}

	if (isAttackParam)
	{
		m_isDirty = true;
	}
	//速度やボーンが変わるとグラフも変わるので作り直す
	BuildReachCurve();
}

void AttackEditorScene::SelectAttack(size_t index)
{
	auto& table = AttackTable::GetInstance();
	//技が一つも無いときは仮の技を作る
	if (table.GetCount() == 0)
	{
		AttackDef def;
		def.name = "NewAttack";
		def.animName = Player::kJabAnimName;
		def.bone = kBoneNames[0];
		def.data = { 10, 3, 10, 10, 15, 8 };
		table.Add(def);
		m_isDirty = true;
	}

	m_attackIndex = std::min(index, table.GetCount() - 1);
	m_frame = 1;
	SetupAttackAnimation();
}

void AttackEditorScene::SwapPlayers()
{
	m_attacker->GetHitBoxes().EndAttack();
	std::swap(m_attacker, m_dummy);

	//相手になった方は待機の最初のポーズで止める
	auto& dummyAnim = m_dummy->GetAnimationController();
	dummyAnim.Play(Player::kIdleAnimName, true, 0.0f, 0.0f);
	dummyAnim.Update();
	dummyAnim.SetTime(0.0f);

	PlacePlayers();
	SetupAttackAnimation();
}

void AttackEditorScene::SetupAttackAnimation()
{
	auto& anim = m_attacker->GetAnimationController();
	//ブレンドせず、勝手に進まないように速度0で再生する
	//(時間はApplyFrameでフレームから計算して直接指定する)
	anim.Play(Current().animName, false, 0.0f, 0.0f);
	//前のアニメーションを外すために一回だけ更新する
	anim.Update();

	BuildReachCurve();
	ApplyFrame();
}

void AttackEditorScene::PlacePlayers()
{
	//攻撃側を左、相手を右に置く
	m_attacker->GetTransform().SetPosition({ -m_distance * 0.5f, 0.0f, 0.0f });
	m_dummy->GetTransform().SetPosition({ m_distance * 0.5f, 0.0f, 0.0f });

	//BattleManagerはP1とP2の順番で渡す(向きの計算に使われる)
	Player* p1 = (m_attacker->GetPlayerID() == PlayerID::P1) ? m_attacker.get() : m_dummy.get();
	Player* p2 = (p1 == m_attacker.get()) ? m_dummy.get() : m_attacker.get();
	m_battleManager->Init(p1, p2);

	//お互いの方を向かせる
	m_attacker->OpponentRotate();
	m_dummy->OpponentRotate();
	m_attacker->ApplyDxLibMatrix();
	m_dummy->ApplyDxLibMatrix();
}

void AttackEditorScene::ApplyFrame()
{
	const AttackDef& def = Current();

	//ゲーム中と同じく「フレーム数 × 再生速度」がアニメーション時間になる
	auto& anim = m_attacker->GetAnimationController();
	anim.SetTime(std::min(m_frame * def.animSpeed, anim.GetTotalTime()));

	//持続フレームの間だけ攻撃判定を出す(PlayerAttackStateと同じ範囲)
	auto& hitBoxes = m_attacker->GetHitBoxes();
	if (m_frame >= def.data.startUp && m_frame < def.data.startUp + def.data.active)
	{
		hitBoxes.StartAttack(def.GetBox(), def.data, kPreviewAttackID);
	}
	else
	{
		hitBoxes.EndAttack();
	}

	//判定をボーンの位置に合わせる
	m_attacker->GetHitBoxes().Update(m_attacker->GetTransform().GetPosition());
	m_dummy->GetHitBoxes().Update(m_dummy->GetTransform().GetPosition());

	//相手のやられ判定に当たっているか
	m_isHit = !m_collision.DetectHits(*m_attacker, *m_dummy).empty();
}

void AttackEditorScene::BuildReachCurve()
{
	m_reachCurve.clear();

	const AttackDef& def = Current();
	const int model = m_attacker->GetModelHandle();
	const int boneFrame = MV1SearchFrame(model, def.bone.c_str());
	if (boneFrame < 0)
	{
		return;
	}

	auto& anim = m_attacker->GetAnimationController();
	const float totalTime = anim.GetTotalTime();
	const Vector3 root = m_attacker->GetTransform().GetPosition();
	const Vector3 forward = m_battleManager->GetOpponentDirection(m_attacker->GetPlayerID());

	//フレームごとにポーズを変えて、ボーンが相手の方向にどれだけ出ているかを調べる
	const int length = GetTimelineLength();
	for (int frame = 1; frame <= length; frame++)
	{
		anim.SetTime(std::min(frame * def.animSpeed, totalTime));
		Vector3 bonePos;
		bonePos = MV1GetFramePosition(model, boneFrame);
		m_reachCurve.push_back((bonePos - root).Dot(forward));
	}

	//ポーズを今のフレームに戻す
	anim.SetTime(std::min(m_frame * def.animSpeed, totalTime));
}

int AttackEditorScene::GetAnimEndFrame() const
{
	const float speed = std::max(kMinAnimSpeed, Current().animSpeed);
	return static_cast<int>(std::ceil(m_attacker->GetAnimationController().GetTotalTime() / speed));
}

int AttackEditorScene::GetTimelineLength() const
{
	const int length = std::max(Current().GetTotalFrame(), GetAnimEndFrame());
	return std::clamp(length, 1, kMaxTimelineLength);
}

void AttackEditorScene::ShowMessage(const std::wstring& message)
{
	m_message = message;
	m_messageTimer = kMessageTime;
}

AttackDef& AttackEditorScene::Current()
{
	return AttackTable::GetInstance().GetAt(m_attackIndex);
}

const AttackDef& AttackEditorScene::Current() const
{
	return AttackTable::GetInstance().GetAt(m_attackIndex);
}

void AttackEditorScene::Draw()
{
	//横から見るカメラ
	SetCameraPositionAndTarget_UpVecY(
		VGet(kCameraShiftX, kCameraHeight, -kCameraDistance),
		VGet(kCameraShiftX, kCameraTargetHeight, 0.0f));

	DrawGrid();

	//Player::Drawは位置の文字も出すので、モデルと判定だけ描く
	MV1DrawModel(m_attacker->GetModelHandle());
	MV1DrawModel(m_dummy->GetModelHandle());
	m_attacker->GetHitBoxes().Draw();
	m_dummy->GetHitBoxes().Draw();

	DrawParams();
	DrawTimeline();
	DrawHelp();

	const auto& wsize = Application::GetInstance().GetWindowSize();
	if (m_isHit)
	{
		DrawExtendString(wsize.w / 2 + 120, 150, 3.0, 3.0, L"HIT!", 0xff4040);
	}

	if (m_messageTimer > 0)
	{
		DrawString(kPanelX + kPanelWidth + 20, 16, m_message.c_str(), 0x80ff80);
	}

	//名前入力中
	if (m_nameInputHandle != -1)
	{
		const int x = wsize.w / 2 - 220;
		const int y = wsize.h / 2 - 40;
		DrawBox(x, y, x + 440, y + 80, 0x202020, true);
		DrawBox(x, y, x + 440, y + 80, 0xffffff, false);
		DrawString(x + 12, y + 10, L"新しい技の名前(半角英数字)", 0xffffff);
		DrawString(x + 12, y + 56, L"Enterで決定 / 空のままEnterで取り消し", 0xaaaaaa);
		DrawKeyInputString(x + 12, y + 32, m_nameInputHandle);
	}
}

void AttackEditorScene::DrawParams()
{
	auto& table = AttackTable::GetInstance();
	const AttackDef& def = Current();
	const int panelBottom = GetParamY(static_cast<int>(Param::count)) + 70;

	//背景
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);
	DrawBox(kPanelX, kPanelY, kPanelX + kPanelWidth, panelBottom, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(kParamX, 18, L"攻撃エディタ", 0xffffff);
	//技の名前(コード側で使う名前なので半角英数字)
	std::wstring name(def.name.begin(), def.name.end());
	DrawFormatString(kParamX, 42, 0xffff00, L"Q <  %s  (%d/%d)%s  > E",
		name.c_str(),
		static_cast<int>(m_attackIndex + 1),
		static_cast<int>(table.GetCount()),
		m_isDirty ? L" *未保存" : L"");

	for (int i = 0; i < static_cast<int>(Param::count); i++)
	{
		//項目の値を文字にする
		wchar_t value[128] = {};
		switch (static_cast<Param>(i))
		{
		case Param::anim:		swprintf_s(value, L"%s", StripPrefix(def.animName).c_str());	break;
		case Param::animSpeed:	swprintf_s(value, L"%.2f (%.1f倍速)", def.animSpeed, def.animSpeed / 0.5f);	break;
		case Param::blendTime:	swprintf_s(value, L"%.0fF", def.blendTime);	break;
		case Param::bone:		swprintf_s(value, L"%s", StripPrefix(def.bone).c_str());	break;
		case Param::length:		swprintf_s(value, L"%.0f", def.length);	break;
		case Param::radius:		swprintf_s(value, L"%.0f", def.radius);	break;
		case Param::offsetY:	swprintf_s(value, L"%.0f", def.offsetY);	break;
		case Param::startUp:	swprintf_s(value, L"%dF", def.data.startUp);	break;
		case Param::active:		swprintf_s(value, L"%dF", def.data.active);	break;
		case Param::recovery:	swprintf_s(value, L"%dF", def.data.recovery);	break;
		case Param::damage:		swprintf_s(value, L"%d", def.data.damage);	break;
		case Param::hitStun:	swprintf_s(value, L"%dF", def.data.hitStun);	break;
		case Param::blockStun:	swprintf_s(value, L"%dF", def.data.blockStun);	break;
		case Param::distance:	swprintf_s(value, L"%.0f", m_distance);	break;
		default:	break;
		}

		const int y = GetParamY(i);
		const bool isSelected = (i == m_paramIndex);
		if (isSelected)
		{
			DrawBox(kPanelX + 4, y - 2, kPanelX + kPanelWidth - 4, y + kParamLineHeight - 4, 0x505020, true);
		}
		const unsigned int color = isSelected ? 0xffff00 : 0xffffff;
		DrawString(kParamX, y, kParamNames[i], color);
		DrawFormatString(kParamValueX, y, color, L"%s %s", isSelected ? L"<" : L" ", value);
	}

	//全体フレームとアニメーションの長さを比べてアドバイスを出す
	const int total = def.GetTotalFrame();
	const int animEnd = GetAnimEndFrame();
	int y = GetParamY(static_cast<int>(Param::count)) + 4;
	DrawFormatString(kParamX, y, 0xffffff, L"全体 %dF / アニメ終了 %dF", total, animEnd);
	y += 20;
	if (animEnd > total + 5)
	{
		DrawFormatString(kParamX, y, 0xffa040, L"アニメが %dF 余る(速度を上げるか硬直を伸ばす)", animEnd - total);
	}
	else if (animEnd < total)
	{
		DrawFormatString(kParamX, y, 0xffa040, L"技の途中でアニメが %dF 止まる", total - animEnd);
	}
	else
	{
		DrawString(kParamX, y, L"アニメと全体フレームがだいたい合っています", 0x80ff80);
	}
}

void AttackEditorScene::DrawTimeline()
{
	const AttackDef& def = Current();
	const auto& wsize = Application::GetInstance().GetWindowSize();
	const int x = kTimelineX;
	const int w = wsize.w - kTimelineX * 2;
	const int barY = wsize.h - kTimelineBottom;
	const int graphTop = barY - kGraphGap - kGraphHeight;
	const int length = GetTimelineLength();
	const float cellW = static_cast<float>(w) / static_cast<float>(length);
	const int total = def.GetTotalFrame();
	const int startUp = def.data.startUp;
	const int activeEnd = def.data.startUp + def.data.active;

	//背景
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);
	DrawBox(x - 10, graphTop - 44, x + w + 10, barY + kTimelineHeight + 22, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//今のフレームの状態
	const wchar_t* phase = L"技終了後";
	if (m_frame < startUp)			phase = L"発生前";
	else if (m_frame < activeEnd)	phase = L"持続中";
	else if (m_frame <= total)		phase = L"硬直中";
	static constexpr const wchar_t* kSlowText[] = { L"等倍", L"1/2", L"1/4" };
	auto& anim = m_attacker->GetAnimationController();
	DrawFormatString(x, graphTop - 40, 0xffffff,
		L"フレーム %d / %d [%s]   アニメ時間 %.1f / %.1f   %s(%s)",
		m_frame, total, phase,
		std::min(m_frame * def.animSpeed, anim.GetTotalTime()), anim.GetTotalTime(),
		m_isPlaying ? L"再生中" : L"停止中", kSlowText[m_slowLevel]);

	//攻撃判定のボーンの伸びのグラフ
	DrawString(x, graphTop - 20, L"攻撃判定のボーンの伸び(上ほど相手側)", 0xaaaaaa);
	if (m_reachCurve.size() >= 2)
	{
		auto [minIt, maxIt] = std::minmax_element(m_reachCurve.begin(), m_reachCurve.end());
		const float minValue = *minIt;
		const float range = std::max(1.0f, *maxIt - minValue);
		auto toY = [&](float value)
			{
				return graphTop + kGraphHeight - (value - minValue) / range * kGraphHeight;
			};
		auto toX = [&](size_t index)
			{
				return x + cellW * (static_cast<float>(index) + 0.5f);
			};

		for (size_t i = 1; i < m_reachCurve.size(); i++)
		{
			DrawLineAA(toX(i - 1), toY(m_reachCurve[i - 1]), toX(i), toY(m_reachCurve[i]), 0xffffff);
		}
		//一番伸びているフレーム
		const size_t peak = static_cast<size_t>(std::distance(m_reachCurve.begin(), maxIt));
		DrawCircleAA(toX(peak), toY(*maxIt), 4.0f, 16, 0xff8080, true);
		DrawFormatString(static_cast<int>(toX(peak)) + 6, static_cast<int>(toY(*maxIt)) - 4, 0xff8080,
			L"最大 %dF", static_cast<int>(peak + 1));
	}

	//フレームのマス(発生前:緑 持続:赤 硬直:青 技の後:灰)
	const int gap = (cellW >= 4.0f) ? 1 : 0;
	for (int frame = 1; frame <= length; frame++)
	{
		unsigned int color = kAfterColor;
		if (frame < startUp)			color = kStartUpColor;
		else if (frame < activeEnd)		color = kActiveColor;
		else if (frame <= total)		color = kRecoveryColor;

		const int left = x + static_cast<int>(cellW * (frame - 1));
		const int right = x + static_cast<int>(cellW * frame) - gap;
		DrawBox(left, barY, right, barY + kTimelineHeight, color, true);

		//10フレームごとに数字
		if (frame % 10 == 0)
		{
			DrawFormatString(left, barY + kTimelineHeight + 2, 0xaaaaaa, L"%d", frame);
		}
	}

	//アニメーションが終わるフレーム
	const int animEnd = GetAnimEndFrame();
	if (animEnd <= length)
	{
		const float endX = x + cellW * animEnd;
		DrawLineAA(endX, static_cast<float>(graphTop), endX, static_cast<float>(barY + kTimelineHeight + 4), 0xffffff);
		DrawString(static_cast<int>(endX) + 4, barY - 18, L"アニメ終了", 0xffffff);
	}

	//今のフレーム
	const float cursorX = x + cellW * (m_frame - 0.5f);
	DrawLineAA(cursorX, static_cast<float>(graphTop - 4), cursorX, static_cast<float>(barY + kTimelineHeight + 4), 0xffff00, 2.0f);
}

void AttackEditorScene::DrawHelp()
{
	static constexpr const wchar_t* kHelp[] =
	{
		L"↑↓: 項目を選ぶ   ←→: 値を変える(Shiftで大きく)",
		L"Q / E: 技の切り替え   N: 今の技をコピーして追加",
		L"Space: 再生/停止   Z / X: コマ戻し/コマ送り",
		L"S: スロー   Home: 最初へ   T: 攻撃側を入れ替え",
		L"タイムラインをクリック/ドラッグでフレーム移動",
		L"Ctrl+S: 保存   Ctrl+R: 読み直し   BackSpace: 戻る",
	};

	const auto& wsize = Application::GetInstance().GetWindowSize();
	const int x = wsize.w - 430;
	int y = 40;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);
	DrawBox(x - 10, y - 6, wsize.w - 10, y + static_cast<int>(std::size(kHelp)) * 18 + 4, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	for (const wchar_t* line : kHelp)
	{
		DrawString(x, y, line, 0xcccccc);
		y += 18;
	}
}

void AttackEditorScene::DrawGrid()
{
	for (int z = -500; z <= 500; z += 100)
	{
		DrawLine3D(VGet(-1000.0f, 0.0f, static_cast<float>(z)), VGet(1000.0f, 0.0f, static_cast<float>(z)), 0x606060);
	}
	for (int x = -1000; x <= 1000; x += 100)
	{
		DrawLine3D(VGet(static_cast<float>(x), 0.0f, -500.0f), VGet(static_cast<float>(x), 0.0f, 500.0f), 0x606060);
	}
}
