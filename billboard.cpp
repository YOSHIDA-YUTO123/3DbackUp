//===================================================
//
// ビルボードの描画 [billboard.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "billboard.h"
#include"manager.h"
#include"renderer.h"

//===================================================
// コンストラクタ
//===================================================
CObjectBillboard::CObjectBillboard(int nPriority) : CObject(nPriority)
{
	memset(m_mtxWorld, NULL, sizeof(D3DXMATRIX));
	m_nTextureIdx = NULL;
	m_pos = VEC3_NULL;
	m_pVtxBuffer = nullptr;
	m_Size = VEC3_NULL;
}

//===================================================
// デストラクタ
//===================================================
CObjectBillboard::~CObjectBillboard()
{
}

//===================================================
// 初期化処理
//===================================================
HRESULT CObjectBillboard::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファの生成・頂点情報の設定
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffer,
		NULL)))
	{
		return E_FAIL;
	}


	return S_OK;

}

//===================================================
// 終了処理
//===================================================
void CObjectBillboard::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}

	// 自分自身の破棄
	Release();
}

//===================================================
// 更新処理
//===================================================
void CObjectBillboard::Update(void)
{
}

//===================================================
// 描画処理
//===================================================
void CObjectBillboard::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャクラスの取得
	CTextureManager* pTexture = CManager::GetTexture();

	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxRot, mtxTrans;

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zテスト
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// ビューマトリックスを宣言
	D3DXMATRIX mtxView;

	// ビューマトリックスの取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// カメラの逆行列を設定
	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	m_mtxWorld._21 = mtxView._12;
	m_mtxWorld._22 = mtxView._22;
	m_mtxWorld._23 = mtxView._32;
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetAdress(m_nTextureIdx));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//// Zテスト
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//===================================================
// 頂点のオフセットの設定処理
//===================================================
void CObjectBillboard::SetOffsetVtx(const D3DXCOLOR col, const int nPosX, const int nPosY)
{
	// 頂点情報のポインタ
	VERTEX_3D* pVtx;

	// 頂点バッファのロック
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_Size.x, m_Size.y, m_Size.z);
	pVtx[1].pos = D3DXVECTOR3(m_Size.x, m_Size.y, m_Size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_Size.x, -m_Size.y, -m_Size.z);
	pVtx[3].pos = D3DXVECTOR3(m_Size.x, -m_Size.y, -m_Size.z);

	// 法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// 頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f / nPosX, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f / nPosY);
	pVtx[3].tex = D3DXVECTOR2(1.0f / nPosX, 1.0f / nPosY);

	// 頂点バッファのアンロック
	m_pVtxBuffer->Unlock();
}

//===================================================
// 頂点の色の設定
//===================================================
void CObjectBillboard::SetColor(const D3DXCOLOR col)
{
	// 頂点情報のポインタ
	VERTEX_3D* pVtx;

	// 頂点バッファのロック
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// 頂点バッファのアンロック
	m_pVtxBuffer->Unlock();
}

//===================================================
// テクスチャのIDの設定
//===================================================
void CObjectBillboard::SetTextureID(const char* pTextureName)
{
	if (pTextureName == NULL)
	{
		m_nTextureIdx = -1;
		return;
	}
	// テクスチャクラスの取得
	CTextureManager* pTexture = CManager::GetTexture();

	m_nTextureIdx = pTexture->Register(pTextureName);
}

//===================================================
// 頂点座標の更新
//===================================================
void CObjectBillboard::UpdateVertexPos(const D3DXVECTOR3 pos)
{
	// 頂点情報のポインタ
	VERTEX_3D* pVtx;

	// 頂点バッファのロック
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	m_pos = pos;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_Size.x, m_Size.y,  m_Size.z);
	pVtx[1].pos = D3DXVECTOR3(m_Size.x,  m_Size.y,  m_Size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_Size.x, -m_Size.y, -m_Size.z);
	pVtx[3].pos = D3DXVECTOR3(m_Size.x,  -m_Size.y, -m_Size.z);

	// 頂点バッファのアンロック
	m_pVtxBuffer->Unlock();
}

//===================================================
// 生成処理
//===================================================
CObjectBillboard* CObjectBillboard::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const char* pTextureName)
{
	CObjectBillboard* pBillboard = nullptr;
	
	// 3Dオブジェクトの生成
	pBillboard = new CObjectBillboard;

	// 優先順位の取得
	int nPriority = pBillboard->GetPriority();

	// 現在のオブジェクトの最大数
	const int nNumAll = CObject::GetNumObject(nPriority);

	// オブジェクトが最大数まであったら
	if (nNumAll >= MAX_OBJECT && pBillboard != nullptr)
	{
		// 自分自身の破棄
		pBillboard->Release();

		// nullにしておく
		pBillboard = nullptr;

		return nullptr;
	}

	if (pBillboard == nullptr) return nullptr;

	pBillboard->SetPosition(pos);
	pBillboard->SetSize(size);
	pBillboard->Init();
	pBillboard->SetOffsetVtx();
	pBillboard->SetTextureID(pTextureName);
	return pBillboard;
}
