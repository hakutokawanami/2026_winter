#pragma once
#include"Game/Common/PlayerID.h"

class Player;
/// <summary>
/// ラウンド情報などを管理するクラス
/// </summary>
class RoundManager
{
public:
	/// <summary>
	/// フェーズ
	/// </summary>
	enum class Phase
	{
		roundStart,
		fight,
		ko,
		timeOver,
		roundEnd,
		MatchEnd
	};
	
	RoundManager();
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="player1"></param>
	/// <param name="player2"></param>
	void Update(const Player& player1, const Player& player2);

	/// <summary>
	/// 現在のフェーズを取得する
	/// </summary>
	/// <returns></returns>
	Phase GetPhase()const { return m_phase; }
	/// <summary>
	/// 今操作してもいいかどうか取得する
	/// </summary>
	/// <returns></returns>
	bool CanControl()const { return m_phase == Phase::fight; }
	/// <summary>
	/// 現在のラウンド数取得
	/// </summary>
	/// <returns></returns>
	int GetRound()const { return m_round; }
	/// <summary>
	/// フェーズを切り替える
	/// </summary>
	/// <param name="next"></param>
	void ChangePhase(Phase next) { m_phase = next; m_frame = 0; }
	/// <summary>
	/// リセットのリクエストが来ているかどうか
	/// </summary>
	/// <returns></returns>
	bool IsResetRequest();
	/// <summary>
	/// マッチを終了するかどうか
	/// </summary>
	/// <returns></returns>
	bool IsMatchEnd();
	/// <summary>
	/// タイマーの秒数を取得する
	/// </summary>
	/// <returns></returns>
	int GetTimer()const { return m_roundTimer; }
	/// <summary>
	/// 勝利ラウンド数を取得する
	/// </summary>
	/// <param name="id">プレイヤー</param>
	/// <returns></returns>
	int GetWins(PlayerID id)const { return m_wins[static_cast<int>(id)]; }
	/// <summary>
	/// 今のフェーズに入ってからのフレーム数を取得する
	/// </summary>
	/// <returns></returns>
	int GetPhaseFrame()const { return m_frame; }

	//勝利に必要なラウンド数
	static constexpr int kWinRounds = 2;

private:
	//現在のフェーズ
	Phase m_phase;
	//現在のラウンド
	int m_round;
	//勝利数
	int m_wins[2];
	//タイマー
	int m_roundTimer;
	//フレームカウンター
	int m_frame;
	//プレイヤーなどのリセットをしてもらうリクエスト
	bool m_resetRequest;
	//マッチを終了するか
	bool m_isMatchEnd;
};

