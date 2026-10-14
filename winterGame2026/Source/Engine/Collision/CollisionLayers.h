#pragma once
#include<cstdint>

using CollisionLayer = uint32_t;

namespace CollisionLayers
{
	constexpr CollisionLayer kNone = 0;
	//攻撃判定
	constexpr CollisionLayer kAttack = 1 << 0;
	//攻撃を受ける判定
	constexpr CollisionLayer kHurt = 1 << 1;
	//押し出し判定
	constexpr CollisionLayer kPush = 1 << 2;
	//つかみ判定
	constexpr CollisionLayer kGrab = 1 << 3;
	//つかみを受ける判定
	constexpr CollisionLayer kGrabbable = 1 << 4;
	//ステージ判定
	constexpr CollisionLayer kStage = 1 << 5;
}