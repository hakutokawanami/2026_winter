#include "CommandInput.h"
#undef max

namespace
{
	//履歴の最大サイズ
	constexpr size_t kMaxHistory = 60;
}

void CommandInput::Update(StickDirection screenDir, bool isOpponentOnRight)
{
	m_history.push_back(ToNumpad(screenDir, isOpponentOnRight));
	//古いものから捨てる
	if (m_history.size() > kMaxHistory)
	{
		m_history.pop_front();
	}
}

bool CommandInput::IsInput(std::string_view command, int maxFrame) const
{
	if (command.empty())
	{
		return false;
	}

	//コマンド履歴の最後から探していく
	int step = static_cast<int>(command.size()) - 1;

	const int size = static_cast<int>(m_history.size());
	//maxFrameより前の履歴は見ない(0まで)
	const int oldest = std::max(0, size - maxFrame);

	//新しいほうから探していく
	for (int i = size - 1; i >= oldest; i--)
	{
		//見つかったら
		if (m_history[i] == command[step])
		{
			//次へ
			step--;
			//全部見つかったらコマンド成立
			if (step < 0)
			{
				return true;
			}
		}
	}

	return false;
}

char CommandInput::ToNumpad(StickDirection screenDir, bool isOpponentRight)
{
	//右を前としてそろえる
	const StickDirection dir = isOpponentRight ? screenDir : Mirror(screenDir);

	//テンキーに変換
	switch (dir)
	{
	case StickDirection::downLeft:  return '1';
	case StickDirection::down:      return '2';
	case StickDirection::downRight: return '3';
	case StickDirection::left:      return '4';
	case StickDirection::right:     return '6';
	case StickDirection::upLeft:    return '7';
	case StickDirection::up:        return '8';
	case StickDirection::upRight:   return '9';
	default:                        return '5';
	}
}
