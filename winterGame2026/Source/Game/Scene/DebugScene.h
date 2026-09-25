#pragma once
#include "SceneBase.h"

class DebugScene : public SceneBase
{
public:
	DebugScene(SceneController& controller);
	~DebugScene();

	void Update() override;
	void Draw() override;

private:
	void NormalUpdate();
	void NormalDraw();

	// 選択中のシーン番号
	int m_selectScene = 0;
};