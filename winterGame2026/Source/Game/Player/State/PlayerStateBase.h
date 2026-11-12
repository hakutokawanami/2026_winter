#pragma once
#include"Game/Base/StateBase.h"
#include"Engine/Animation/AnimationController.h"
#include"Game/Player/Player.h"
#include"Engine/Input/InputManager.h"
#include"Game/Battle/BattleManager.h"
#include <Engine/Input/StickDirection.h>
#include"Engine/Input/Button.h"

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
	/// 攻撃が当たる状態かどうか
	/// </summary>
	/// <returns></returns>
	virtual bool IsHittable()const { return true; }
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
	/// <summary>
	/// 体の向きを固定するかどうか
	/// </summary>
	/// <returns></returns>
	virtual bool IsFacingOpponent()const { return true; }

	/// <summary>
	/// 体の向きを変えないようにするかどうか
	/// (アニメーションの位置合わせ中など、回転させると体が振り回される時にtrue)
	/// </summary>
	/// <returns></returns>
	virtual bool IsRotationLocked()const { return false; }

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

	/// <summary>
	/// ステートから使うIsTriggerd
	/// </summary>
	/// <param name="button"></param>
	/// <returns></returns>
	bool IsTriggered(const char* button)const;
	/// <summary>
	/// ステートから使うGetStickDirection
	/// </summary>
	/// <returns></returns>
	StickDirection GetStickDirection()const;
	/// <summary>
	/// ステートから使うIsStickDoubleTap
	/// </summary>
	/// <param name="dir"></param>
	/// <returns></returns>
	bool IsStickDoubleTap(StickDirection dir)const;
	/// <summary>
	/// ステートから使うIsBufferd
	/// </summary>
	/// <param name="button"></param>
	/// <returns></returns>
	bool IsBufferd(Button button,int frame)const;

	Player& m_player;

	AnimationController& m_animationController;

	BattleManager* m_battleManager;
};

