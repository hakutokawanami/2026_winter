#pragma once
#include "CameraBase.h"
#include<memory>

class Player;
class BattleCamera : public CameraBase
{
public:
	BattleCamera();
	~BattleCamera()override;

	void Init()override;
	void Update()override;

	void SetPlayers(std::shared_ptr<Player>player1, std::shared_ptr<Player>player2);

private:
	std::weak_ptr<Player>m_player1;
	std::weak_ptr<Player>m_player2;
};

