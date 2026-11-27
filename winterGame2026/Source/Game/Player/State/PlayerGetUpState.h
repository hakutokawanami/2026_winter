#pragma once
#include "PlayerStateBase.h"

enum class GetUp
{
	normal,
	attack,
	rollBack
};

class PlayerGetUpState : public PlayerStateBase
{
public:
	PlayerGetUpState(Player& player, GetUp getUp);

	void Enter()override;
	void Update()override;
	void Exit()override;
	
	bool IsHittable()const override{ return false; }
	bool IsThrowable()const override{ return false; }
	bool IsGuarding()const override{ return IsGuardInput(); }
	bool IsFacingOpponent()const override{ return false; }

	bool UseRootMotion()const override { return true; }

private:
	GetUp m_getUp;
};

