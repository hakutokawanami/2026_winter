#pragma once
#include"Game/Base/StateBase.h"
#include"Engine/Animation/AnimationController.h"
#include"Game/Player/Player.h"
#include"Engine/Input/InputManager.h"
#include"Game/Battle/BattleManager.h"

class PlayerStateBase : public StateBase
{
public:
	PlayerStateBase(Player& player);

protected:
	Player& m_player;

	AnimationController& m_animationController;

	BattleManager* m_battleManager;
};

