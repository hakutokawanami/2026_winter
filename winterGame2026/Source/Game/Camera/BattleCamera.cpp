#include "BattleCamera.h"

BattleCamera::BattleCamera()
{
}

BattleCamera::~BattleCamera()
{
}

void BattleCamera::Init()
{
}

void BattleCamera::Update()
{
}

void BattleCamera::SetPlayers(std::shared_ptr<Player>player1, std::shared_ptr<Player>player2)
{
	m_player1 = player1;
	m_player2 = player2;
}
