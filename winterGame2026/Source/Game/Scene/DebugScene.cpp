#include "DebugScene.h"
#include "SceneController.h"
#include "Engine/Core/Application.h"
#include "DxLib.h"

#include "TitleScene.h"
#include "GameScene.h"
//#include "ResultScene.h"
//#include "CollisionTestScene.h"
//#include "AnimationTestScene.h"

namespace
{
	// デバッグシーンで選択できるシーン数
	constexpr int kSceneCount = 5;

	// メニューの表示位置
	constexpr int kStartX = 100;
	constexpr int kStartY = 150;
	constexpr int kLineHeight = 50;
}

DebugScene::DebugScene(SceneController& controller) :
	SceneBase(controller)
{
}

DebugScene::~DebugScene()
{
}

void DebugScene::Update()
{
	NormalUpdate();
}

void DebugScene::Draw()
{
	NormalDraw();
}

void DebugScene::NormalUpdate()
{
	// 1～5キーでシーンを選択
	if (CheckHitKey(KEY_INPUT_1))
	{
		m_selectScene = 0;
	}

	if (CheckHitKey(KEY_INPUT_2))
	{
		m_selectScene = 1;
	}

	if (CheckHitKey(KEY_INPUT_3))
	{
		m_selectScene = 2;
	}

	if (CheckHitKey(KEY_INPUT_4))
	{
		m_selectScene = 3;
	}

	if (CheckHitKey(KEY_INPUT_5))
	{
		m_selectScene = 4;
	}

	// Enterで決定
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		switch (m_selectScene)
		{
		case 0:
			m_controller.ChangeScene(
				std::make_shared<TitleScene>(m_controller)
			);
			break;

		case 1:
			m_controller.ChangeScene(
				std::make_shared<GameScene>(m_controller)
			);
			break;

		case 2:
			//m_controller.ChangeScene(
			//	std::make_shared<ResultScene>(m_controller)
			//);
			break;

		case 3:
			//m_controller.ChangeScene(
			//	std::make_shared<CollisionTestScene>(m_controller)
			//);
			break;

		case 4:
			//m_controller.ChangeScene(
			//	std::make_shared<AnimationTestScene>(m_controller)
			//);
			break;
		}
	}

	// ESCでゲーム終了
	if (CheckHitKey(KEY_INPUT_ESCAPE))
	{
		m_controller.EndGame();
	}
}

void DebugScene::NormalDraw()
{
	auto& wsize = Application::GetInstance().GetWindowSize();

#ifdef _DEBUG

	DrawFormatString(
		kStartX,
		50,
		0xffffff,
		L"===== DEBUG SCENE ====="
	);

	DrawFormatString(
		kStartX,
		90,
		0xaaaaaa,
		L"数字キーで選択/EnterKeyで決定"
	);

	// シーン一覧
	DrawFormatString(
		kStartX,
		kStartY,
		m_selectScene == 0 ? 0xffff00 : 0xffffff,
		L"[1] TitleScene"
	);

	DrawFormatString(
		kStartX,
		kStartY + kLineHeight,
		m_selectScene == 1 ? 0xffff00 : 0xffffff,
		L"[2] GameScene"
	);

	DrawFormatString(
		kStartX,
		kStartY + kLineHeight * 2,
		m_selectScene == 2 ? 0xffff00 : 0xffffff,
		L"[3] ResultScene"
	);

	DrawFormatString(
		kStartX,
		kStartY + kLineHeight * 3,
		m_selectScene == 3 ? 0xffff00 : 0xffffff,
		L"[4] CollisionTestScene"
	);

	DrawFormatString(
		kStartX,
		kStartY + kLineHeight * 4,
		m_selectScene == 4 ? 0xffff00 : 0xffffff,
		L"[5] AnimationTestScene"
	);

#endif // _DEBUG
}