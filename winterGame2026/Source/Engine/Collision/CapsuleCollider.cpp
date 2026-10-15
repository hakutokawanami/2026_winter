#include "CapsuleCollider.h"
#include<DxLib.h>
CapsuleCollider::CapsuleCollider() :
	m_info({})
{
}

void CapsuleCollider::Init(const Vector3& pos1, const Vector3& pos2, float radius)
{
	m_info.start = pos1;
	m_info.end = pos2;
	m_info.radius = radius;
}

void CapsuleCollider::Update(const Vector3& pos1, const Vector3& pos2)
{
}

void CapsuleCollider::Draw()
{
#ifdef _DEBUG
	if (m_isActive)
	{
		DrawCapsule3D(m_info.start, m_info.end,
			m_info.radius, 8, 0xff0000, 0xff0000, false);
	}
#endif // _DEBUG
}
