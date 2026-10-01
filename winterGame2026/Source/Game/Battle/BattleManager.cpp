#include "BattleManager.h"
#include "Game/Player/Player.h"

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
}

void BattleManager::Update()
{
	if(m_player1 == nullptr || m_player2 == nullptr)
	{
		return;
	}

	const Vector3 pos1 = m_player1->GetTransform().GetPosition();
	const Vector3 pos2 = m_player2->GetTransform().GetPosition();
	// プレイヤー間の中心
	m_centerPosition = (pos1 + pos2) * 0.5f;
	//P1からP2への方向ベクトル
	m_direction = (pos2 - pos1);
	// 水平方向のみ
	m_direction.y = 0.0f;
	// プレイヤー間の距離
	m_distance = m_direction.Length();

	if(m_distance > 0.0f)
	{
		m_direction.Normalize();
	}
}

void BattleManager::Draw()
{
}

Player* BattleManager::GetPlayer(PlayerID id) const
{
	return nullptr;
}

Vector3 BattleManager::GetCenterPosition() const
{
	return Vector3();
}

Vector3 BattleManager::GetDirection() const
{
	return Vector3();
}

float BattleManager::GetDistance() const
{
	return 0.0f;
}
