#pragma once
#include"Game/Base/StateBase.h"

class BattleCamera;

/// <summary>
/// カメラのステートの基底クラス
/// </summary>
class CameraStateBase : public StateBase
{
public:
	CameraStateBase(BattleCamera& camera);

	void Enter()override {}
	void Exit()override {}

	/// <summary>
	/// 投げ用のカメラかどうか
	/// </summary>
	/// <returns></returns>
	virtual bool IsThrow()const { return false; }

	/// <summary>
	/// 演出が終わったかどうか
	/// </summary>
	/// <returns></returns>
	virtual bool IsFinished()const { return false; }

protected:
	BattleCamera& m_camera;
};
