#pragma once

class SceneController;
/// <summary>
/// シーンの基底クラス
/// </summary>
class SceneBase
{
public:
	SceneBase(SceneController& controller);

	virtual void Update()abstract;

	virtual void Draw()abstract;

protected:
	SceneController& m_controller;
};

