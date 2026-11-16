#pragma once
#include<array>
#include<string_view>

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

//名前からボタンを受け取る(見つからなければButton::count)
inline Button FromName(std::string_view name)
{
	for (size_t i = 0; i < kButtonNames.size(); i++)
	{
		if (name == kButtonNames[i])
		{
			return static_cast<Button>(i);
		}
	}
	return Button::count;
}
