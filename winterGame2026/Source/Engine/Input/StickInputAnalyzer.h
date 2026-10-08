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
public:
	/// <summary>
	/// 毎フレーム呼び出す
	/// </summary>
	/// <param name="direction"></param>
	void Update(StickDirection direction);
	/// <summary>
	/// 履歴クリア
	/// </summary>
	void Clear();
	/// <summary>
	/// 一瞬だけ倒したかどうか
	/// </summary>
	/// <param name="direction"></param>
	/// <returns></returns>
	bool IsQuickInput(StickDirection direction)const;
	/// <summary>
	/// 二回連続で倒したかどうか
	/// </summary>
	/// <param name="direction"></param>
	/// <returns></returns>
	bool IsDoubleTap(StickDirection direction)const;
	/// <summary>
	/// 入力履歴をデバッグ表示する(新しい順)
	/// </summary>
	/// <param name="x">表示位置X</param>
	/// <param name="y">表示位置Y</param>
	void DebugDraw(int x, int y)const;

private:
	/// <summary>
	/// 履歴に入力を追加
	/// </summary>
	/// <param name="direction"></param>
	void AddRecord(StickDirection direction);
	/// <summary>
	/// 古い履歴を削除
	/// </summary>
	void RemoveOldRecords();

	//現在の入力方向
	StickDirection m_currentDirection = StickDirection::neutral;
	//入力履歴
	std::deque<StickInputRecord>m_history;
	//現在のフレーム数
	int m_currentFrame = 0;
};

