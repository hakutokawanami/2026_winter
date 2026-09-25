#include "SceneController.h"
#include"SceneBase.h"

SceneController::SceneController()
{
}

void SceneController::ChangeScene(std::shared_ptr<SceneBase> newScene)
{
	//最初にemptyかどうかのチェック
	if (m_scenes.empty())
	{
		//最初は要素がないからPush_backする
		m_scenes.push_back(newScene);
	}
	//シーンの切り替え
	//元あったシーンは自動的に削除される
	m_scenes.back() = newScene;
}

void SceneController::PushScene(std::shared_ptr<SceneBase> newScene)
{
	//新しいシーンを末尾に積む
	m_scenes.push_back(newScene);
}

void SceneController::PopScene()
{
	// 末尾のシーンを削除（空の場合は何もしない）
	if (!m_scenes.empty())
	{
		m_scenes.pop_back();
	}
}

void SceneController::Update()
{
	//末尾のシーンのみ、Updateを行う
	m_scenes.back()->Update();
}

void SceneController::Draw()
{
	//Drawはすべて行う
	for (auto& scene : m_scenes)
	{
		scene->Draw();
	}
}
