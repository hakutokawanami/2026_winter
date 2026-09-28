#pragma once
#include"Engine/Math/Vector3.h"

class CameraBase
{
public:
	virtual ~CameraBase() = default;

	virtual void Init()abstract;
	virtual void Update()abstract;

protected:
	Vector3 m_position;

	Vector3 m_target;
};

