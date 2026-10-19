#pragma once
#include"Game/Common/PlayerID.h"
#include"Engine/Math/Vector3.h"
#include"BattleCollision.h"

class Player;
class BattleManager
{
public:
	BattleManager();
	~BattleManager();
	void Init(Player* player1, Player* player2);
	void Update();
	void Draw();

	/// <summary>
	/// プレイヤー間の中心位置を計算する
	/// </summary>
	/// <param name="pos1">プレイヤー1の位置</param>
	/// <param name="pos2">プレイヤー2の位置</param>
	void CalculateCenter(const Vector3& pos1, const Vector3& pos2);

	/// <summary>
	/// P1からP2への方向ベクトルと距離を計算する
	/// </summary>
	/// <param name="pos1">プレイヤー1の位置</param>
	/// <param name="pos2">プレイヤー2の位置</param>
	void CalculateDirectionAndDistance(const Vector3& pos1, const Vector3& pos2);

	/// <summary>
	/// 指定されたIDのプレイヤーを取得する
	/// </summary>
	/// <param name="id">取得したいプレイヤーのID</param>
	/// <returns></returns>
	Player* GetPlayer(PlayerID id) const;

	/// <summary>
	/// 自分のIDを指定して、相手のプレイヤーを取得する
	/// </summary>
	/// <param name="id">自分のプレイヤーID</param>
	/// <returns></returns>
	Player* GetOpponentPlayer(PlayerID id) const;

	/// <summary>
	/// プレイヤー間の中心位置を取得する
	/// </summary>
	/// <returns></returns>
	Vector3 GetCenterPosition() const;
	/// <summary>
	/// 相手のプレイヤーへの方向ベクトルを取得する
	/// </summary>
	/// <returns></returns>
	Vector3 GetOpponentDirection(PlayerID id) const;
	/// <summary>
	/// プレイヤー間の距離を取得する
	/// </summary>
	/// <returns></returns>
	float GetDistance() const;
	/// <summary>
	/// 右方向へのベクトル
	/// </summary>
	/// <returns></returns>
	Vector3 GetRightDirection() const;

	/// <summary>
	/// 前方向へのベクトル
	/// </summary>
	/// <returns></returns>
	Vector3 GetForwardDirection()const;

	/// <summary>
	/// 相手のプレイヤーが右側にいるかどうか
	/// </summary>
	/// <param name="id">自分のプレイヤーID</param>
	/// <returns></returns>
	bool IsOpponentOnRight(PlayerID id)const;

private:
	Player* m_player1;
	Player* m_player2;
	// プレイヤー間の中心位置
	Vector3 m_centerPosition;
	//P1からP2への方向ベクトル
	Vector3 m_direction;
	// プレイヤー間の距離
	float m_distance;
	//プレイヤー同士の当たり判定の処理
	BattleCollision m_collision;
};

