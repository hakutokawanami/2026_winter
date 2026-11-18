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
	m_blendTimer(0.0f),
	m_speed(0.5f),
	m_prevSpeed(0.5f),
	m_rootFrame(-1),
	m_rootStartPos({}),
	m_isRootLocked(false),
	m_prevRootStartPos({}),
	m_isPrevRootLocked(false),
	m_currentBlendRate(1.0f),
	m_rootOffset({})
{
	//腰のフレーム番号を取得する
	m_rootFrame = MV1SearchFrame(m_modelHandle, L"mixamorig:Hips");
}

void AnimationController::Update()
{
	//アニメーションがなければ何もしない
	if (m_currentAnimNo < 0)
	{
		return;
	}
	//アニメーション時間の更新
	m_currentTime += m_speed;

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
		m_prevTime += m_prevSpeed;
		MV1SetAttachAnimTime(m_modelHandle, m_prevAttachNo, m_prevTime);
		//ブレンド時間の更新
		m_blendTimer += 1.0f;
		//ブレンドレート
		m_currentBlendRate = 1.0f;

		//ブレンドレートの計算
		if (m_blendTime > 0.0f)
		{
			m_currentBlendRate = m_blendTimer / m_blendTime;
		}

		//ブレンドレートが1.0を超えないようにする
		if (m_currentBlendRate > 1.0f)
		{
			m_currentBlendRate = 1.0f;
		}
		//新しいアニメーションを補正
		MV1SetAttachAnimBlendRate(m_modelHandle, m_currentAttachNo, m_currentBlendRate);

		//前のアニメーションを補正
		MV1SetAttachAnimBlendRate(m_modelHandle, m_prevAttachNo, 1.0f - m_currentBlendRate);

		//ブレンドの終了
		if (m_currentBlendRate >= 1.0f)
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

	//アニメーションの打ち消す移動量を計算
	CalculateRootOffset();
}

void AnimationController::Play(const std::wstring& name, bool loop, float blendTime,float speed,bool replay)
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
	if (newAnim == m_currentAnimNo && !replay)
	{
		return;
	}

	//現在のアニメーションを前のアニメーションとして保存
	MV1DetachAnim(m_modelHandle, m_prevAttachNo);
	m_prevAnimNo = m_currentAnimNo;
	m_prevAttachNo = m_currentAttachNo;
	m_prevTime = m_currentTime;
	m_prevSpeed = m_speed;
	m_prevRootStartPos = m_rootStartPos;
	m_isPrevRootLocked = m_isRootLocked;
	//新しいアニメーションを設定
	m_currentAnimNo = newAnim;
	m_currentTime = 0.0f;
	m_isLoop = loop;
	m_speed = speed;
	//新しいアニメーションのアタッチ番号を取得
	m_currentAttachNo = MV1AttachAnim(m_modelHandle, m_currentAnimNo);
	//アニメーションの時間を0にする
	MV1SetAttachAnimTime(m_modelHandle, m_currentAttachNo, 0.0f);
	MV1SetAttachAnimBlendRate(m_modelHandle, m_currentAttachNo, 0.0f);
	//ブレンド時間のセット
	m_blendTime = blendTime;
	m_blendTimer = 0.0f;
	//腰の位置を保存
	m_rootStartPos = MV1GetAttachAnimFrameLocalPosition(m_modelHandle, m_currentAttachNo, m_rootFrame);
	m_isRootLocked = false;
	m_isEnd = false;
}

void AnimationController::SetTime(float time)
{
	if (m_currentAnimNo < 0)
	{
		return;
	}
	m_currentTime = time;
	MV1SetAttachAnimTime(m_modelHandle, m_currentAttachNo, m_currentTime);
}

float AnimationController::GetTotalTime() const
{
	if (m_currentAnimNo < 0)
	{
		return 0.0f;
	}
	return MV1GetAnimTotalTime(m_modelHandle, m_currentAnimNo);
}

void AnimationController::RootLocked()
{
	m_isRootLocked = true;
}

Vector3 AnimationController::GetRootOffset()
{
	//親のフレーム番号を取得
	int parentFrame = MV1GetFrameParent(m_modelHandle, m_rootFrame);

	MATRIX mat = {};
	//親のフレームがあるかないか
	if (parentFrame == -2)
	{
		//ワールド座標に変換
		mat = MV1GetLocalWorldMatrix(m_modelHandle);
	}
	else
	{
		//ワールド座標に変換
		mat = MV1GetFrameLocalWorldMatrix(m_modelHandle, parentFrame);
	}
	Vector3 world = {};
	world = VTransformSR(m_rootOffset, mat);
	//y成分を消す
	world.y = 0.0f;

	return world;
}

void AnimationController::CalculateRootOffset()
{
	m_rootOffset = {};

	if (m_rootFrame >= 0)
	{
		if (m_isRootLocked)
		{
			Vector3 currentPos = {};
			currentPos = MV1GetAttachAnimFrameLocalPosition(m_modelHandle, m_currentAttachNo, m_rootFrame);
			m_rootOffset += (currentPos - m_rootStartPos) * m_currentBlendRate;
		}

		if (m_prevAttachNo >= 0 && m_isPrevRootLocked)
		{
			Vector3 prevPos = {};
			prevPos = MV1GetAttachAnimFrameLocalPosition(m_modelHandle, m_prevAttachNo, m_rootFrame);

			m_rootOffset += (prevPos - m_prevRootStartPos) * (1.0f - m_currentBlendRate);
		}
	}
}

void AnimationController::AddAnimation(const std::wstring& name)
{
	//アニメーション名とアニメーション番号を登録
	m_animTable[name] = MV1GetAnimIndex(m_modelHandle, name.c_str());
}
