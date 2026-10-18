#pragma once
#include"Engine/Math/Vector3.h"
#include<variant>

/// <summary>
/// 球
/// </summary>
struct Sphere
{
	Vector3 center;
	float radius = 0.0f;
};

/// <summary>
/// カプセル
/// </summary>
struct Capsule
{
	Vector3 start;
	Vector3 end;
	float radius;
};

//球かカプセルのどちらかを入れれる型
using Shape = std::variant<Sphere, Capsule>;

/// <summary>
/// 判定結果
/// </summary>
struct Contact
{
	//当たってるかどうか
	bool isHit = false;
	//bからaに向かうベクトル
	Vector3 normal;
	//めり込み量
	float penetration = 0.0f;
	//接触した位置
	Vector3 point;
};

