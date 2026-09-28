#pragma once
#include"Game/Base/GameObject.h"

enum class PlayerID
{
	P1,
	P2
};

class Player : public GameObject
{
public:
	Player(PlayerID id);
	~Player();

	void Init()override;
	void End()override;
	void Update()override;
	void Draw()override;

private:

	PlayerID m_playerID;
};

