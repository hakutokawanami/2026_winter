#pragma once

/// <summary>
/// モード
/// </summary>
enum class GameMode
{
	vsCpu,
	vsPlayer,
	training
};

/// <summary>
/// キャラクター
/// </summary>
enum class CharacterID
{
	character1,
	character2
};

/// <summary>
/// 試合の設定
/// </summary>
struct MatchSetting
{
	GameMode gameMode = GameMode::vsCpu;
	CharacterID p1 = CharacterID::character1;
	CharacterID p2 = CharacterID::character2;
};