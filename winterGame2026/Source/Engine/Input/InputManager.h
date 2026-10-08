#pragma once
#include"Engine/Math/Vector2.h"
#include"Game/Common/PlayerID.h"
#include"StickDirection.h"
#include"StickInputAnalyzer.h"
#include<map>
#include<string>
#include<vector>
#include<DxLib.h>
#include<array>
#include<deque>
/// <summary>
/// 周辺機器種別
/// </summary>
enum class PeripheralType
{
	keyBoard,
	pad
};

/// <summary>
/// 入力対応情報
/// </summary>
struct  InputState
{
	//入力された機器の種別
	PeripheralType type;
	//入力情報が入る(キーボードならインデックス,パッドならビット)
	int id;
};

/// <summary>
/// 入力を抽象化するためのクラス
/// </summary>
class InputManager
{
private:
	InputManager();
	// コピー禁止
	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;
public:
	/// <summary>
	/// インスタンスを取得する関数
	/// </summary>
	/// <returns></returns>
	static InputManager& GetInstance();
	/// <summary>
	/// 入力情報の更新
	/// </summary>
	void Update();

	/// <summary>
	/// スティックの入力値を-1から1に変換する関数
	/// </summary>
	/// <param name="id">何Pか</param>
	void ConvertLeftStick(int id);

	/// <summary>
	/// スティックの入力を八方向+ニュートラルに変換する関数
	/// </summary>
	/// <param name="id">何Pか</param>
	void ConvertStickDirection(int id);

	/// <summary>
	/// 特定のボタンが押されているか
	/// </summary>
	/// <param name="player">何Pか</param>
	/// <param name="name">イベント名</param>
	/// <returns>押されている:true / 押されていない:false</returns>
	bool IsPressed(PlayerID player,const char* name)const;
	/// <summary>
	/// 特定のボタンが今押されたか
	/// </summary>
	/// <param name="player">何Pか</param>
	/// <param name="name">イベント名</param>
	/// <returns>今押された:true / 押されていないor押しっぱなし:false</returns>
	bool IsTriggered(PlayerID player, const char* name)const;
	/// <summary>
	/// 特定のボタンが今離されたか
	/// </summary>
	/// <param name="player">何Pか</param>
	/// <param name="name">イベント名</param>
	/// <returns>今離された:true / 押されていないor押しっぱなし:false</returns>
	bool IsReleased(PlayerID player, const char* name)const;
	/// <summary>
	/// 左スティックの入力を取得する関数
	/// </summary>
	/// <param name="player">何Pか</param>
	/// <returns>入力の方向ベクトル</returns>
	Vector2 GetLeftStick(PlayerID player)const;
	/// <summary>
	/// 右スティックの入力を取得する関数
	/// </summary>
	/// <param name="player">何Pか</param>
	/// <returns>入力の方向ベクトル</returns>
	Vector2 GetRightStick(PlayerID player)const;
	/// <summary>
	/// InputManagerが管理しているすべての入力イベントのうち、どれか一つでも入力されているか
	/// </summary>
	/// <param name="player">何Pか</param>
	/// <returns>どれか一つでも入力されている:true / すべて入力されていない:false</returns>
	bool AnyPressButtonInput(PlayerID player)const;
	/// <summary>
	/// どのゲームパッドを使うか
	/// </summary>
	/// <param name="player">何Pか</param>
	/// <returns></returns>
	int GetPadNo(PlayerID player)const;

	/// <summary>
	/// スティックの方向を取得
	/// </summary>
	/// <param name="player">何Pか</param>
	/// <returns></returns>
	StickDirection GetStickDirection(PlayerID player)const;
	/// <summary>
	/// 一瞬倒したかどうか
	/// </summary>
	/// <param name="player"></param>
	/// <param name="direction"></param>
	/// <returns></returns>
	bool IsStickQuickInput(PlayerID player, StickDirection direction);

	/// <summary>
	/// 二回連続で倒したかどうか
	/// </summary>
	/// <param name="player"></param>
	/// <param name="direction"></param>
	/// <returns></returns>
	bool IsStickDoubleTapInput(PlayerID player, StickDirection direction);

	/// <summary>
	/// スティックの入力履歴をデバッグ表示する
	/// </summary>
	/// <param name="player">何Pか</param>
	/// <param name="x">表示位置X</param>
	/// <param name="y">表示位置Y</param>
	void DebugDrawStickHistory(PlayerID player, int x, int y)const;

private:
	//イベント名と実際の入力の対応表
	std::map<std::string, std::vector<InputState>>m_inputTable;
	//実際に入力されたかどうかのデータ
	std::array<std::map<std::string, bool>,static_cast<int>(PlayerID::PlayerCount)>m_inputData;
	//前のフレームに押されたかどうか
	std::array<std::map<std::string, bool>, static_cast<int>(PlayerID::PlayerCount)>m_lastInputData;
	//スティックをどの方向に倒したかの情報
	std::array<StickDirection, static_cast<int>(PlayerID::PlayerCount)>m_stickDirection;
	//左スティックの入力を保存
	std::array<Vector2, static_cast<int>(PlayerID::PlayerCount)>m_leftStick;
	//スティック入力履歴
	std::array<StickInputAnalyzer, static_cast<int>(PlayerID::PlayerCount)>m_stickAnalyzer;

	//GetJoypadXInputStateで取得した入力を保存する変数
	std::array<XINPUT_STATE,2> m_stick;
};
