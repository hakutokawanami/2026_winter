#pragma once
#include"Game/Base/StateBase.h"
#include"Engine/Animation/AnimationController.h"
#include"Game/Player/Player.h"
#include"Engine/Input/InputManager.h"

class PlayerStateBase : public StateBase
{
public:
	PlayerStateBase(Player& player, AnimationController& anim);

protected:
	Player& m_player;

	AnimationController& m_animationController;
};

