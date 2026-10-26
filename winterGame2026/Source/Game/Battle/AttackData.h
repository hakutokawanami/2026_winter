#pragma once
#include"HitBox.h"
#include"Game/Player/Player.h"

/// <summary>
/// 技の性能
/// </summary>
struct AttackData
{
	//発生フレーム
	int startUp;
	//持続フレーム
	int active;
	//硬直フレーム
	int recovery;
	//ダメージ
	int damage;
	//ヒット時に相手が動けないフレーム
	int hitStun;
	//ガード時に相手が動けないフレーム
	int blockStun;
};

/// <summary>
/// 技一つの情報
/// </summary>
struct AttackDef
{
	//アニメーション名
	const wchar_t* animName;
	//攻撃判定を付ける場所
	BoneCapsuleDef box;
	//技性能
	AttackData data;
};

namespace AttackTable
{
	inline constexpr AttackDef kJab =
	{
		Player::kJabAnimName,
		{L"mixamorig:LeftHandMiddle4",L"mixamorig:LeftHand",30.0f},
		{ 6, 3, 10, 10, 15, 8},
	};
}