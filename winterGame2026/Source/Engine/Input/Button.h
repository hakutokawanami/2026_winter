#pragma once
#include<array>

//攻撃に使うボタン
enum class Button
{
	A,
	B,
	X,
	Y,
	count
};

//ボタンの名前
inline constexpr std::array<const char*, static_cast<size_t>(Button::count)>kButtonNames =
{
	"A","B","X","Y"
};

//名前にして受け取る
inline constexpr const char* ToName(Button button)
{
	return kButtonNames[static_cast<size_t>(button)];
}
