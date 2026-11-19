#pragma once
#include "PlayerStateBase.h"
class PlayerKnockbackState : public PlayerStateBase
{
public:
	using PlayerStateBase::PlayerStateBase;

	void Enter()override;
	void Update()override;
	void Exit()override;

	bool IsHittable()const override { return false; }
	bool IsThrowable()const override { return false; }
	bool IsRotationLocked()const override { return true; }
	bool UseRootMotion()const override { return true; }
};

