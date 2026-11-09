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

	/// <summary>
	/// ガード中かどうか
	/// </summary>
	/// <returns></returns>
	virtual bool IsGuarding()const { return false; }
	/// <summary>
	/// 投げられる状態かどうか
	/// </summary>
	/// <returns></returns>
	virtual bool IsThrowable()const { return true; }
	/// <summary>
	/// 投げられているかどうか
	/// </summary>
	/// <returns></returns>
	virtual bool IsThrowing()const { return false; }

protected:
	/// <summary>
	/// プレイヤーの向いている方向に応じて入力を反転させる関数
	/// </summary>
	/// <param name="direction"></param>
	/// <returns></returns>
	StickDirection ToScreenDirection(StickDirection direction)const;

	/// <summary>
	/// スティックを後ろに倒しているか
	/// </summary>
	/// <returns></returns>
	bool IsBackInput()const;
	/// <summary>
	/// ガードの入力をしているか
	/// </summary>
	/// <returns></returns>
	bool IsGuardInput()const;

	Player& m_player;

	AnimationController& m_animationController;

	BattleManager* m_battleManager;
};

