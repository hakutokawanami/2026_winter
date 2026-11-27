#pragma once
#include"CameraStateBase.h"

/// <summary>
/// 試合開始時の演出カメラ
/// </summary>
class CameraRoundStartState : public CameraStateBase
{
public:
	CameraRoundStartState(BattleCamera& camera);

	void Enter()override;
	void Update()override;

	bool IsFinished()const override;

private:
	//演出が始まってからのフレーム数
	int m_frame;
};
