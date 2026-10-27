#pragma once
#include"Engine/Math/Vector3.h"
#include"Game/Common/PlayerID.h"
#include<vector>

class Player;
class HitBoxSet;
struct AttackData;

/// <summary>
/// 攻撃が当たったという情報
/// </summary>
struct HitEvent
{
	//攻撃した側
	PlayerID attacker;
	//攻撃を受けた側
	PlayerID defender;
	//技の性能
	const AttackData* attack;
	//攻撃の番号
	int attackInstanceId;
	//当たった位置
	Vector3 point;
};
/// <summary>
/// プレイヤー同士の当たり判定の処理
/// </summary>
class BattleCollision
{
public:
	/// <summary>
	/// 押し出し判定がめり込んでいたら押し戻す
	/// </summary>
	/// <param name="p1">プレイヤー1</param>
	/// <param name="p2">プレイヤー2</param>
	void ResolvePush(Player& p1, Player& p2);
	/// <summary>
	/// 両方の攻撃を調べて当たったものを返す
	/// </summary>
	/// <param name="p1">プレイヤー1</param>
	/// <param name="p2">プレイヤー2</param>
	/// <returns></returns>
	std::vector<HitEvent> DetectHits(const Player& p1, const Player& p2)const;

private:
	/// <summary>
	/// 攻撃が当たっているかどうかを調べる
	/// </summary>
	/// <param name="attacker">攻撃側</param>
	/// <param name="defender">受ける側</param>
	/// <param name="events">当たった情報</param>
	void DetectOneWay(const HitBoxSet& attacker, const HitBoxSet& defender, std::vector<HitEvent>& events)const;
};

