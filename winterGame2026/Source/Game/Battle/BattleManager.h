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

	Vector3 GetCenterPosition() const;
	Vector3 GetDirection() const;
	float GetDistance() const;

private:
	Player* m_player1;
	Player* m_player2;

	Vector3 m_centerPosition;
	Vector3 m_direction;
	float m_distance;
};

