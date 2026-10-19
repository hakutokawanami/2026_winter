#include "BattleCollision.h"
#include"HitBoxSet.h"
#include"Engine/Collision/Collision.h"
#include"Game/Player/Player.h"

namespace
{
	//0とみなす小さな値
	constexpr float kEpsilon = 1.0e-4f;
}

void BattleCollision::ResolvePush(Player& p1, Player& p2)
{
	const HitBox& push1 = p1.GetHitBoxes().GetPushBox();
	const HitBox& push2 = p2.GetHitBoxes().GetPushBox();
	//どちらかの判定が無効なら何もしない
	if (!push1.isActive || !push2.isActive)
	{
		return;
	}
	
	//判定をした結果
	Contact contact = Collision::Intersect(push1.shape, push2.shape);

	//押し出し判定同士が当たっていないなら何もしない
	if (!contact.isHit)
	{
		return;
	}

	//押し戻す向きのy成分を消す
	Vector3 normal = contact.normal;
	normal.y = 0.0f;

	//重なっていて押し戻しの方向が決まらないときは位置から向きを決める
	if (normal.Length() < kEpsilon)
	{
		normal = p1.GetTransform().GetPosition() - p2.GetTransform().GetPosition();
		normal.y = 0.0f;
		//位置も同じなら適当な向きにする
		if (normal.Length() < kEpsilon)
		{
			normal = Vector3::Right();
		}
	}
	//正規化
	normal.Normalized();
	//めり込み量の半分
	float half = contact.penetration * 0.5f;
	//半分ずつ押し戻す
	p1.GetTransform().Translate(normal * half);
	p2.GetTransform().Translate(-normal * half);
}
