#pragma once
#include "PlayerStateBase.h"

/// <summary>
/// ガード耐久がなくなって無防備になっている状態
/// </summary>
class PlayerGuardBreakState : public PlayerStateBase
{
public:
	PlayerGuardBreakState(Player& player);

	void Enter()override;
	void Update()override;
	void Exit()override;

	bool IsGuardBroken()const override { return true; }

private:
	//経過フレーム
	int m_frame;
};
