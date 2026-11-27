#pragma once
#include"CameraStateBase.h"

/// <summary>
/// 投げ中のカメラ
/// </summary>
class CameraThrowState : public CameraStateBase
{
public:
	
	CameraThrowState(BattleCamera& camera);

	void Update()override;

	bool IsThrow()const override { return true; }
};
