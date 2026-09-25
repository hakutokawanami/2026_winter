#pragma once
#include"Engine/Math/Vector2.h"

class Application
{
private:
	//ウィンドウサイズを入れる変数
	Size m_windowSize;

	//newも変数宣言もできなくする
	Application();
	//コピーコンストラクタも無効にする
	Application(const Application& app) = delete;
	//代入も無効にする
	void operator = (const Application& app) = delete;
public:
	~Application();
	/// <summary>
	/// アプリケーションのシングルトンインスタンスを取得
	/// </summary>
	/// <returns>Application</returns>
	static Application& GetInstance();
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <returns>初期化成功:true / 初期化失敗 : false</returns>
	bool Init();
	/// <summary>
	/// アプリケーションをスタートする
	/// ゲームループを内蔵
	/// </summary>
	void Run();
	/// <summary>
	/// アプリケーション終了処理
	/// </summary>
	void Terminate();
	/// <summary>
	/// ウィンドウのサイズを取得する関数
	/// </summary>
	/// <returns>ウィンドウサイズ</returns>
	const Size& GetWindowSize()const;
};