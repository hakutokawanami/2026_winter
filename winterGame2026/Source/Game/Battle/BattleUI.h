#pragma once

class RoundManager;
class Player;
/// <summary>
/// バトル中のUIを描画するクラス
/// </summary>
class BattleUI
{
public:
    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="round">ラウンドマネージャー</param>
    /// <param name="p1">プレイヤー1</param>
    /// <param name="p2">プレイヤー2</param>
    void Draw(const RoundManager& round, const Player& p1, const Player& p2) const;
private:
    /// <summary>
    /// HPバーを描画する
    /// </summary>
    /// <param name="player">プレイヤー</param>
    void DrawHpBar(const Player& player) const;
    /// <summary>
    /// タイマーを描画する
    /// </summary>
    /// <param name="seconds">秒数</param>
    void DrawTimer(int seconds) const;
    /// <summary>
    /// 勝利ラウンドを描画する
    /// </summary>
    /// <param name="wins">勝利ラウンド数</param>
    /// <param name="player">プレイヤー</param>
    void DrawWins(int wins,const Player& player) const;
    /// <summary>
    /// 各フェーズの合図を描画する
    /// </summary>
    /// <param name="round">ラウンドマネージャー</param>
    void DrawPhaseText(const RoundManager& round) const;  // "ROUND 1" "FIGHT" "KO" など
};
