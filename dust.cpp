//===================================================
//
// 瓦礫 [dust.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "dust.h"
#include"objectX.h"
#include "meshfield.h"
#include "manager.h"
#include"math.h"

//===================================================
// コンストラクタ
//===================================================
CDust::CDust()
{
	m_nLife = NULL;
	m_pObjectX = nullptr;
	m_pos = VEC3_NULL;
	m_rot = VEC3_NULL;
	m_move = VEC3_NULL;
	m_nMaxLife = NULL;
	m_fDecAlv = NULL;
}

//===================================================
// デストラクタ
//===================================================
CDust::~CDust()
{
}

//===================================================
// 生成処理
//===================================================
CDust* CDust::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 dir, const int nLife)
{
	CDust* pDust = nullptr;

	// 瓦礫の生成
	pDust = new CDust;

	// 優先順位の取得
	int nPriority = pDust->GetPriority();

	// 現在のオブジェクトの最大数
	const int nNumAll = CObject::GetNumObject(nPriority);

	// オブジェクトが最大数まであったら
	if (nNumAll >= MAX_OBJECT && pDust != nullptr)
	{
		// 自分のポインタの解放
		pDust->Release();

		// nullにする
		pDust = nullptr;

		// オブジェクトを消す
		return nullptr;
	}

	if (pDust == nullptr) return nullptr;

	int random = rand() % TYPE::TYPE_MAX;

	switch (random)
	{
	case TYPE::TYPE_ONE:
		pDust->m_pObjectX = CObjectX::Create(pos, "data/MODEL/dust/dust000.x");
		break;
	case TYPE::TYPE_TWO:
		pDust->m_pObjectX = CObjectX::Create(pos, "data/MODEL/dust/dust001.x");
		break;
	case TYPE::TYPE_THREE:
		pDust->m_pObjectX = CObjectX::Create(pos, "data/MODEL/dust/dust002.x");
		break;
	default:
		break;
	}
	

	pDust->Init();
	pDust->m_nLife = nLife;
	pDust->m_nMaxLife = nLife;
	pDust->m_fDecAlv = 1.0f / nLife;

	pDust->m_move.x = dir.x;
	pDust->m_move.y = dir.y;
	pDust->m_move.z = dir.z;

	pDust->m_pos = pos;
	return pDust;
}

//===================================================
// 瓦礫の生成処理(円形)
//===================================================
void CDust::Creates(const int nNumDust,const D3DXVECTOR3 move, const D3DXVECTOR3 pos, const int nLife)
{
	for (int nCnt = 0; nCnt < nNumDust; nCnt++)
	{
		float fAngle = (D3DX_PI * 2.0f) / nNumDust * nCnt;

		float dirX = sinf(fAngle) * move.x;
		float dirZ = cosf(fAngle) * move.z;

		CDust::Create(pos, D3DXVECTOR3(dirX, move.y, dirZ), nLife);
	}
}

//===================================================
// 初期化処理
//===================================================
HRESULT CDust::Init(void)
{
	// 初期化処理
	if (FAILED(m_pObjectX->Init()))
	{
		return E_FAIL;
	}
	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CDust::Uninit(void)
{
	m_pObjectX = nullptr;

	Release();
}

//===================================================
// 更新処理
//===================================================
void CDust::Update(void)
{
	CMeshField* pMesh = CManager::GetMeshField();

	//// 更新処理
	//m_pObjectX->Update();
	m_pos += m_move;

	float fHeight = 0.0f;

	if (pMesh->Collision(m_pos, &fHeight))
	{
		m_pos.y = fHeight;

		m_move.y = -m_move.y * 0.8f;
	}

	D3DXVECTOR3 up = pMesh->GetNor(); // 上方向ベクトル

	D3DXVECTOR3 dir = m_move;

	D3DXVec3Normalize(&dir, &dir);

	D3DXVECTOR3 axis;
	D3DXVec3Cross(&axis, &up, &dir);

	float dot = D3DXVec3Dot(&up, &dir);
	float fAngle = acosf(dot);

	D3DXMATRIX mtxRot; // 計算用回転行列

	// 任意の軸から回転行列を求める
	D3DXMatrixRotationAxis(&mtxRot, &axis, fAngle);

	// 回転行列をオイラー角に変換
	D3DXVECTOR3 rot = math::MatrixToEulerXYZ(mtxRot);

	m_rot = rot;

	m_move.y += -MAX_GLABITY;

	if (m_pObjectX != nullptr)
	{
		// 位置の設定処理
		m_pObjectX->SetPosition(m_pos);

		// 向きの設定処理
		m_pObjectX->SetRotaition(m_rot);
	}

	m_nLife--;

	if (m_nLife <= 0)
	{
		m_pObjectX->Uninit();
		Uninit();
	}
}

//===================================================
// 描画処理
//===================================================
void CDust::Draw(void)
{
	if (m_pObjectX != nullptr)
	{
		// 描画処理
		m_pObjectX->Draw();
	}
}
