#pragma once
#include"Engine/Math/Vector3.h"

/// <summary>
/// コライダーの種類
/// </summary>
enum ColliderType
{
	//カプセル
	capsule,
	//球
	sphere
};

/// <summary>
/// コライダー基底クラス
/// </summary>
class ColliderBase
{
public:
	virtual ~ColliderBase() = default;

	virtual void Update(const Vector3& pos)abstract;

	virtual void Draw()abstract;
	/// <summary>
	/// コライダーの種類を返す
	/// </summary>
	/// <returns></returns>
	virtual ColliderType GetType()const = 0;
	/// <summary>
	/// 当たり判定を行うかどうか
	/// </summary>
	/// <returns></returns>
	bool IsActive() { return m_isActive; }

protected:
	//当たり判定を行うかどうか
	bool m_isActive = true;
};

