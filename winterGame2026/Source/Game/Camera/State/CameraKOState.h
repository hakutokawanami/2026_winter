#pragma once
#include"CameraStateBase.h"

class Player;

/// <summary>
/// KO時の演出カメラ
/// </summary>
class CameraKOState : public CameraStateBase
{
public:
	/// <param name="camera"></param>
	/// <param name="loser">KOされたプレイヤー</param>
	CameraKOState(BattleCamera& camera, Player* loser);

	void Enter()override;
	void Update()override;

	bool IsFinished()const override;

private:
	//KOされたプレイヤー
	Player* m_loser;
	//演出が始まってからのフレーム数
	int m_frame;
};
