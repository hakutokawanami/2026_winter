#pragma once
#include"Button.h"
#include"StickDirection.h"

/// <summary>
/// 入力の種類の抽象基底クラス
/// </summary>
class IInputSource
{
public:
	virtual ~IInputSource() = default;

	virtual void Update() {}

	virtual bool IsTriggerd(Button button)const abstract;

	virtual StickDirection GetStickDirection()const abstract;

	virtual bool IsStickDoubleTap(StickDirection dir)const abstract;
};

