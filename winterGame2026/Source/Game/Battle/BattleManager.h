#pragma once
#include"Game/Common/PlayerID.h"
#include"Engine/Math/Vector3.h"

class Player;
class BattleManager
{
public:
	BattleManager();
	~BattleManager();
	void Init(Player* player1, Player* player2);
	void Update();
	void Draw();

	Player* GetPlayer(PlayerID id) const;
	/// <summary>
	/// プレイヤー間の中心位置を取得する
	/// </summary>
	/// <returns></returns>
	Vector3 GetCenterPosition() const;
	/// <summary>
	/// P1からP2への方向ベクトルを取得する
	/// </summary>
	/// <returns></returns>
	Vector3 GetDirection() const;
	/// <summary>
	/// プレイヤー間の距離を取得する
	/// </summary>
	/// <returns></returns>
	float GetDistance() const;

private:
	Player* m_player1;
	Player* m_player2;
	// プレイヤー間の中心位置
	Vector3 m_centerPosition;
	//P1からP2への方向ベクトル
	Vector3 m_direction;
	// プレイヤー間の距離
	float m_distance;
};

