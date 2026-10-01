#pragma once
#include"Engine/Math/Vector3.h"

class CameraBase
{
public:
	virtual ~CameraBase() = default;

	virtual void Init()abstract;
	virtual void Update()abstract;

protected:
	//カメラの位置
	Vector3 m_position = {};
	//カメラのターゲット
	Vector3 m_target = {};
	//計算されたカメラの位置
	Vector3 m_calculatedPosition = {};
	//計算されたカメラのターゲット
	Vector3 m_calculatedTarget = {};
};

