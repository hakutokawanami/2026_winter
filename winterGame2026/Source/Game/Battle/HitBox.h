#pragma once
#include"Engine/Collision/Shape.h"
#include"game/Common/PlayerID.h"

struct AttackData;

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
/// ボーンにつけるカプセル
/// </summary>
struct BoneCapsuleDef
{
	//カプセルの始点のボーンの名前
	const wchar_t* startBone;
	//カプセルの終点のボーンの名前
	const wchar_t* endBone;
	//半径
	float radius;
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

	//技の性能
	const AttackData* kAttack = nullptr;
	//技を出すたびに代わる番号
	int m_attackInstanceID = 0;
};