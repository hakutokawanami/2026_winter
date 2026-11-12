#include "RoundManager.h"
#include "Game/Player/Player.h"
namespace
{
	//タイマーの初期値
	constexpr int kStartTime = 99;
	//ラウンド開始フェーズに使うフレーム数
	constexpr int kRoundStartFrame = 120;
	//KOフェーズに使うフレーム数
	constexpr int kKoFrame = 180;
	//タイムオーバーフェーズに使うフレーム数
	constexpr int kTimeOverFrame = 180;
}

RoundManager::RoundManager():
	m_phase(),
	m_round(),
	m_roundTimer(),
	m_frame(),
	m_wins(),
	m_resetRequest(false),
	m_isMatchEnd(false)
{
}

void RoundManager::Init()
{
	m_phase = Phase::roundStart;
	m_round = 1;
	m_roundTimer = kStartTime;
	m_frame = 0;
	m_wins[0] = 0;
	m_wins[1] = 0;
	m_resetRequest = false;
}

void RoundManager::Update(const Player& player1, const Player& player2)
{
	m_frame++;
	
	switch (m_phase)
	{
	case Phase::roundStart:
		//ラウンド開始の合図をする
		if (m_frame > kRoundStartFrame)
		{
			ChangePhase(Phase::fight);
		}
		break;

	case Phase::fight:
		//60フレームで一秒進める
		if (m_frame % 60 == 0)
		{
			m_roundTimer--;
			//タイマーが0以下になったら
			if (m_roundTimer <= 0)
			{
				//0で固定
				m_roundTimer = 0;
				//プレイヤー1のほうが体力が多いなら
				if (player1.GetHp() > player2.GetHp())
				{
					//プレイヤー1がラウンド勝利
					m_wins[0]++;
				}
				//プレイヤー2のほうが体力が多いなら
				else if (player1.GetHp() < player2.GetHp())
				{
					//プレイヤー2がラウンド勝利
					m_wins[1]++;
				}
				//どちらも同じ体力量なら
				else
				{
					//どちらもラウンド取得
					m_wins[0]++;
					m_wins[1]++;
				}
				ChangePhase(Phase::timeOver);
				break;
			}
		}
		//どちらかが死亡していたら
		if (player1.IsDead() || player2.IsDead())
		{
			//プレイヤー1が生きていたら
			if (!player1.IsDead())
			{
				//プレイヤー1がラウンド勝利
				m_wins[0]++;
			}
			//プレイヤー2が生きていたら
			else if (!player2.IsDead())
			{
				//プレイヤー2がラウンド勝利
				m_wins[1]++;
			}
			//同時に死亡していたら
			else
			{
				//どちらもラウンド取得
				m_wins[0]++;
				m_wins[1]++;
			}
			ChangePhase(Phase::ko);
		}
		break;

	case Phase::ko:
		//KO演出の時間
		if (m_frame > kKoFrame)
		{
			ChangePhase(Phase::roundEnd);
		}
		break;

	case Phase::timeOver:
		//タイムオーバー演出の時間
		if (m_frame > kTimeOverFrame)
		{
			ChangePhase(Phase::roundEnd);
		}
		break;

	case Phase::roundEnd:
		//どちらかが勝利ラウンド数を満たしていたら
		if (m_wins[0] >= kWinRounds || m_wins[1] >= kWinRounds)
		{
			//リザルト
			ChangePhase(Phase::MatchEnd);
		}
		//満たしていないなら
		else
		{
			//ラウンドを進める
			m_round++;
			//タイマー初期化
			m_roundTimer = kStartTime;
			//リクエスト
			m_resetRequest = true;
			ChangePhase(Phase::roundStart);
		}

		break;

	case Phase::MatchEnd:
		m_resetRequest = true;
		m_isMatchEnd = true;
		break;
	}
}

bool RoundManager::IsResetRequest()
{
	//trueを受け取ったらすぐfalseに戻す
	const bool request = m_resetRequest;
	m_resetRequest = false;
	return request;
}

bool RoundManager::IsMatchEnd()
{
	//trueを受け取ったらすぐfalseに戻す
	const bool request = m_isMatchEnd;
	m_isMatchEnd = false;
	return request;
}
