#pragma once
#include"Engine/Math/Vector2.h"
#include<map>
#include<string>
#include<vector>
#include<DxLib.h>

/// <summary>
/// 周辺機器種別
/// </summary>
enum class PeripheralType
{
	keyBoard,
	pad1
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
	/// 特定のボタンが押されているか
	/// </summary>
	/// <param name="name">イベント名</param>
	/// <returns>押されている:true / 押されていない:false</returns>
	bool IsPressed(const char* name)const;
	/// <summary>
	/// 特定のボタンが今押されたか
	/// </summary>
	/// <param name="name">イベント名</param>
	/// <returns>今押された:true / 押されていないor押しっぱなし:false</returns>
	bool IsTriggered(const char* name)const;
	/// <summary>
	/// 特定のボタンが今離されたか
	/// </summary>
	/// <param name="name">イベント名</param>
	/// <returns>今離された:true / 押されていないor押しっぱなし:false</returns>
	bool IsReleased(const char* name)const;
	/// <summary>
	/// 左スティックの入力を取得する関数
	/// </summary>
	/// <returns>入力の方向ベクトル</returns>
	Vector2 GetLeftStick()const;
	/// <summary>
	/// 右スティックの入力を取得する関数
	/// </summary>
	/// <returns>入力の方向ベクトル</returns>
	Vector2 GetRightStick()const;
	/// <summary>
	/// InputManagerが管理しているすべての入力イベントのうち、どれか一つでも入力されているか
	/// </summary>
	/// <returns>どれか一つでも入力されている:true / すべて入力されていない:false</returns>
	bool AnyPressButtonInput()const;

private:
	//イベント名と実際の入力の対応表
	std::map<std::string, std::vector<InputState>>m_inputTable;
	//実際に入力されたかどうかのデータ
	std::map<std::string, bool>m_inputData;
	//前のフレームに押されたかどうか
	std::map<std::string, bool>m_lastInputData;

	//GetJoypadXInputStateで取得した入力を保存する変数
	XINPUT_STATE m_stick;
};
