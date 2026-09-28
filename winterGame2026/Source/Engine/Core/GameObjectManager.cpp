#include "GameObjectManager.h"

#include "Game/Base/GameObject.h"

#include <algorithm>

void GameObjectManager::Add(
    std::shared_ptr<GameObject> object)
{
    m_objects.push_back(std::move(object));
}

void GameObjectManager::Init()
{
    for (auto& obj : m_objects)
    {
        obj->Init();
    }
}

void GameObjectManager::Update()
{
    // 更新
    for (auto& obj : m_objects)
    {
        if (!obj->IsActive())
        {
            continue;
        }

        obj->Update();
    }

    // 削除
    m_objects.erase(
        std::remove_if(
            m_objects.begin(),
            m_objects.end(),
            [](const std::shared_ptr<GameObject>& obj)
            {
                return obj->IsPendingDestroy();
            }),
        m_objects.end());
}

void GameObjectManager::Draw()
{
    for (auto& obj : m_objects)
    {
        if (!obj->IsActive())
        {
            continue;
        }

        obj->Draw();
    }
}

void GameObjectManager::Clear()
{
    m_objects.clear();
}