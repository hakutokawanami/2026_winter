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
	normal.Normalize();
	//めり込み量の半分
	float half = contact.penetration * 0.5f;
	//半分ずつ押し戻す
	p1.GetTransform().Translate(normal * half);
	p2.GetTransform().Translate(-normal * half);
}

std::vector<HitEvent> BattleCollision::DetectHits(const Player& p1, const Player& p2) const
{
	std::vector<HitEvent>events;
	//P1の攻撃 → P2
	DetectOneWay(p1.GetHitBoxes(), p2.GetHitBoxes(), events);
	//P2の攻撃 → P1
	DetectOneWay(p2.GetHitBoxes(), p1.GetHitBoxes(), events);
	return events;
}

void BattleCollision::DetectOneWay(const HitBoxSet& attacker, const HitBoxSet& defender, std::vector<HitEvent>& events) const
{
	//攻撃判定
	const HitBox& attackBox = attacker.GetAttackBox();

	//攻撃判定が出ていないなら何もしない
	if (!attackBox.isActive)
	{
		return;
	}
	//すでに当たっていたら何もしない
	if (attackBox.attackInstanceID == defender.GetLastReceivedAttackID())
	{
		return;
	}

	//すべてのやられ判定分調べる
	for (const HitBox& hurtBox : defender.GetHurtBoxes())
	{
		//判定を行わないなら何もしない
		if (!hurtBox.isActive)
		{
			continue;
		}
		//当たってるか調べる
		Contact contact = Collision::Intersect(attackBox.shape, hurtBox.shape);
		//当たっていないなら何もしない
		if (!contact.isHit)
		{
			continue;
		}

		//当たった情報
		HitEvent event;
		event.attacker = attackBox.owner;
		event.defender = hurtBox.owner;
		event.attack = attackBox.attack;
		event.attackInstanceId = attackBox.attackInstanceID;
		event.point = contact.point;
		events.push_back(event);

		//やられ判定のどれか一つに当たれば終わる
		break;
	}
}
