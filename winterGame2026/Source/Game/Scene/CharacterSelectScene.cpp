#include "CharacterSelectScene.h"
#include"Engine/Core/Application.h"
#include"ModeSelectScene.h"
#include"GameScene.h"
#include"SceneController.h"
#include"Engine/Input/InputManager.h"
#include<DxLib.h>

namespace
{
	//フェードにかかる時間
	constexpr int kFadeInterval = 60;
	//名前と対応するキャラクターID
	struct CharacterItem
	{
		const wchar_t* name;
		CharacterID id;
	};
	//キャラクターの配列
	constexpr CharacterItem kCharacterItems[] =
	{
		{ L"キャラクター1",CharacterID::character1 },
		{ L"キャラクター2",CharacterID::character2 }
	};
	//キャラクターの数
	constexpr int kCharacterCount = static_cast<int>(std::size(kCharacterItems));
}

CharacterSelectScene::CharacterSelectScene(SceneController& controller, const MatchSetting& matchSetting) :
	SceneBase(controller),
	m_isBack(false)
{
	//フェードイン
	m_update = &CharacterSelectScene::FadeInUpdate;
	//フェード用描画
	m_draw = &CharacterSelectScene::FadeDraw;
	//フェード用のフレームカウンター初期化
	m_frame = kFadeInterval;
	//マッチ設定を受け継ぐ
	m_matchSetting = matchSetting;
}

CharacterSelectScene::~CharacterSelectScene()
{
}

void CharacterSelectScene::Update()
{
	(this->*m_update)();
}

void CharacterSelectScene::Draw()
{
	(this->*m_draw)();
}

bool CharacterSelectScene::UpdateSelector(Selector& selector, PlayerID id)
{
	auto& input = InputManager::GetInstance();

	//決定
	if (input.IsTriggered(id, "A"))
	{
		selector.isDecided = true;
	}

	//決定済みだったら
	if (selector.isDecided)
	{
		//キャンセル
		if (input.IsTriggered(id, "B"))
		{
			selector.isDecided = false;
		}

		//決定済みの時はカーソルが動かないように
		return false;
	}

	if (input.IsTriggered(id, "left"))
	{
		selector.cursor = (selector.cursor + kCharacterCount - 1) % kCharacterCount;
	}

	if (input.IsTriggered(id, "right"))
	{
		selector.cursor = (selector.cursor + 1) % kCharacterCount;
	}

	//戻る
	if (input.IsTriggered(id, "B"))
	{
		return true;
	}

	return false;
}

bool CharacterSelectScene::UpdateVersus()
{
	//それぞれ操作
	bool isBack1 = UpdateSelector(m_selectors[0], PlayerID::P1);
	bool isBack2 = UpdateSelector(m_selectors[1], PlayerID::P2);
	//どちらかが戻る操作をしたなら戻る
	return isBack1 || isBack2;
}

bool CharacterSelectScene::UpdateSingle()
{	
	//CPUを操作するかどうか
	const bool isSelectCpu = m_selectors[0].isDecided;
	
	//自分が決まっていたらCPU
	//決まっていないなら自分を動かす
	Selector& select = isSelectCpu ? m_selectors[1] : m_selectors[0];

	//動かすカーソルの更新
	bool isBack = UpdateSelector(select, PlayerID::P1);

	//戻る操作をしたとき
	if (isBack)
	{
		//CPUを選んでいたら
		if (isSelectCpu)
		{
			//自分の操作に戻る
			m_selectors[0].isDecided = false;
			return false;
		}
		//自分を選んでるなら
		else
		{
			//前のシーンに戻る
			return true;
		}
	}

	return false;
}

void CharacterSelectScene::FadeInUpdate()
{
	if (--m_frame <= 0)
	{
		m_update = &CharacterSelectScene::NormalUpdate;
		m_draw = &CharacterSelectScene::NormalDraw;
		return;
	}
}

void CharacterSelectScene::NormalUpdate()
{
	auto& input = InputManager::GetInstance();
	input.Update();

	//どちらも選択済みだったら
	if (m_selectors[0].isDecided && m_selectors[1].isDecided)
	{
		//次へ
		if (input.IsTriggered("A"))
		{
			m_matchSetting.p1 = kCharacterItems[m_selectors[0].cursor].id;
			m_matchSetting.p2 = kCharacterItems[m_selectors[1].cursor].id;

			m_update = &CharacterSelectScene::FadeOutUpdate;
			m_draw = &CharacterSelectScene::FadeDraw;
			return;
		}
	}
	//対人戦なら
	if (m_matchSetting.gameMode == GameMode::vsPlayer)
	{
		m_isBack = UpdateVersus();
	}
	//一人用なら
	else
	{
		m_isBack = UpdateSingle();
	}

	//前のシーンに戻る
	if (m_isBack)
	{
		m_update = &CharacterSelectScene::FadeOutUpdate;
		m_draw = &CharacterSelectScene::FadeDraw;
	}
}

void CharacterSelectScene::FadeOutUpdate()
{
	if (++m_frame >= kFadeInterval)
	{
		if (m_isBack)
		{
			m_controller.ChangeScene(std::make_shared<ModeSelectScene>(m_controller));
			return;
		}
		else
		{
			m_controller.ChangeScene(std::make_shared<GameScene>(m_controller, m_matchSetting));
		}
	}
}

void CharacterSelectScene::NormalDraw()
{
	auto& wsize = Application::GetInstance().GetWindowSize();

	//見出しの高さ(画面の高さに対する割合)
	constexpr float kTitleRate = 0.12f;
	//キャラクター枠の中心の高さ(画面の高さに対する割合)
	constexpr float kPanelRate = 0.42f;
	//キャラクター枠の大きさ
	constexpr int kPanelWidth = 220;
	constexpr int kPanelHeight = 140;
	//キャラクター枠同士の間隔
	constexpr int kPanelSpace = 48;
	//カーソル枠の太さ
	constexpr int kCursorThickness = 4;
	//カーソル枠をキャラクター枠からどれだけ外側に描くか(P1は外側、P2は内側の輪にして重なっても見えるように)
	constexpr int kCursorOffset[2] = { 8, 2 };
	//「1P」「2P」の文字とキャラクター枠の間
	constexpr int kLabelOffset = 20;
	//プレイヤー情報の高さ(画面の高さに対する割合)
	constexpr float kInfoRate = 0.7f;
	//プレイヤー情報の横位置(画面の幅に対する割合)
	constexpr float kInfoXRate[2] = { 0.25f, 0.75f };
	//プレイヤー情報の行の間隔
	constexpr int kInfoLineSpace = 32;
	//プレイヤーごとの色
	constexpr unsigned int kPlayerColor[2] = { 0xff4040, 0x40a0ff };
	//キャラクター枠の色
	constexpr unsigned int kPanelColor = 0x303030;
	constexpr unsigned int kPanelFrameColor = 0x808080;
	//決定済みの色
	constexpr unsigned int kDecidedColor = 0xffd040;
	//選択中の色
	constexpr unsigned int kSelectingColor = 0x808080;

	//文字列を指定したX座標の中央にそろえて描画する
	auto drawCenterString = [](int centerX, int y, const wchar_t* str, unsigned int color)
		{
			int width = GetDrawStringWidth(str, static_cast<int>(wcslen(str)));
			DrawString(centerX - width / 2, y, str, color);
		};

	const int fontSize = GetFontSize();

	//プレイヤーの呼び方(対人以外は2P側がCPU)
	const wchar_t* playerLabel[2] = { L"1P", L"2P" };
	if (m_matchSetting.gameMode != GameMode::vsPlayer)
	{
		playerLabel[1] = L"CPU";
	}

	//見出し
	drawCenterString(wsize.w / 2, static_cast<int>(wsize.h * kTitleRate), L"CHARACTER SELECT", 0xffffff);

	//キャラクター枠を横に並べたときの左端
	const int totalWidth = kCharacterCount * kPanelWidth + (kCharacterCount - 1) * kPanelSpace;
	const int startX = (wsize.w - totalWidth) / 2;
	const int panelTop = static_cast<int>(wsize.h * kPanelRate) - kPanelHeight / 2;

	//キャラクター枠
	for (int i = 0; i < kCharacterCount; i++)
	{
		int left = startX + i * (kPanelWidth + kPanelSpace);
		DrawBox(left, panelTop, left + kPanelWidth, panelTop + kPanelHeight, kPanelColor, true);
		DrawBox(left, panelTop, left + kPanelWidth, panelTop + kPanelHeight, kPanelFrameColor, false);
		//名前は枠の真ん中
		drawCenterString(left + kPanelWidth / 2, panelTop + (kPanelHeight - fontSize) / 2, kCharacterItems[i].name, 0xffffff);
	}

	//カーソル(P1は枠の上、P2は枠の下に名前を出す)
	for (int side = 0; side < 2; side++)
	{
		const Selector& selector = m_selectors[side];
		int left = startX + selector.cursor * (kPanelWidth + kPanelSpace);
		int right = left + kPanelWidth;
		int bottom = panelTop + kPanelHeight;

		//決定したら色を変える
		unsigned int color = selector.isDecided ? kDecidedColor : kPlayerColor[side];

		//太さの分だけ1pxずつずらして枠を重ねる
		for (int t = 0; t < kCursorThickness; t++)
		{
			int offset = kCursorOffset[side] + t;
			DrawBox(left - offset, panelTop - offset, right + offset, bottom + offset, color, false);
		}

		int centerX = left + kPanelWidth / 2;
		if (side == 0)
		{
			drawCenterString(centerX, panelTop - kCursorOffset[0] - kCursorThickness - kLabelOffset - fontSize / 2, playerLabel[side], kPlayerColor[side]);
		}
		else
		{
			drawCenterString(centerX, bottom + kCursorOffset[0] + kCursorThickness + kLabelOffset - fontSize / 2, playerLabel[side], kPlayerColor[side]);
		}
	}

	//プレイヤー情報(左に1P、右に2P)
	for (int side = 0; side < 2; side++)
	{
		const Selector& selector = m_selectors[side];
		int centerX = static_cast<int>(wsize.w * kInfoXRate[side]);
		int y = static_cast<int>(wsize.h * kInfoRate);

		drawCenterString(centerX, y, playerLabel[side], kPlayerColor[side]);
		drawCenterString(centerX, y + kInfoLineSpace, kCharacterItems[selector.cursor].name, 0xffffff);
		if (selector.isDecided)
		{
			drawCenterString(centerX, y + kInfoLineSpace * 2, L"OK!", kDecidedColor);
		}
		else
		{
			drawCenterString(centerX, y + kInfoLineSpace * 2, L"選択中…", kSelectingColor);
		}
	}

	//操作説明
	drawCenterString(wsize.w / 2, wsize.h - 60, L"←→：選択　A：決定　B：キャンセル／戻る", 0xffffff);
}

void CharacterSelectScene::FadeDraw()
{
	NormalDraw();

	auto& wsize = Application::GetInstance().GetWindowSize();
	//フェード処理
	float rate = static_cast<float>(m_frame) / static_cast<float>(kFadeInterval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast <int>(255 * rate));
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, static_cast <int>(255 * rate));
}
