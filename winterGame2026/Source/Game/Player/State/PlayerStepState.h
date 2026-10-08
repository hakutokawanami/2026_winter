#pragma once
#include "PlayerStateBase.h"
class PlayerStepState : public PlayerStateBase
{
public:
	/// <summary>
	/// ステップの種類
	/// </summary>
	enum class Type
	{
		forward,//前ステップ
		back,//後ろステップ
		sideIn,//画面奥への横ステップ
		sideOut,//画面手前への横ステップ
	};

	PlayerStepState(Player& player,Type type);

	void Enter()override;
	void Update()override;
	void Exit()override;

private:
	//ステップの種類
	Type m_type;
	//移動する方向
	Vector3 m_direction;
	//経過フレーム
	int m_frame;
};

