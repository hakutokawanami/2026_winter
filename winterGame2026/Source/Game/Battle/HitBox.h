#pragma once
#include"Engine/Collision/Shape.h"
#include"game/Common/PlayerID.h"

/// <summary>
/// 判定の種類
/// </summary>
enum class HitBoxType
{
	//押し出し
	push,
	//やられ
	hurt,
	//攻撃
	attack
};

/// <summary>
/// 当たり判定一つの構成
/// </summary>
struct HitBox
{
	//種類
	HitBoxType type = HitBoxType::push;
	//持ち主
	PlayerID owner = PlayerID::P1;
	//形
	Shape shape;
	//判定を行うかどうか
	bool isActive = true;
};