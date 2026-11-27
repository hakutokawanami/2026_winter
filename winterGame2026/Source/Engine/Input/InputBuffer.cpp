#include "InputBuffer.h"
#include"InputManager.h"
#include"IInputSource.h"
namespace
{
	//押されていない状態の数値
	constexpr int kNone = -1;
	//履歴を保存する最大値
	constexpr int kHistorySize = 30;
}


InputBuffer::InputBuffer()
{
	AllClear();
}

void InputBuffer::Update(const IInputSource& input)
{
	//すべてのボタン分繰り返す
	for (int i = 0; i < m_histories.size(); i++)
	{
		const Button button = static_cast<Button>(i);
		//ボタンが押されたら
		if (input.IsTriggerd(button))
		{
			//0フレームから数える
			m_histories[i] = 0;
			continue;
		}
		//押されていないなら何もしない
		if (m_histories[i] == kNone)
		{
			continue;
		}
		//保存しておくフレームが過ぎたらクリア
		if (++m_histories[i] > kHistorySize)
		{
			m_histories[i] = kNone;
		}
	}
}

void InputBuffer::AllClear()
{
	m_histories.fill(kNone);
}

bool InputBuffer::IsBuffered(Button button, int frame)
{
	//入力がないならfalse
	if (m_histories[static_cast<int>(button)] == kNone)
	{
		return false;
	}
	//フレーム以内に押されていたらtrue
	if (m_histories[static_cast<int>(button)] <= frame)
	{
		//クリアしておく
		Clear(button);
		return true;
	}
	return false;
}

void InputBuffer::Clear(Button button)
{
	m_histories[static_cast<int>(button)] = kNone;
}
