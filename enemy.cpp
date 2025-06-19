//===================================================
//
// 敵 [enemy.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "enemy.h"
#include "player.h"
#include"manager.h"
#include"renderer.h"
#include "math.h"

//***************************************************
// マクロ定義
//***************************************************

using namespace math;

//===================================================
// コンストラクタ
//===================================================
CEnemy::CEnemy(int nPriority) : CCharacter3D(nPriority)
{
	memset(m_apModel, NULL, sizeof(m_apModel));
	m_fSpeed = NULL;
	m_move = VEC3_NULL;
	m_nNumModel = NULL;
	m_pMotion = nullptr;
	m_pAI = nullptr;
}

//===================================================
// デストラクタ
//===================================================
CEnemy::~CEnemy()
{
}

//===================================================
// 生成処理
//===================================================
CEnemy* CEnemy::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CEnemy* pEnemy = nullptr;

	// 敵オブジェクトの生成
	pEnemy = new CEnemy;

	// 優先順位の取得
	int nPriority = pEnemy->GetPriority();

	// 現在のオブジェクトの最大数
	const int nNumAll = CObject::GetNumObject(nPriority);

	// オブジェクトが最大数まであったら
	if (nNumAll >= MAX_OBJECT && pEnemy != nullptr)
	{
		// 自分のポインタの解放
		pEnemy->Release();

		// nullにする
		pEnemy = nullptr;

		// オブジェクトを消す
		return nullptr;
	}

	if (pEnemy == nullptr) return nullptr;

	pEnemy->SetPosition(pos);
	pEnemy->SetRotation(rot);
	pEnemy->Init();

	return pEnemy;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CEnemy::Init(void)
{
	m_pMotion = CMotion::Load("data/MOTION/motionEnemy000.txt", &m_apModel[0], ENEMY_MAX_PARTS, &m_nNumModel, MOTIONTYPE_MAX);

	// 敵のAIの生成
	m_pAI = new CEnemyAI;

	// 敵のAI初期化処理
	m_pAI->Init(m_pMotion);

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CEnemy::Uninit(void)
{
	for (int nCnt = 0; nCnt < ENEMY_MAX_PARTS; nCnt++)
	{
		// モデルの破棄
		if (m_apModel[nCnt] != nullptr)
		{
			// 終了処理
			m_apModel[nCnt]->Uninit();

			delete m_apModel[nCnt];

			m_apModel[nCnt] = nullptr;
		}
	}

	// モーションの終了処理
	m_pMotion->Uninit();

	// モーションの破棄
	if (m_pMotion != nullptr)
	{
		delete m_pMotion;

		m_pMotion = nullptr;
	}

	if (m_pAI != nullptr)
	{
		delete m_pAI;
		m_pAI = nullptr;
	}
	CCharacter3D::Uninit();
}

//===================================================
// 更新処理
//===================================================
void CEnemy::Update(void)
{
	CPlayer* pPlayer = CManager::GetPlayer();

	D3DXVECTOR3 pos = GetPosition();

	D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();

	// 移動量の減衰
	m_move.x += (0.0f - m_move.x) * 0.25f;
	m_move.z += (0.0f - m_move.z) * 0.25f;

	pos += m_move;

	// メッシュフィールドの取得
	CMeshField* pMesh = CManager::GetMeshField();

	pMesh->Collision(&pos);
	
	m_move.y += -MAX_GLABITY;

	if (m_pMotion != nullptr)
	{
		float fAngle = GetTargetAngle(PlayerPos, pos);		  // ターゲットまでの角度の取得

		// 待機じゃなかったら
		if (m_pAI != nullptr && m_pAI->Wait() == false)
		{
			// 一定の距離にいたら
			if (m_pAI->CheckDistance(PlayerPos, pos, 1000.0f) == MOTIONTYPE_MOVE)
			{
				m_pMotion->SetMotion(MOTIONTYPE_MOVE, true, 10);
			}
			// いなかったら
			else if (m_pAI->CheckDistance(PlayerPos, pos, 1000.0f) == MOTIONTYPE_NEUTRAL)
			{
				m_pMotion->SetMotion(MOTIONTYPE_NEUTRAL, true, 10);
			}

			// 攻撃可能だったら
			if (m_pAI->IsAttack() == MOTIONTYPE_ACTION)
			{
				m_pMotion->SetMotion(MOTIONTYPE_ACTION, true, 10);

				// 目的の角度の設定
				SetRotDest(D3DXVECTOR3(0.0f, fAngle + D3DX_PI, 0.0f));
			}
		}

		// AIの更新処理
		if (m_pAI != nullptr)
		{
			m_pAI->Update();
		}

		// モーションの更新処理
		m_pMotion->Update(&m_apModel[0], m_nNumModel);
	}

	// モーションの遷移
	TransitionMotion();

	SetPosition(pos);

	SmoothAngle(0.1f);
}

//===================================================
// 描画処理
//===================================================
void CEnemy::Draw(void)
{
	// キャラクターの描画
	CCharacter3D::Draw();

	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		if (m_apModel[nCnt] != nullptr)
		{
			// 描画処理
			m_apModel[nCnt]->Draw();

			// モデルの影の描画処理
			m_apModel[nCnt]->DrawShadow();
		}
	}
}

//===================================================
// 敵の移動処理
//===================================================
void CEnemy::UpdateMoveMotion(void)
{
	CPlayer* pPlayer = CManager::GetPlayer();

	D3DXVECTOR3 pos = GetPosition();

	D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();

	D3DXVECTOR3 TargetDir = GetTargetDir(PlayerPos, pos); // ターゲットまでの方向を取得
	float fAngle = GetTargetAngle(PlayerPos, pos);		  // ターゲットまでの角度の取得

	// 移動量の設定
	m_move.x = TargetDir.x * 10.0f;
	m_move.z = TargetDir.z * 10.0f;

	// 目的の角度の設定
	SetRotDest(D3DXVECTOR3(0.0f, fAngle + D3DX_PI, 0.0f));
}

//===================================================
// モーションの遷移
//===================================================
void CEnemy::TransitionMotion(void)
{
	// モーションの種類の取得
	MOTIONTYPE type = (MOTIONTYPE)m_pMotion->GetBlendMotionType();

	switch (type)
	{
	case MOTIONTYPE_NEUTRAL:
		break;
	case MOTIONTYPE_MOVE:

		// 敵の移動処理
		UpdateMoveMotion();

		break;
	case MOTIONTYPE_ACTION:
		break;
	default:
		break;
	}
}

//===================================================
// 敵のAIのコンストラクタ
//===================================================
CEnemyAI::CEnemyAI()
{
	m_pMotion = nullptr;
	m_Action = ACTION_IDLE;
	m_nCounterAction = NULL;
}

//===================================================
// 敵のAIのデストラクタ
//===================================================
CEnemyAI::~CEnemyAI()
{

}

//===================================================
// 敵のAIの初期化処理
//===================================================
void CEnemyAI::Init(CMotion* pMotion, const int nBlendFrame, const int nFirstMotion)
{
	// モーションの情報の取得
	m_pMotion = pMotion;

	m_pMotion->SetMotion(nFirstMotion, true, nBlendFrame);
}

//===================================================
// 更新処理
//===================================================
void CEnemyAI::Update(void)
{
	switch (m_Action)
	{
	case ACTION_WAIT:
		m_nCounterAction--;

		if (m_nCounterAction <= 0)
		{
			m_Action = ACTION_IDLE;
		}
		break;
	default:
		break;
	}
}

//===================================================
// 敵の移動のAI
//===================================================
int CEnemyAI::CheckDistance(const D3DXVECTOR3 dest, const D3DXVECTOR3 pos,const float fRadius)
{
	if (m_Action != ACTION_IDLE && m_Action != ACTION_MOVE) return -1;

	// 差分を求める
	D3DXVECTOR3 Diff = dest - pos;

	// 距離を求める
	float fDistance = GetDistance(Diff);

	if (fDistance >= fRadius)
	{
		m_Action = ACTION_MOVE;

		return CEnemy::MOTIONTYPE_MOVE;
	}

	m_Action = ACTION_IDLE;

	return CEnemy::MOTIONTYPE_NEUTRAL;
}

//===================================================
// 攻撃開始判定
//===================================================
int CEnemyAI::IsAttack(void)
{
	if (m_Action != ACTION_IDLE) return -1;

	int random = rand() % 100;

	m_Action = ACTION_ATTACK;

	return CEnemy::MOTIONTYPE_ACTION;
}

//===================================================
// 待機状態
//===================================================
bool CEnemyAI::Wait(void)
{
	if (m_pMotion->IsFinishEndBlend() == true)
	{
		m_Action = ACTION_WAIT;

		m_pMotion->SetMotion(CEnemy::MOTIONTYPE_NEUTRAL, true, 10);

		m_nCounterAction = 60;

		return true;
	}

	return false;
}
