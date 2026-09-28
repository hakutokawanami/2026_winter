#pragma once
#include "CameraBase.h"

class Player;
class BattleCamera : public CameraBase
{
public:
	BattleCamera();
	~BattleCamera()override;

	void Init()override;
	void Update()override;

	void SetPlayers(Player& player1, Player& player2);

private:
};

