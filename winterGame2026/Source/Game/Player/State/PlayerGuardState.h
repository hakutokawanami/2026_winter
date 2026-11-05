#pragma once
#include "PlayerStateBase.h"
class PlayerGuardState : public PlayerStateBase
{
public:
	using PlayerStateBase::PlayerStateBase;

	void Enter()override;
	void Update()override;
	void Exit()override;

	bool IsGuarding()const override { return true; }
};

