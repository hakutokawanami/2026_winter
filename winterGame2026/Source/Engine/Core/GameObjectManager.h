#pragma once

#include <vector>
#include <memory>

class GameObject;

class GameObjectManager
{
public:
    GameObjectManager() = default;
    ~GameObjectManager() = default;

    /// <summary>
    /// オブジェクトを追加する
    /// </summary>
    /// <param name="object">追加するオブジェクト</param>
    void Add(std::shared_ptr<GameObject> object);

    /// <summary>
    /// オブジェクトすべての初期化
    /// </summary>
    void Init();

    /// <summary>
    /// オブジェクトすべての更新
    /// </summary>
    void Update();

    /// <summary>
    /// オブジェクトすべての描画
    /// </summary>
    void Draw();

    /// <summary>
    /// オブジェクトすべての解放
    /// </summary>
    void Clear();

    /// <summary>
    /// 特定の型のオブジェクトを探す
    /// </summary>
    template<typename T>
    T* Find();

private:
    // オブジェクトの所有
    std::vector<std::shared_ptr<GameObject>> m_objects;
};

template<typename T>
inline T* GameObjectManager::Find()
{
    for (auto& object : m_objects)
    {
        if (auto cast = dynamic_cast<T*>(object.get()))
        {
            return cast;
        }
    }

    return nullptr;
}