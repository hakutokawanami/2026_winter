#pragma once
#include "PlayerStateBase.h"
#include"Game/Battle/AttackData.h"

class PlayerThrowHitState : public PlayerStateBase
{
public:
	PlayerThrowHitState(Player& player, const AttackData& attack);
	void Enter() override;
	void Update() override;
	void Exit() override;

	bool IsThrowable()const override { return false; }
	bool IsThrowing()const override { return true; }
private:
	const AttackData& m_attack;

	int m_frame;
};

