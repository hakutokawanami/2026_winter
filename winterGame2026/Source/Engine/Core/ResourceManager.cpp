#include "ResourceManager.h"
#include<DxLib.h>

namespace
{
    constexpr const wchar_t* kPlayer1ModelPath = L"Data/Model/Player/hakuto.mv1";
 //   constexpr const wchar_t* kPlayer1ModelPath = L"Data/Model/Player/Player_1.mv1";
    constexpr const wchar_t* kPlayer2ModelPath = L"Data/Model/Player/Player_2.mv1";
}


ResourceManager::ResourceManager() :
    m_player1ModelHandle(-1),
    m_player2ModelHandle(-1)
{
}


ResourceManager::~ResourceManager()
{
}

ResourceManager& ResourceManager::GetInstance()
{
    static ResourceManager instance;
    return instance;
}

void ResourceManager::LoadResources()
{
    MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_DISABLE);

    //プレイヤー
    m_player1ModelHandle = MV1LoadModel(kPlayer1ModelPath);
    m_modelHandleTable[ModelType::player1] = { m_player1ModelHandle };
    m_player2ModelHandle = MV1LoadModel(kPlayer2ModelPath);
    m_modelHandleTable[ModelType::player2] = { m_player2ModelHandle };
}

void ResourceManager::ReleaseResources()
{
    //モデル全部解放
    for (const auto all : m_modelHandleTable)
    {
        MV1DeleteModel(all.second);
    }

    //画像全部開放
    for (const auto all : m_graphHandleTable)
    {
        DeleteGraph(all.second);
    }
}

int ResourceManager::GetModel(ModelType type) const
{
    //モデル取得
    for (const auto all : m_modelHandleTable)
    {
        if (all.first == type)
        {
            return all.second;
        }
    }
    return -1;
}

int ResourceManager::GetGraph(GraphType type) const
{
    //画像グラフ
    for (const auto all : m_graphHandleTable)
    {
        if (all.first == type)
        {
            return all.second;
        }
    }
    return -1;
}

const wchar_t* ResourceManager::GetEffectPath(EffectType type) const
{
    //エフェクト
    for (const auto all : m_effectHandleTable)
    {
        if (all.first == type)
        {
            return all.second;
        }
    }
    return nullptr;
}
