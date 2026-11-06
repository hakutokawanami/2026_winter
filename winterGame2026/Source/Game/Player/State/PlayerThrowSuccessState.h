#pragma once
#include "PlayerStateBase.h"
#include"Game/Battle/AttackData.h"

class PlayerThrowSuccessState : public PlayerStateBase
{
public:
	PlayerThrowSuccessState(Player& player, const AttackData& attack);
	void Enter() override;
	void Update() override;
	void Exit() override;

private:
	const AttackData& m_attack;	

	int m_frame;
};

