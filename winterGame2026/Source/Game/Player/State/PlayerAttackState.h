#pragma once
#include "PlayerStateBase.h"
#include "Game/Player/Player.h"

struct AttackDef;
class PlayerAttackState : public PlayerStateBase
{
public:
	PlayerAttackState(Player& player, const AttackDef& attack);

	void Enter()override;
	void Update()override;
	void Exit()override;

	bool IsAttacking()const override { return true; }
private:

	bool CanCombo()const;

	//出している技
	const AttackDef& m_attack;
	//経過フレーム
	int m_frame;
};

