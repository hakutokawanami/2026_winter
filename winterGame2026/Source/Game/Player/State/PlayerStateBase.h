#pragma once
#include"Game/Base/StateBase.h"
#include"Engine/Animation/AnimationController.h"
#include"Game/Player/Player.h"
#include"Engine/Input/InputManager.h"
#include"Game/Battle/BattleManager.h"
#include <Engine/Input/StickDirection.h>

class PlayerStateBase : public StateBase
{
public:
	PlayerStateBase(Player& player);

protected:
	/// <summary>
	/// プレイヤーの向いている方向に応じて入力を反転させる関数
	/// </summary>
	/// <param name="direction"></param>
	/// <returns></returns>
	StickDirection ToScreenDirection(StickDirection direction)const;

	Player& m_player;

	AnimationController& m_animationController;

	BattleManager* m_battleManager;
};

