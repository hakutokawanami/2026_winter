#pragma once
#include "PlayerStateBase.h"
#include <Game/Player/Player.h>
class PlayerWalkState : public PlayerStateBase
{
public:
	/// <summary>
	/// 移動のタイプ
	/// </summary>
	enum class Type
	{
		forward,//前移動
		back,//後ろ移動
		sideIn,//画面奥への移動
		sideOut,//画面手前への移動
	};

	using PlayerStateBase::PlayerStateBase;

	void Enter()override;
	void Update()override;
	void Exit()override;

private:
	//移動の種類
	Type m_type;
	//移動する方向
	Vector3 m_direction;
};

