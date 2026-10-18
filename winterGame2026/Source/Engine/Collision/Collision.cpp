#include "Collision.h"
#include<DxLib.h>

namespace
{
	//0とみなす小さな値
	constexpr float kEpsilon = 1.0e-6f;

	/// <summary>
	/// 2つの球の当たり判定
	/// カプセルも一番近い点を中心にした球にして最後にこの関数で判定する
	/// </summary>
	/// <param name="posA">Aの中心</param>
	/// <param name="radiusA">Aの半径</param>
	/// <param name="posB">Bの中心</param>
	/// <param name="radiusB">Bの半径</param>
	/// <returns></returns>
	Contact MakeContact(const Vector3& posA, float radiusA, 
		const Vector3& posB, float radiusB)
	{
		//Bの中心からAの中心へのベクトル
		Vector3 diff = posA - posB;
		//中心同士の距離
		float dist = diff.Length();
		//半径の合計
		float radiusSum = radiusA + radiusB;
		//距離が半径の合計以上なら当たってない
		if (dist >= radiusSum)
		{
			return Contact{};
		}

		Contact contact;
		//当たった
		contact.isHit = true;
		//めり込み量
		contact.penetration = radiusSum - dist;
		//BからAに向かうベクトル
		//中心が重なっていたら適当な向き
		contact.normal = (dist > kEpsilon) ? diff / dist : Vector3::Right();
		//Bの表面とAの表面の中間を接触位置にする
		contact.point = posB + contact.normal * (radiusB - contact.penetration * 0.5f);

		return contact;
	}
}

namespace Collision
{
	Contact Intersect(const Sphere& a, const Sphere& b)
	{
		//球同士はそのまま判定
		return MakeContact(a.center, a.radius, b.center, b.radius);
	}

	Contact Intersect(const Capsule& a,const Sphere& b)
	{
		//カプセルの線分上で球の中心に一番近い点を求める
		VECTOR start = a.start;
		VECTOR end = a.end;
		VECTOR center = b.center;
		SEGMENT_POINT_RESULT result;
		Segment_Point_Analyse(&start, &end, &center, &result);

		//一番近い点
		Vector3 closest;
		closest = result.Seg_MinDist_Pos;
		//一番近い点を中心にした球として判定
		return MakeContact(closest, a.radius, b.center, b.radius);
	}

	Contact Intersect(const Sphere& a, const Capsule& b)
	{
		//カプセルと球の判定を使う
		Contact contact = Intersect(b, a);
		//法線の向きだけ反転
		contact.normal = -contact.normal;
		return contact;
	}

	Contact Intersect(const Capsule& a, const Capsule& b)
	{
		//2本の線分でお互いに一番近い点を求める
		VECTOR aStart = a.start;
		VECTOR aEnd = a.end;
		VECTOR bStart = b.start;
		VECTOR bEnd = b.end;
		SEGMENT_SEGMENT_RESULT result;
		Segment_Segment_Analyse(&aStart, &aEnd, &bStart, &bEnd, &result);

		Vector3 closestA;
		Vector3 closestB;
		closestA = result.SegA_MinDist_Pos;
		closestB = result.SegB_MinDist_Pos;
		//それぞれの一番近い点を中心にした球で判定
		return MakeContact(closestA, a.radius, closestB, b.radius);
	}
}