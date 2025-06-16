//================================================
//
// メッシュドーム [dome.cpp]
// Author: YUTO YOSHIDA
//
//================================================

//************************************************
// インクルードファイル
//************************************************
#include "dome.h"
#include"manager.h"
#include"renderer.h"

//================================================
// コンストラクタ
//================================================
CMeshDome::CMeshDome()
{
	m_pIdxBuffMeshDome = nullptr;
}

//================================================
// デストラクタ
//================================================
CMeshDome::~CMeshDome()
{
}

//================================================
// 生成処理
//================================================
CMeshDome* CMeshDome::Create(const D3DXVECTOR3 pos, const int nSegX, const int nSegZ, const float fRadius, const float fHeight, const D3DXVECTOR3 rot)
{
	// メッシュドームを生成
	CMeshDome* pMesh = new CMeshDome;

	// 優先順位の取得
	int nPriority = pMesh->GetPriority();

	// 現在のオブジェクトの最大数
	const int nNumAll = CObject::GetNumObject(nPriority);

	// オブジェクトが最大数まであったら
	if (nNumAll >= MAX_OBJECT && pMesh != nullptr)
	{
		// 自分のポインタの解放
		pMesh->Release();

		// nullにする
		pMesh = nullptr;

		// オブジェクトを消す
		return nullptr;
	}

	if (pMesh == nullptr) return nullptr;

	// 頂点数の設定
	int nNumFanVtx = (nSegX + 1) + 1;

	// 頂点数の設定
	int nNumDomeVtx = (nSegX + 1) * (nSegZ + 1);

	// ポリゴン数の設定
	int nNumDomePolygon = (((nSegX * nSegZ) * 2)) + (4 * (nSegZ - 1));

	// インデックス数の設定
	int nNumDomeIndex = nNumDomePolygon + 2;

	// 頂点情報の設定
	int nNumVtx = nNumFanVtx + nNumDomeVtx;

	// 頂点の設定
	pMesh->SetVtxElement(nNumVtx, nNumDomePolygon, nNumDomeIndex);
	pMesh->SetSegment(nSegX, nSegZ);
	
	// 初期化処理
	pMesh->Init();

	// 設定処理
	pMesh->SetPosition(pos);
	pMesh->SetRotation(rot);

	pMesh->SetDome(nSegX, nSegZ, fRadius, fHeight);
	return pMesh;
}

//================================================
// 初期化処理
//================================================
HRESULT CMeshDome::Init(void)
{
	// 初期化処理
	if (FAILED(CMesh::Init()))
	{
		return E_FAIL;
	}

	// テクスチャのIDの設定
	CMesh::SetTextureID("data/TEXTURE/sky.jpg");

	return E_FAIL;
}

//================================================
// 終了処理
//================================================
void CMeshDome::Uninit(void)
{
	if (m_pIdxBuffMeshDome != nullptr)
	{
		m_pIdxBuffMeshDome->Release();
		m_pIdxBuffMeshDome = nullptr;
	}

	// 終了処理
	CMesh::Uninit();
}

//================================================
// 更新処理
//================================================
void CMeshDome::Update(void)
{
	UpdateRotation(D3DXVECTOR3(0.0f,0.001f,0.0f));
}

//================================================
// 描画処理
//================================================
void CMeshDome::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	pDevice->SetRenderState(D3DRS_CULLMODE, TRUE);

	// 描画処理
	CMesh::Draw();

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuffMeshDome);

	//テクスチャフォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	LPDIRECT3DTEXTURE9 pTexture = nullptr;

	// テクスチャの取得
	pDevice->GetTexture(0, (IDirect3DBaseTexture9**)&pTexture);

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture);

	int nSegX = GetSegX();

	// 頂点数の設定
	int nNumFanVtx = (nSegX + 1) + 1;

	// ポリゴン数の設定
	int nNumFanPolygon = (nNumFanVtx - 1) * 3;

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, nNumFanVtx, 0, nNumFanPolygon);
	
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//================================================
// ドームの設定処理
//================================================
void CMeshDome::SetDome(const int nSegX, const int nSegZ, const float fRadius, const float fHeight)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	int nCntVtx = 0;
	float fTexX = 1.0f / (nSegX);
	float fTexY = 1.0f / (nSegZ);

	float fNowRadius = fRadius / (nSegZ + 1);

	// 蓋の部分の作成
	for (int nCntX = 0; nCntX <= nSegX; nCntX++)
	{
		float fAngle = (D3DX_PI * 2.0f) / nSegX * nCntX;

		D3DXVECTOR3 posWk = VEC3_NULL,posOld = VEC3_NULL;

		posWk.x = sinf(fAngle) * fNowRadius;
		posWk.y = fHeight;
		posWk.z = cosf(fAngle) * fNowRadius;

		float fPosTexX = fTexX + cosf(fAngle) * fTexX;
		float fPosTexY = fTexY - sinf(fAngle) * fTexY;

		SetVtxBuffer(posWk, nCntVtx, D3DXVECTOR2(fPosTexX, fPosTexY));

		// 最後まで行ったら
		if (nCntX == nSegX)
		{
			// 最後をつなげる
			SetVtxBuffer(posOld, nCntVtx + 1, D3DXVECTOR2(fPosTexX, fPosTexY));
		}

		// 前回の計算を保存
		posOld = posWk;

		nCntVtx++;
	}
	
	int OffsetIdx = nCntVtx;

	// ドームのてっぺんの位置の設定
	SetVtxBuffer(D3DXVECTOR3(0.0f, fHeight + (fHeight / nSegZ + 1),0.0f), 0, D3DXVECTOR2(fTexX, fTexY));

	int nNumVertex = (nSegX + 1) + 1;
	int nNumPolygon = (nNumVertex - 1) * 3;
	int nNumidx = nNumPolygon + 2;

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * nNumidx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuffMeshDome,
		NULL);

	WORD* pIdx;

	//インデックスバッファのロック
	m_pIdxBuffMeshDome->Lock(0, 0, (void**)&pIdx, 0);

	int nCntIdx = 0;

	for (int IndxCount2 = 0; IndxCount2 < nSegX; IndxCount2++)
	{
		pIdx[nCntIdx] = (WORD)0;
		pIdx[nCntIdx + 1] = (WORD)IndxCount2 + 1;
		pIdx[nCntIdx + 2] = (WORD)IndxCount2 + 2;

		nCntIdx += 3;
	}

	pIdx[nCntIdx] = (WORD)0;
	pIdx[nCntIdx + 1] = (WORD)nSegX;
	pIdx[nCntIdx + 2] = (WORD)1;

	nCntIdx += 3;

	//インデックスバッファのアンロック
	m_pIdxBuffMeshDome->Unlock();

	// テクスチャのオフセット
	fTexX = 1.0f / nSegX;
	fTexY = 1.0f / nSegZ;

	// 半径
	fNowRadius = fRadius / (nSegZ + 1);
	float fRateRadius = fNowRadius;

	//縦
	for (int nCntZ = 0; nCntZ <= nSegZ; nCntZ++)
	{
		//横
		for (int nCntX = 0; nCntX <= nSegX; nCntX++)
		{
			float fAngel = (D3DX_PI * 2.0f) / nSegX * nCntX;

			D3DXVECTOR3 posWk = VEC3_NULL;

			posWk.x = sinf(fAngel) * fNowRadius;
			posWk.y = fHeight - (fHeight / nSegZ) * nCntZ;
			posWk.z = cosf(fAngel) * fNowRadius;
			
			SetVtxBuffer(posWk, nCntVtx, D3DXVECTOR2(fTexX * nCntX, fTexY * nCntZ));

			nCntVtx++;//加算
		}
		fNowRadius += fRateRadius / (nCntZ + 1);
	}

	int IndxCount3 = nSegX + 1;//X

	int IdxCnt = 0;//配列

	int Num = 0;//

	//インデックスの設定
	for (int IndxCount1 = 0; IndxCount1 < nSegZ; IndxCount1++)
	{
		for (int IndxCount2 = 0; IndxCount2 <= nSegX; IndxCount2++, IndxCount3++, Num++)
		{
			// インデックスバッファの設定
			SetIndexBuffer((WORD)(IndxCount3 + OffsetIdx), IdxCnt);
			SetIndexBuffer((WORD)(Num + OffsetIdx), IdxCnt + 1);
			IdxCnt += 2;
		}

		if (IndxCount1 < nSegZ - 1)
		{
			// インデックスバッファの設定
			SetIndexBuffer((WORD)(Num - 1 + OffsetIdx), IdxCnt);
			SetIndexBuffer((WORD)(IndxCount3 + OffsetIdx), IdxCnt + 1);
			IdxCnt += 2;
		}
	}

}
