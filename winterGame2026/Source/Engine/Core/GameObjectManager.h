#pragma once
#include"Engine/Math/Vector3.h"
#include<vector>
#include<memory>

class CollisionManager;
class GameObject;
class GameObjectManager
{
public:
    GameObjectManager(CollisionManager& collisionManager);
    ~GameObjectManager();

    /// <summary>
    /// オブジェクトを追加する
    /// </summary>
    /// <param name="object">追加するオブジェクト</param>
    void Add(std::unique_ptr<GameObject> object);
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

    //特定の型を探す
    template<typename T>
    T* Find();


private:
    //オブジェクト配列
    std::vector<std::unique_ptr<GameObject>> m_objects;

    //コリジョンマネージャー
    CollisionManager& m_collisionManager;
};

template<typename T>
inline T* GameObjectManager::Find()
{
    //object全体を探す
    for (auto& object : m_objects)
    {
        //dynamic_castで特定の型のオブジェクトを探す
        if (auto cast = dynamic_cast<T*>(object.get()))
        {
            //見つかったらそのオブジェクトのポインタを返す
            return cast;
        }
    }
    //なかったらぬるぽ
    return nullptr;
}
