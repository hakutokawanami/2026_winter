#include "HitBoxSet.h"
#include<DxLib.h>
#include<algorithm>
#include<cassert>
#undef max


HitBoxSet::HitBoxSet(PlayerID owner,int modelHandle):
	m_owner(owner),
	m_modelHandle(modelHandle),
	m_pushBox(),
	m_pushBoxHeight(0.0f),
	m_attackBox(),
	m_attackAttachment(-1,{}),
	m_lastReceivedAttackID(0)
{
	//押し出し判定を設定
	m_pushBox.type = HitBoxType::push;
	m_pushBox.owner = owner;
	m_pushBox.shape = Capsule{};

	//攻撃判定
	m_attackBox.type = HitBoxType::attack;
	m_attackBox.owner = owner;
	m_attackBox.shape = Capsule{};
	m_attackBox.isActive = false;
}

void HitBoxSet::SetPushBox(float radius, float height)
{
	//カプセルの半径と高さを設定
	std::get<Capsule>(m_pushBox.shape).radius = radius;
	m_pushBoxHeight = height;
}

void HitBoxSet::AddHurtBox(const BoneCapsuleDef& def)
{
	HitBox box;
	box.type = HitBoxType::hurt;
	box.owner = m_owner;
	box.shape = Capsule{ {},{},def.radius };

	m_hurtBoxes.push_back(box);
	m_hurtAttachments.push_back({ FindBones(def.bone),def});
}

void HitBoxSet::StartAttack(const BoneCapsuleDef& def, const AttackData& data, int instanceID)
{
	m_attackBox.shape = Capsule{ {}, {}, def.radius };
	m_attackBox.attack = &data;
	m_attackBox.attackInstanceID = instanceID;
	m_attackBox.isActive = true;
	m_attackAttachment = { FindBones(def.bone),def};

	UpdateBoneCapsule(m_attackBox, m_attackAttachment);
}

void HitBoxSet::EndAttack()
{
	m_attackBox.isActive = false;
	m_attackBox.attack = nullptr;
}

void HitBoxSet::Update(const Vector3& playerPos)
{
	auto& capsule = std::get<Capsule>(m_pushBox.shape);
	//カプセルの下の端を半径分上にあげておく
	capsule.start = playerPos + Vector3::Up() * capsule.radius;
	//カプセルの上の端を半径分下に下げておく
	//半径よりも小さいなら半径の長さにする
	float top = std::max(capsule.radius, m_pushBoxHeight - capsule.radius);
	capsule.end = playerPos + Vector3::Up() * top;

	//やられ判定をボーンに合わせる
	for (size_t i = 0; i < m_hurtBoxes.size(); i++)
	{
		UpdateBoneCapsule(m_hurtBoxes[i], m_hurtAttachments[i]);
	}
	//攻撃判定をボーンに合わせる
	if (m_attackBox.isActive)
	{
		UpdateBoneCapsule(m_attackBox, m_attackAttachment);
	}
}

void HitBoxSet::Draw()
{
#ifdef _DEBUG
	//カプセルの描画
	auto DrawCapsule = [](const HitBox& box, unsigned int color)
		{
			if (!box.isActive)
			{
				return;
			}

			const auto& capsule = std::get<Capsule>(box.shape);
			DrawCapsule3D(capsule.start, capsule.end, capsule.radius, 8, color, color, false);
		};

	//押し出し:青 やられ:緑 攻撃:赤
	DrawCapsule(m_pushBox, 0x4080ff);
	for (const auto& box : m_hurtBoxes)
	{
		DrawCapsule(box, 0x40ff40);
	}
	DrawCapsule(m_attackBox, 0xff4040);
#endif // _DEBUG
}

int HitBoxSet::FindBones(const wchar_t* boneName) const
{
	int frame = MV1SearchFrame(m_modelHandle, boneName);
	//ボーンが見つからない時
	assert(frame>= 0 && "ボーンが見つかりません");

	return frame;
}

void HitBoxSet::UpdateBoneCapsule(HitBox& box, const BoneAttachment& attachment)
{
	const BoneCapsuleDef& def = attachment.def;

	//カプセルの中心をボーンの位置とオフセットから決める
	Vector3 center;
	center = MV1GetFramePosition(m_modelHandle, attachment.frame);
	center.y += def.offsetY;

	//中心から上下に長さの半分ずつ伸ばす
	Vector3 half = Vector3::Up() * (def.length * 0.5f);
	auto& capsule = std::get<Capsule>(box.shape);
	capsule.start = center - half;
	capsule.end = center + half;
}
