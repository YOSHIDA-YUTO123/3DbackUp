//===================================================
//
// キャラクター3D [character.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "character3D.h"
#include "textureManager.h"
#include"manager.h"
#include"renderer.h"
#include"math.h"

//===================================================
// コンストラクタ
//===================================================
CCharacter3D::CCharacter3D(int nPriority) : CObject(nPriority)
{
	m_pos = VEC3_NULL;
	m_rot = VEC3_NULL;
	memset(m_mtxWorld, NULL, sizeof(m_mtxWorld));
	m_nLife = NULL;
	m_rotDest = VEC3_NULL;
}

//===================================================
// デストラクタ
//===================================================
CCharacter3D::~CCharacter3D()
{
}

//===================================================
// 初期化処理
//===================================================
HRESULT CCharacter3D::Init(void)
{
	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CCharacter3D::Uninit(void)
{
	// 自分自身の破棄
	Release();
}

//===================================================
// 更新処理
//===================================================
void CCharacter3D::Update(void)
{

}

//===================================================
// 描画処理
//===================================================
void CCharacter3D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//===================================================
// キャラクターの設定
//===================================================
void CCharacter3D::SetCharacter(const int nLife)
{
	m_nLife = nLife;
}

//===================================================
// キャラクターのヒット処理
//===================================================
bool CCharacter3D::HitCharacter(int nDamage)
{
	// 死んだかどうか
	bool bDeath = false;

	m_nLife -= nDamage;

	if (m_nLife <= 0)
	{
		Uninit();

		return bDeath;
	}
	else
	{

	}

	return bDeath;
}

//===================================================
// 目的の角度に近づける処理S
//===================================================
void CCharacter3D::SmoothAngle(const float fCoefficient)
{
	// 向きの差分
	float fDiffRot = m_rotDest.y - m_rot.y;

	// 角度の正規化
	NormalizeDiffRot(fDiffRot, &m_rot.y);

	// 向きを目的の向きに近づける
	m_rot = LerpDest(m_rotDest, m_rot, fCoefficient);
}

