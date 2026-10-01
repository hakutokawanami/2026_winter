#pragma once
#include "CameraBase.h"
#include<memory>

/// <summary>
/// カメラの状態を表す列挙型()
/// </summary>
enum class CameraState
{
	// バトルカメラ
	Battle,
};

class Player;
class BattleCamera : public CameraBase
{
public:
	BattleCamera();
	~BattleCamera()override;

	void Init()override;
	void Init(std::shared_ptr<Player>player1, std::shared_ptr<Player>player2);
	void Update()override;

	void SetPlayers(std::shared_ptr<Player>player1, std::shared_ptr<Player>player2);
	// カメラの位置とターゲットを計算する関数
	void CalculateCameraPositionAndTarget();
	// カメラの位置とターゲットを補間する関数
	void LerpCamera();
	//補完せずに直接カメラの位置とターゲットを設定する関数
	void SetCameraPositionAndTarget();
	//DXライブラリにカメラの位置とターゲットを適用する関数
	void Apply();

private:
	std::weak_ptr<Player>m_player1;
	std::weak_ptr<Player>m_player2;

	CameraState m_cameraState;
};

