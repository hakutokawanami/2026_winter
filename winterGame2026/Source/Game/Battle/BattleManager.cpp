#include "BattleManager.h"
#include "Game/Player/Player.h"
#include"HitBoxSet.h"
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
	//移動後の位置に判定を合わせる
	m_player1->GetHitBoxes().Update(m_player1->GetTransform().GetPosition());
	m_player2->GetHitBoxes().Update(m_player2->GetTransform().GetPosition());
	//押し戻し
	m_collision.ResolvePush(*m_player1, *m_player2);

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
	// デバッグ用にプレイヤー間の距離を描画
	DrawFormatString(10, 10, GetColor(255, 255, 255), L"Distance: %.2f", m_distance);
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
		defender->OnHit(*event.attack);
	}
}

void BattleManager::ResolveThrowHits(const std::vector<ThrowHitEvent>& throwEvents, const std::vector<HitEvent>& hitEvents)
{
	if(throwEvents.empty())
	{
		return;
	}

	if (throwEvents.size() >= 2)
	{
		//投げぬけの処理
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
			//投げる処理
			attacker->OnThrowSuccess(*event.attack);
			//投げられる処理
			defender->OnThrowHit(*event.attack);
		}
	}
}
