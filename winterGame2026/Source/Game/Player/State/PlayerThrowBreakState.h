#pragma once
#include "PlayerStateBase.h"
class PlayerThrowBreakState : public PlayerStateBase
{
public:
	PlayerThrowBreakState(Player& player, bool attacker);

	void Enter()override;
	void Update()override;
	void Exit()override;

	bool IsThrowable()const override { return false; }

private:
	bool m_isAttacker;
	//離れる方向
	Vector3 m_backDir;

	int m_frame;
};

