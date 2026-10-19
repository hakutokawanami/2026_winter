#include "HitBoxSet.h"
#include<DxLib.h>
#include<algorithm>
#undef max


HitBoxSet::HitBoxSet(PlayerID owner):
	m_pushBox(),
	m_pushBoxHeight(0.0f)
{
	//押し出し判定を設定
	m_pushBox.type = HitBoxType::push;
	m_pushBox.owner = owner;
	m_pushBox.shape = Capsule{};
}

void HitBoxSet::SetPushBox(float radius, float height)
{
	//カプセルの半径と高さを設定
	std::get<Capsule>(m_pushBox.shape).radius = radius;
	m_pushBoxHeight = height;
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
}

void HitBoxSet::Draw()
{
#ifdef _DEBUG
	if (!m_pushBox.isActive)
	{
		return;
	}
	const auto& capsule = std::get<Capsule>(m_pushBox.shape);
	DrawCapsule3D(capsule.start, capsule.end, capsule.radius, 8, 0x4080ff, 0x4080ff, false);
#endif // _DEBUG

}
