#include "DxLib.h"
#include"Application.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//アプリケーションのインスタンス作成
	auto& app = Application::GetInstance();

	//初期化処理
	if (!app.Init())
	{
		return -1;
	}

	//ゲームループ
	app.Run();

	//後処理
	app.Terminate();

	// ソフトの終了 
	return 0;
}