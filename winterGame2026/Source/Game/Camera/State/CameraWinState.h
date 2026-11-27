#pragma once
#include"CameraStateBase.h"

class Player;

/// <summary>
/// 勝利時の演出カメラ
/// (ラウンドがリセットされるまで続くので、IsFinishedは常にfalse)
/// </summary>
class CameraWinState : public CameraStateBase
{
public:
	/// <param name="camera"></param>
	/// <param name="winner">勝ったプレイヤー</param>
	CameraWinState(BattleCamera& camera, Player* winner);

	void Enter()override;
	void Update()override;

private:
	//勝ったプレイヤー
	Player* m_winner;
	//演出が始まってからのフレーム数
	int m_frame;
};
