#pragma once
#include"Game/Common/PlayerID.h"
#include"Button.h"
#include<array>

/// <summary>
/// ボタンの先行入力を行うクラス
/// </summary>
class InputBuffer
{
public:
	InputBuffer();
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="id"></param>
	void Update(PlayerID id);
	/// <summary>
	/// すべての履歴をクリアする
	/// </summary>
	void AllClear();
	/// <summary>
	/// 指定のフレーム以内に押されているか
	/// </summary>
	/// <param name="button"></param>
	/// <param name="frame"></param>
	/// <returns></returns>
	bool IsBuffered(Button button, int frame);
	/// <summary>
	/// ボタンを使ったらそのボタンの履歴をクリアする
	/// </summary>
	/// <param name="button"></param>
	void Clear(Button button);

private:
	//何フレーム経過しているか
	std::array<int, static_cast<size_t>(Button::count)>m_histories;
};

