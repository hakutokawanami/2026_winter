#pragma once
#include "PlayerStateBase.h"
#include"Game/Battle/AttackData.h"

class PlayerHitState : public PlayerStateBase
{
public:
	PlayerHitState(Player& player, const AttackData& attack);

	void Enter()override;
	void Update()override;
	void Exit()override;

private:
	//受けた技の性能
	AttackData m_attack;
	//経過フレーム
	int m_frame;
	//吹き飛ぶ方向
	Vector3 m_knockbackDir;
};

