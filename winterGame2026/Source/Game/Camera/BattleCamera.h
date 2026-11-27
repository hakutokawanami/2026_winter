#pragma once
#include "CameraBase.h"
#include<memory>

class Player;
class CameraStateBase;
class BattleCamera : public CameraBase
{
public:
	// カメラの高さ
	static constexpr float kCameraHeightOffset = 260.0f;
	// カメラのオフセット
	static constexpr float kCameraDistanceOffset = 1000.0f;
	// カメラの最小距離
	static constexpr float kCameraDistanceMin = 500.0f;
	// カメラの最大距離
	static constexpr float kCameraDistanceMax = 1500.0f;
	//カメラの距離に使うプレイヤー同士の距離に掛ける倍率
	static constexpr float kDistanceRate = 0.8f;

	BattleCamera();
	~BattleCamera()override;

	void Init()override;
	void Init(Player* player1, Player* player2);
	void Update()override;

	void SetPlayers(Player* player1, Player* player2);
	/// <summary>
	/// カメラのステートを切り替える
	/// </summary>
	/// <param name="newState"></param>
	void ChangeState(std::unique_ptr<CameraStateBase> newState);
	/// <summary>
	/// 2人を横から映す位置と注視点を計算する
	/// </summary>
	/// <param name="player1Pos"></param>
	/// <param name="player2Pos"></param>
	void CalculateSideView(const Vector3& player1Pos, const Vector3& player2Pos);
	// カメラの位置とターゲットを補間する関数
	void LerpCamera();
	//補完せずに直接カメラの位置とターゲットを設定する関数
	void SetCameraPositionAndTarget();
	//DXライブラリにカメラの位置とターゲットを適用する関数
	void Apply();

	//揺れ開始の設定
	void ShakeStart(float power = 10.0f,int time = 10);
	//揺れの更新
	void ShakeUpdate();

	Player* GetPlayer1()const { return m_player1; }
	Player* GetPlayer2()const { return m_player2; }

	void SetCalculatedPosition(const Vector3& pos)override { m_calculatedPosition = pos; }

private:
	Player* m_player1;
	Player* m_player2;

	//今のカメラのステート
	std::unique_ptr<CameraStateBase> m_state;
	//揺れの力
	float m_shakePower;
	//揺れる時間
	int m_shakeTime;
	//揺れのオフセット
	Vector3 m_shakeOffset;
};
