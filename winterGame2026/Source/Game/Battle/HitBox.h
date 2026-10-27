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
/// <param name="bone">ボーンの名前</param>
/// <param name="length">長さ</param>
/// <param name="radius">半径</param>
/// <param name="offsetY">ボーンの位置からのオフセット</param>
/// </summary>
struct BoneCapsuleDef
{
	//カプセルの中心のボーンの名前
	const wchar_t* bone;
	//長さ
	float length;
	//半径
	float radius;
	//ボーンの位置からのオフセット
	float offsetY = 0.0f;
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
	const AttackData* attack = nullptr;
	//技を出すたびに代わる番号
	int attackInstanceID = 0;
};