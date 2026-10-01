#include "AnimationController.h"
#include<DxLib.h>

AnimationController::AnimationController(int modelHandle) :
	m_modelHandle(modelHandle),
	m_currentAnimNo(-1),
	m_currentAttachNo(-1),
	m_currentTime(0.0f),
	m_isLoop(true),
	m_isEnd(false),
	m_prevAnimNo(-1),
	m_prevAttachNo(-1),
	m_prevTime(0.0f),
	m_blendTime(0.0f),
	m_blendTimer(0.0f)
{
}

void AnimationController::Update()
{
	//アニメーションがなければ何もしない
	if (m_currentAnimNo < 0)
	{
		return;
	}
	//アニメーション時間の更新
	m_currentTime += 0.5f;

	//アニメーション全体の時間を取得
	float totalTime = MV1GetAnimTotalTime(m_modelHandle, m_currentAnimNo);

	if (m_isLoop)//ループする場合
	{
		//経過時間がアニメーションの時間を超えたら
		//アニメーションの時間分戻す
		while (m_currentTime > totalTime)
			m_currentTime -= totalTime;
		m_isEnd = false;
	}
	else// ループしない場合
	{
		//経過時間がアニメーションの時間を超えたら
		if (m_currentTime > totalTime)
		{
			//アニメーションを止める
			m_currentTime = totalTime;
			m_isEnd = true;
		}
		else
		{
			m_isEnd = false;
		}
	}

	// アニメーションをモデルに反映
	MV1SetAttachAnimTime(m_modelHandle, m_currentAttachNo, m_currentTime);

	//ブレンド処理
	if (m_prevAnimNo >= 0)
	{
		//前のアニメーションの時間更新
		m_prevTime += 0.5f;
		MV1SetAttachAnimTime(m_modelHandle, m_prevAttachNo, m_prevTime);
		//ブレンド時間の更新
		m_blendTimer += 1.0f;
		//ブレンドレート
		float rate = 1.0f;

		//ブレンドレートの計算
		if (m_blendTime > 0.0f)
		{
			rate = m_blendTimer / m_blendTime;
		}

		//ブレンドレートが1.0を超えないようにする
		if (rate > 1.0f)
		{
			rate = 1.0f;
		}
		//新しいアニメーションを補正
		MV1SetAttachAnimBlendRate(m_modelHandle, m_currentAttachNo, rate);

		//前のアニメーションを補正
		MV1SetAttachAnimBlendRate(m_modelHandle, m_prevAttachNo, 1.0f - rate);

		//ブレンドの終了
		if (rate >= 1.0f)
		{
			//前のアニメーションを削除
			MV1DetachAnim(m_modelHandle, m_prevAttachNo);
			m_prevAnimNo = -1;
		}
	}
	else
	{
		// ブレンドしてないときは100%
		MV1SetAttachAnimBlendRate(m_modelHandle, m_currentAttachNo, 1.0f);
	}
}

void AnimationController::Play(const std::wstring& name, bool loop, float blendTime)
{
	//アニメーション名で探す
	auto it = m_animTable.find(name);
	//なければ終了
	if (it == m_animTable.end())
	{
		return;
	}
	//アニメーション番号をセット
	int newAnim = it->second;
	//同じアニメーションなら再生しない
	if (newAnim == m_currentAnimNo)
	{
		return;
	}

	//// ブレンド中
	//if (m_prevAnimNo >= 0)
	//{
	//	// currentだけ差し替える
	//	MV1DetachAnim(m_modelHandle, m_currentAttachNo);
	//	m_currentAnimNo = newAnim;
	//	m_currentTime = 0.0f;
	//	m_isLoop = loop;

	//	m_currentAttachNo = MV1AttachAnim(m_modelHandle, m_currentAttachNo);
	//	m_blendTime = blendTime;
	//	m_blendTimer = 0.0f;
	//	return;
	//}

	//現在のアニメーションを前のアニメーションとして保存
	MV1DetachAnim(m_modelHandle, m_prevAttachNo);
	m_prevAnimNo = m_currentAnimNo;
	m_prevAttachNo = m_currentAttachNo;
	m_prevTime = m_currentTime;

	//新しいアニメーションを設定
	m_currentAnimNo = newAnim;
	m_currentTime = 0.0f;
	m_isLoop = loop;
	//新しいアニメーションのアタッチ番号を取得
	m_currentAttachNo = MV1AttachAnim(m_modelHandle, m_currentAnimNo);
	//ブレンド時間のセット
	m_blendTime = blendTime;
	m_blendTimer = 0.0f;
}

void AnimationController::AddAnimation(const std::wstring& name)
{
	//アニメーション名とアニメーション番号を登録
	m_animTable[name] = MV1GetAnimIndex(m_modelHandle, name.c_str());
}
