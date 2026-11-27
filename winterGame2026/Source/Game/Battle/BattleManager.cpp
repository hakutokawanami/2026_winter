#include "BattleManager.h"
#include "Game/Player/Player.h"
#include"HitBoxSet.h"

namespace
{
	//ステージの中央
	const Vector3 kStageCenter = { 0.0f,0.0f,0.0f };
	//ステージの半径
	constexpr float kStageRadius = 1000.0f;
	//プレイヤーの押出判定の半径
	constexpr float kBodyRadius = 80.0f;
}

BattleManager::BattleManager():
	m_centerPosition({}),
	m_direction({}),
	m_distance(0.0f),
	m_player1(nullptr),
	m_player2(nullptr)
{
}

BattleManager::~BattleManager()
{
}

void BattleManager::Init(Player* player1, Player* player2)
{
	m_player1 = player1;
	m_player2 = player2;

	if (m_player1 == nullptr || m_player2 == nullptr)
	{
		return;
	}
	// プレイヤーの位置を取得
	const Vector3 pos1 = m_player1->GetTransform().GetPosition();
	const Vector3 pos2 = m_player2->GetTransform().GetPosition();
	// プレイヤー間の中心位置を計算
	CalculateCenter(pos1, pos2);
	// P1からP2への方向ベクトルと距離を計算
	CalculateDirectionAndDistance(pos1, pos2);
}

void BattleManager::Update()
{
	if(m_player1 == nullptr || m_player2 == nullptr)
	{
		return;
	}
	//ステージとの押し戻し
	ClampToStage(*m_player1);
	ClampToStage(*m_player2);

	//移動後の位置に判定を合わせる
	m_player1->GetHitBoxes().Update(m_player1->GetTransform().GetPosition());
	m_player2->GetHitBoxes().Update(m_player2->GetTransform().GetPosition());
	//押し戻し
	m_collision.ResolvePush(*m_player1, *m_player2);

	//プレイヤー同士の押し戻しをした結果のステージとの押し戻し
	const Vector3 back1 = ClampToStage(*m_player1);
	const Vector3 back2 = ClampToStage(*m_player2);
	//戻された分は相手を押す
	m_player2->GetTransform().Translate(back1);
	m_player1->GetTransform().Translate(back2);
	//またはみ出す可能性があるのでもう一度
	ClampToStage(*m_player1);
	ClampToStage(*m_player2);

	//押し戻し後の位置に合わせる
	m_player1->ApplyDxLibMatrix();
	m_player2->ApplyDxLibMatrix();
	m_player1->GetHitBoxes().Update(m_player1->GetTransform().GetPosition());
	m_player2->GetHitBoxes().Update(m_player2->GetTransform().GetPosition());
	
	//攻撃の当たった処理
	auto hitEvents = m_collision.DetectHits(*m_player1, *m_player2);
	ResolveHits(hitEvents);

	//投げの当たった処理
	auto throwEvents = m_collision.DetectThrowHits(*m_player1, *m_player2);
	ResolveThrowHits(throwEvents, hitEvents);

	// プレイヤーの位置を取得
	const Vector3 pos1 = m_player1->GetTransform().GetPosition();
	const Vector3 pos2 = m_player2->GetTransform().GetPosition();
	// プレイヤー間の中心位置を計算
	CalculateCenter(pos1, pos2);
	// P1からP2への方向ベクトルと距離を計算
	CalculateDirectionAndDistance(pos1, pos2);
}

void BattleManager::Draw()
{
#ifdef _DEBUG
	// デバッグ用にプレイヤー間の中心位置を描画
	DrawSphere3D(m_centerPosition, 10.0f, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), true);
	// デバッグ用にP1からP2への方向ベクトルを描画
	DrawLine3D(m_player1->GetTransform().GetPosition(), m_player2->GetTransform().GetPosition(), GetColor(0, 255, 0));

	//ステージの端を壁のように描く
	//円を何分割するか
	constexpr int kDivide = 64;
	//壁の高さ
	constexpr float kWallHeight = 250.0f;
	//縦の線を何分割ごとに引くか
	constexpr int kPillarInterval = 4;
	//壁の面の透明度(0～255)
	constexpr int kWallAlpha = 60;
	constexpr unsigned int kWallColor = 0xff8000;
	const Vector3 up = { 0.0f, kWallHeight, 0.0f };

	//面は半透明にして、奥のプレイヤーを隠さないようにZバッファには書き込まない
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kWallAlpha);
	SetWriteZBuffer3D(FALSE);
	for (int i = 0; i < kDivide; i++)
	{
		const float a0 = DX_TWO_PI_F * i / kDivide;
		const float a1 = DX_TWO_PI_F * (i + 1) / kDivide;
		const Vector3 p0 = kStageCenter + Vector3{ cosf(a0), 0.0f, sinf(a0) } * kStageRadius;
		const Vector3 p1 = kStageCenter + Vector3{ cosf(a1), 0.0f, sinf(a1) } * kStageRadius;
		//四角形を三角形2枚で描く
		DrawTriangle3D(p0, p1, p1 + up, kWallColor, true);
		DrawTriangle3D(p0, p1 + up, p0 + up, kWallColor, true);
	}
	SetWriteZBuffer3D(TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//枠線(床の円・上の円・一定間隔の縦線)
	for (int i = 0; i < kDivide; i++)
	{
		const float a0 = DX_TWO_PI_F * i / kDivide;
		const float a1 = DX_TWO_PI_F * (i + 1) / kDivide;
		const Vector3 p0 = kStageCenter + Vector3{ cosf(a0), 0.0f, sinf(a0) } * kStageRadius;
		const Vector3 p1 = kStageCenter + Vector3{ cosf(a1), 0.0f, sinf(a1) } * kStageRadius;
		DrawLine3D(p0, p1, kWallColor);
		DrawLine3D(p0 + up, p1 + up, kWallColor);
		if (i % kPillarInterval == 0)
		{
			DrawLine3D(p0, p0 + up, kWallColor);
		}
	}
#endif // _DEBUG
}

void BattleManager::CalculateCenter(const Vector3& pos1, const Vector3& pos2)
{
	// プレイヤー間の中心
	m_centerPosition = (pos1 + pos2) * 0.5f;
}

void BattleManager::CalculateDirectionAndDistance(const Vector3& pos1, const Vector3& pos2)
{
	// P1からP2への方向ベクトル
	m_direction = (pos2 - pos1);
	// 水平方向のみ
	m_direction.y = 0.0f;
	// プレイヤー間の距離
	m_distance = m_direction.Length();
	// 正規化
	if(m_distance > 0.0f)
	{
		m_direction.Normalize();
	}
}

Player* BattleManager::GetPlayer(PlayerID id) const
{
	if(id == PlayerID::P1)
	{
		return m_player1;
	}

	if(id == PlayerID::P2)
	{
		return m_player2;
	}

	return nullptr;
}

Player* BattleManager::GetOpponentPlayer(PlayerID id) const
{
	if(id == PlayerID::P1)
	{
		return m_player2;
	}

	if(id == PlayerID::P2)
	{
		return m_player1;
	}

	return nullptr;
}

Vector3 BattleManager::GetCenterPosition() const
{
	return m_centerPosition;
}

Vector3 BattleManager::GetOpponentDirection(PlayerID id) const
{
	if (id == PlayerID::P1)
	{
		return m_direction;
	}
	else
	{
		return -m_direction;
	}
}

float BattleManager::GetDistance() const
{
	return m_distance;
}

Vector3 BattleManager::GetRightDirection() const
{
	return m_direction;
}

Vector3 BattleManager::GetForwardDirection() const
{
	return m_direction.Cross(Vector3::Up());
}

bool BattleManager::IsOpponentOnRight(PlayerID id) const
{
	const Vector3 right = m_direction;
	
	return GetOpponentDirection(id).Dot(right) > 0.0f;
}

void BattleManager::ResolveHits(const std::vector<HitEvent>& hitEvents)
{
	for (const HitEvent& event : hitEvents)
	{
		Player* defender = GetPlayer(event.defender);
		//最後に受けた攻撃の番号を記録
		defender->GetHitBoxes().SetLastReceivedAttackID(event.attackInstanceId);
		//当たった処理
		bool isShake = defender->OnHit(*event.attack);
		//イベントを保存
		m_events.push_back(BattleEvent{ isShake });
	}
}

void BattleManager::ResolveThrowHits(const std::vector<ThrowHitEvent>& throwEvents, const std::vector<HitEvent>& hitEvents)
{
	if(throwEvents.empty())
	{
		return;
	}

	//投げぬけ
	if (throwEvents.size() >= 2)
	{
		Player* attacker = GetPlayer(throwEvents[0].attacker);
		Player* defender = GetPlayer(throwEvents[0].defender);

		attacker->OnThrowBreak(true);
		defender->OnThrowBreak(false);
		return;
	}

	//投げようとした側が攻撃を受けていたら攻撃を受けた処理を優先する
	for (const HitEvent& event : hitEvents)
	{
		if (event.defender == throwEvents.front().attacker)
		{
			return;
		}
	}

	for (const ThrowHitEvent& event : throwEvents)
	{
		Player* attacker = GetPlayer(event.attacker);
		Player* defender = GetPlayer(event.defender);
		//投げられる状態かどうか
		if (defender->IsThrowable())
		{
			//投げの際にプレイヤー同士の距離を調整する
			Vector3 center = m_player1->GetBattleManager()->GetCenterPosition();
			Vector3 dir = m_player1->GetBattleManager()->GetOpponentDirection(m_player1->GetPlayerID());
			m_player1->GetTransform().SetPosition(center - (dir * 100.0f));
			m_player1->ApplyDxLibMatrix();
			m_player2->GetTransform().SetPosition(center + (dir * 100.0f));
			m_player2->ApplyDxLibMatrix();

			//投げる処理
			attacker->OnThrowSuccess(*event.attack);
			//投げられる処理
			defender->OnThrowHit(*event.attack);
		}
	}
}

Vector3 BattleManager::ClampToStage(Player& player)
{
	//現在のポジション
	const Vector3 pos = player.GetTransform().GetPosition();
	//体の端が円に触れるとこまで
	const float limit = kStageRadius - kBodyRadius;
	//ステージ中央からのずれ
	Vector3 offset = pos - kStageCenter;
	offset.y = 0.0f;
	//ステージ中央からの距離
	const float distance = offset.Length();
	//円の中にいるなら
	if (distance <= limit)
	{
		//何もしない
		return { 0.0f,0.0f,0.0f };
	}
	//端の位置に戻す
	Vector3 clamped = kStageCenter + offset * (limit / distance);
	clamped.y = 0.0f;
	//プレイヤーの位置を補正
	player.GetTransform().SetPosition(clamped);
	//戻した量を返す
	return clamped - pos;
}
