#pragma once
#include "ColliderBase.h"

/// <summary>
/// カプセルコライダー
/// </summary>
class CapsuleCollider : public ColliderBase
{
public:
	/// <summary>
	/// カプセルを形成する情報
	/// </summary>
	struct CapsureInfo
	{
		Vector3 start;
		Vector3 end;
		float radius;
	};

	CapsuleCollider();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pos1">ポジション1</param>
	/// <param name="pos2">ポジション2</param>
	/// <param name="radius">半径</param>
	void Init(const Vector3& pos1, const Vector3& pos2, float radius);
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="pos1">ポジション1</param>
	/// <param name="pos2">ポジション2</param>
	void Update(const Vector3& pos1, const Vector3& pos2);
	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;
	/// <summary>
	/// コライダーの種類を返す
	/// </summary>
	/// <returns></returns>
	ColliderType GetType()const override{ return ColliderType::capsule; }

private:
	//カプセルを形成する情報
	CapsureInfo m_info;
};

