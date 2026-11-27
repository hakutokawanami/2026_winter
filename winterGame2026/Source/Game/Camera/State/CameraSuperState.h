#pragma once
#include"CameraStateBase.h"

class Player;

/// <summary>
/// 超必殺技の演出カメラ
/// </summary>
class CameraSuperState : public CameraStateBase
{
public:
	/// <param name="camera"></param>
	/// <param name="user">超必殺技を出したプレイヤー</param>
	CameraSuperState(BattleCamera& camera, Player* user);

	void Enter()override;
	void Update()override;

	bool IsFinished()const override;

private:
	//超必殺技を出したプレイヤー
	Player* m_user;
	//演出が始まってからのフレーム数
	int m_frame;
};
