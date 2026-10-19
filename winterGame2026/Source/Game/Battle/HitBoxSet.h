#pragma once
#include"HitBox.h"
#include"Engine/Math/Vector3.h"
#include"Game/Common/PlayerID.h"

class HitBoxSet
{
public:
	HitBoxSet(PlayerID owner);

	/// <summary>
	/// 押し出し判定の大きさを設定する
	/// </summary>
	/// <param name="radius">半径</param>
	/// <param name="height">高さ</param>
	void SetPushBox(float radius,float height);
	/// <summary>
	/// プレイヤーの位置に合わせて判定を更新する
	/// </summary>
	/// <param name="playerPos">プレイヤーの位置</param>
	void Update(const Vector3& playerPos);
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 押し出し判定を取得する関数
	/// </summary>
	/// <returns></returns>
	const HitBox& GetPushBox()const { return m_pushBox; }

private:
	//押し出し判定
	HitBox m_pushBox;
	//押し出し判定の高さ
	float m_pushBoxHeight;
};

