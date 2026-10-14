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
		float height;
	};

	CapsuleCollider();
};

