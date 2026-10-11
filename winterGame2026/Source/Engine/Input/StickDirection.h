#pragma once

/// <summary>
///	スティックの8方向+ニュートラルの情報
/// </summary>
enum class StickDirection
{
	neutral,

	up,
	down,
	left,
	right,

	upLeft,
	upRight,
	downLeft,
	downRight
};
/// <summary>
/// 入力方向の左右を反転した方向を返す関数
/// </summary>
/// <param name="direction"></param>
/// <returns></returns>
constexpr StickDirection Mirror(StickDirection direction)
{
	switch (direction)
	{
	case StickDirection::left: return StickDirection::right;
	case StickDirection::right: return StickDirection::left;
	case StickDirection::upLeft: return StickDirection::upRight;
	case StickDirection::downLeft: return StickDirection::downRight;
	case StickDirection::upRight: return StickDirection::upLeft;
	case StickDirection::downRight: return StickDirection::downLeft;

	default: return direction;
	}
}