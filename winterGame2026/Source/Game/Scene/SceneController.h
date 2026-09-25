#pragma once
#include<memory>
#include<list>

enum scene
{
	title,
	game,
	result
};

class SceneBase;
class SceneController
{
public:
	SceneController();

	void ChangeScene(std::shared_ptr<SceneBase> newScene);

	void PushScene(std::shared_ptr<SceneBase> newScene);

	void PopScene();

	void Update();

	void Draw();

	bool IsGameEnd() const { return m_isGameEnd; }

	void EndGame() { m_isGameEnd = true; }

private:
	std::list<std::shared_ptr<SceneBase>> m_scenes;

	bool m_isGameEnd = false;
};
