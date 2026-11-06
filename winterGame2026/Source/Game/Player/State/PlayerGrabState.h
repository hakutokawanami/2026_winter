#pragma once
#include "PlayerStateBase.h"
#include"Game/Battle/AttackData.h"

class PlayerGrabState : public PlayerStateBase
{
public:
	PlayerGrabState(Player& player, const AttackDef& attack);

	void Enter() override;
	void Update() override;
	void Exit() override;

	bool IsThrowable()const override { return false; }
private:
	const AttackDef& m_attack;
	int m_frame;
};

