#include "GameObjectManager.h"
#include"Game/Base/GameObject.h"

GameObjectManager::GameObjectManager(CollisionManager& collisionManager) :
	m_collisionManager(collisionManager)
{
}
GameObjectManager::~GameObjectManager()
{
}
void GameObjectManager::Add(std::unique_ptr<GameObject> object)
{
	//追加
	m_objects.push_back(std::move(object));

	//追加したオブジェクトがIColliderを継承しているか確認
	GameObject* obj = m_objects.back().get();
	if (auto collider = dynamic_cast<ICollider*>(obj))
	{
		//コリジョンマネージャーに追加
		m_collisionManager.AddObject(*collider);
	}
}

void GameObjectManager::Init()
{
	//初期化
	for (auto& obj : m_objects)
	{
		obj->Init();
	}
}

void GameObjectManager::Update()
{
	//更新
	for (auto& obj : m_objects)
	{
		obj->Update();
	}

	//削除
	m_objects.erase(
		std::remove_if(m_objects.begin(), m_objects.end(),
			[this](const std::unique_ptr<GameObject>& obj)
			{
				if (!obj->IsPendingDestroy())
				{
					return false;
				}

				if (auto collider = dynamic_cast<ICollider*>(obj.get()))
				{
					m_collisionManager.RemoveObject(collider);
				}

				return true;

			}),
		m_objects.end()
	);
}

void GameObjectManager::Draw()
{
	//描画
	for (auto& obj : m_objects)
	{
		obj->Draw();
	}
}

void GameObjectManager::Clear()
{
	//解放
	m_objects.clear();
}
