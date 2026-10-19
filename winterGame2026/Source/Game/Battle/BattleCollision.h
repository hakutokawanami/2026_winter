#pragma once

class Player;
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
};

