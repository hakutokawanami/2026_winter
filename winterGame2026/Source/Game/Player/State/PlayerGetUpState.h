#pragma once
#include "PlayerStateBase.h"
class PlayerGetUpState : public PlayerStateBase
{
public:
	using PlayerStateBase::PlayerStateBase;

	void Enter()override;
	void Update()override;
	void Exit()override;
	
	bool IsThrowable()const override{ return false; }
	bool IsGuarding()const override{ return IsGuardInput(); }
	bool IsFacingOpponent()const override{ return false; }
};

