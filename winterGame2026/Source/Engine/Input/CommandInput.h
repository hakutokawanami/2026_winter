#pragma once
#include"StickDirection.h"
#include<deque>
#include<string>
#include<string_view>

/// <summary>
/// コマンドを判定するクラス
/// </summary>
class CommandInput
{
public:
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="screenDir"></param>
	/// <param name="IsOpponentOnRight"></param>
	void Update(StickDirection screenDir, bool isOpponentOnRight);
	/// <summary>
	/// 履歴の削除
	/// </summary>
	void Clear() { m_history.clear(); }
	/// <summary>
	/// コマンドが最近入力されたかどうか
	/// </summary>
	/// <param name="command">テンキー表記のコマンド</param>
	/// <param name="maxFrame"></param>
	/// <returns></returns>
	bool IsInput(std::string_view command, int maxFrame)const;

	/// <summary>
	/// デバッグ表示用
	/// </summary>
	/// <returns></returns>
	std::string GetHistoryString()const { return { m_history.begin(),m_history.end() }; }
private:
	/// <summary>
	/// 相手基準のテンキーに変換する
	/// </summary>
	/// <param name="screenDir"></param>
	/// <param name="isOpponentRight"></param>
	/// <returns></returns>
	static char ToNumpad(StickDirection screenDir, bool isOpponentRight);

	std::deque<char>m_history;

};

