#pragma once
#include"StickDirection.h"
#include<deque>

//スティック入力一つの情報
struct StickInputRecord
{
	//方向
	StickDirection direction;
	//入力開始フレーム
	int startFrame;
	//入力終了フレーム
	int endFrame;
	/// <summary>
	/// 何フレーム入力されていたか
	/// </summary>
	/// <returns></returns>
	int GetDuration()const
	{
		return endFrame - startFrame + 1;
	}
};

class StickInputAnalyzer
{
};

