//================================================
//
// メッシュフィールド [meshfield.cpp]
// Author: YUTO YOSHIDA
//
//================================================

//************************************************
// インクルードファイル
//************************************************
#include "meshfield.h"
#include"manager.h"
#include "math.h"

//************************************************
// マクロ定義
//************************************************
#define NUM_POLYGON (2)		 // 判定するポリゴンの数
#define NULL_VECTOR (999.0f) // vectorの空配列

//================================================
// コンストラクタ
//================================================
CMeshField::CMeshField()
{
	m_fWidth = NULL;
	m_fHeight = NULL;
	memset(&m_Wave, NULL, sizeof(m_Wave));
	m_pNor = nullptr;
}

//================================================
// デストラクタ
//================================================
CMeshField::~CMeshField()
{
}

//================================================
// 生成処理
//================================================
CMeshField* CMeshField::Create(const D3DXVECTOR3 pos, const int nSegX, const int nSegZ, const D3DXVECTOR2 Size, const D3DXVECTOR3 rot)
{
	// メッシュフィールドを生成
	CMeshField* pMeshField = new CMeshField;

	// 優先順位の取得
	int nPriority = pMeshField->GetPriority();

	// 現在のオブジェクトの最大数
	const int nNumAll = CObject::GetNumObject(nPriority);

	// オブジェクトが最大数まであったら
	if (nNumAll >= MAX_OBJECT && pMeshField != nullptr)
	{
		// 自分のポインタの解放
		pMeshField->Release();

		// nullにする
		pMeshField = nullptr;

		// オブジェクトを消す
		return nullptr;
	}

	if (pMeshField == nullptr) return nullptr;
	
	// 頂点数の設定
	int nNumVtx = (nSegX + 1) * (nSegZ + 1);

	// ポリゴン数の設定
	int nNumPolygon = ((nSegX * nSegZ) * 2) + (4 * (nSegZ - 1));

	// インデックス数の設定
	int nNumIndex = nNumPolygon + 2;

	// 頂点の設定
	pMeshField->SetVtxElement(nNumVtx, nNumPolygon, nNumIndex);
	pMeshField->SetSegment(nSegX, nSegZ);

	// 初期化処理
	pMeshField->Init();

	// 設定処理
	pMeshField->SetPosition(pos);
	pMeshField->SetRotation(rot);
	pMeshField->SetMeshField(nSegX, nSegZ, pos, Size);
	pMeshField->m_fWidth = Size.x;
	pMeshField->m_fHeight = Size.y;

	return pMeshField;
}

//================================================
// 初期化処理
//================================================
HRESULT CMeshField::Init(void)
{
	if (FAILED(CMesh::Init()))
	{
		return E_FAIL;
	}

	// テクスチャのIDの設定
	CMesh::SetTextureID("data/TEXTURE/field.png");

	return S_OK;
}

//================================================
// 終了処理
//================================================
void CMeshField::Uninit(void)
{
	if (m_pNor != nullptr)
	{
		delete[]m_pNor;
		m_pNor = nullptr;
	}
	// 終了処理
	CMesh::Uninit();
}

//================================================
// 更新処理
//================================================
void CMeshField::Update(void)
{
	CPlayer* pPlayer = CManager::GetPlayer();
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	int nSegX = GetSegX();
	int nSegZ = GetSegZ();

	// 頂点数の設定
	int nNumVtx = (nSegX + 1) * (nSegZ + 1);

	//// ポリゴン数の設定
	//int nNumPolygon = ((nSegX * nSegZ) * 2) + (4 * (nSegZ - 1));

	//// インデックス数の設定
	//int nNumIndex = nNumPolygon + 2;

	//for (int nCnt = 0; nCnt < nNumIndex; nCnt++)
	//{
	//	// インデックスを取得
	//	int nIdx0 = GetIndex(nCnt);
	//	int nIdx1 = GetIndex(nCnt + 1);
	//	int nIdx2 = GetIndex(nCnt + 2);

	//	// 頂点を取得
	//	D3DXVECTOR3 vtx0 = GetVtxPos(nIdx0);
	//	D3DXVECTOR3 vtx1 = GetVtxPos(nIdx1);
	//	D3DXVECTOR3 vtx2 = GetVtxPos(nIdx2);

	//	D3DXVECTOR3 edge0 = vtx1 - vtx0; // 辺ベクトル0
	//	D3DXVECTOR3 edge1 = vtx2 - vtx1; // 辺ベクトル1
	//	D3DXVECTOR3 edge2 = vtx0 - vtx2; // 辺ベクトル2

	//	D3DXVECTOR3 Normal = { 0.0f,-1.0f,0.0f};

	//	//if (nCnt % 2 == 0)
	//	//{
	//	//	// 偶数番目の三角形
	//	//	D3DXVec3Cross(&Normal, &edge0, &edge1);
	//	//}
	//	//else
	//	//{
	//	//	// 奇数番目の三角形（順序が逆になっている）
	//	//	D3DXVec3Cross(&Normal, &edge1, &edge0);
	//	//}

	//	//D3DXVec3Normalize(&Normal, &Normal);

	//	SetNormal(Normal, nIdx0);
	//	SetNormal(Normal, nIdx1);
	//	SetNormal(Normal, nIdx2);
	//}
#if 0

	if (m_Wave.bUse == true)
	{
		if (m_Wave.nWaveCounter >= m_Wave.nWaveTime)
		{
			m_Wave.bUse = false;
			m_Wave.nWaveCounter = 0;
		}

		m_Wave.nWaveCounter++;

		//m_fTime += 5.0f;
		m_Wave.fTime += m_Wave.fSpeed;
	}

	for (int nCnt = 0; nCnt < nNumVtx; nCnt++)
	{
		D3DXVECTOR3 pos = GetVtxPos(nCnt);

		D3DXVECTOR3 diff = m_Wave.epicenter - pos;

		float dis = sqrtf((diff.x * diff.x) + (diff.z * diff.z));

		if (dis >= (m_Wave.fInRadius + m_Wave.fTime) && dis <= (m_Wave.fOutRadius + m_Wave.fTime) && m_Wave.bUse == true)
		{
			float dest = m_Wave.fWaveHeight + sinf(dis * 0.005f);

			pos.y += (dest - pos.y) * 0.1f;
		}
		else
		{
			float fRate = m_Wave.bUse ? 0.05f : 0.001f;
			pos.y += (0.0f - pos.y) * fRate;
		}
		SetVtxPos(pos, nCnt);
	}
#endif // 0

	
#if 0

	m_Wave.fTime += 1.0f / 25.0f;
	
	for (int nCnt = 0; nCnt < nNumVtx; nCnt++)
	{
		D3DXVECTOR3 pos = GetVtxPos(nCnt);

		D3DXVECTOR3 diff = pPlayer->GetPosition() - pos;

		float dis = sqrtf((diff.x * diff.x) + (diff.z * diff.z));

		float offset = sinf((dis * 0.005f) + m_Wave.fTime);

		pos.y = offset * 150.0f;
		
		SetVtxPos(pos, nCnt);
	}
#endif // 0

	for (int nCnt = 0; nCnt < nNumVtx; nCnt++)
	{
		D3DXVECTOR3 pos = GetVtxPos(nCnt);

		D3DXVECTOR3 diff = pPlayer->GetPosition() - pos;

		float sqdiffX = diff.x * diff.x;
		float sqdiffY = diff.y * diff.y;
		float sqdiffZ = diff.z * diff.z;

		float fDistance = (sqdiffX + sqdiffY + sqdiffZ);

		float fRadius = 50.0f + 50.0f;

		fRadius = fRadius * fRadius;

		if (fDistance <= fRadius)
		{
			if (pKeyboard->GetPress(DIK_UP))
			{
				pos.y += 5.0f;
			}
			else if (pKeyboard->GetPress(DIK_DOWN))
			{
				pos.y -= 5.0f;
			}
		}
		SetVtxPos(pos, nCnt);
	}

	int nCnt = 0;

	//インデックスの設定
	for (int nCntZ = 0; nCntZ <= nSegZ; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= nSegX; nCntX++)
		{
			D3DXVECTOR3 vtx0, vtx1, vtx2,vtx3;
			vtx0 = vtx1 = vtx2 = vtx3 = VEC3_NULL;

			D3DXVECTOR3 vec0, vec1, vec2, vec3;
			vec0 = vec1 = vec2 = vec3 = VEC3_NULL;
			int nIdx0 = 0;
			int nIdx1 = 0;
			int nIdx2 = 0;
			D3DXVECTOR3 Nor0,Nor1,Nor2,Nor3;
			Nor0 = Nor1 = Nor2 = Nor3 = VEC3_NULL;

			D3DXVECTOR3 Normal = VEC3_NULL;

			// 左上だったら
			if (nCntX == 0 && nCntZ == 0)
			{
				nIdx0 = 0;
				nIdx1 = 1;
				nIdx2 = nSegX + 1;

				vtx0 = GetVtxPos(nIdx0);
				vtx1 = GetVtxPos(nIdx1);
				vtx2 = GetVtxPos(nIdx2);

				vec0 = vtx1 - vtx0;
				vec1 = vtx2 - vtx0;

				D3DXVec3Cross(&Normal, &vec0, &vec1);
			}
			else
			{
				// 左の辺(角以外)だったら
				if (nCntX == 0 && nCnt == (nSegX + 1) * nCntZ)
				{
					nIdx0 = nCnt - (nSegX + 1);
					nIdx1 = nCnt + 1;
					nIdx2 = nCnt + (nSegX + 1);

					vtx0 = GetVtxPos(nIdx0);
					vtx1 = GetVtxPos(nIdx1);
					vtx2 = GetVtxPos(nIdx2);
					vtx3 = GetVtxPos(nCnt);

					vec0 = vtx0 - vtx3;
					vec1 = vtx1 - vtx3;
					vec2 = vtx2 - vtx3;

					D3DXVec3Cross(&Nor0, &vec0, &vec1);
					D3DXVec3Cross(&Nor1, &vec1, &vec2);

					Normal = (Nor0 + Nor1) * 0.5f;
				}
			}

			// 左下だったら
			if (nCntX == 0 && nCntZ == nSegZ)
			{
				nIdx0 = (nSegX + 1) * nSegZ;
				nIdx1 = (nSegX + 1) * (nSegZ - 1);
				nIdx2 = ((nSegX + 1) * nSegZ) + 1;

				vtx0 = GetVtxPos(nIdx0);
				vtx1 = GetVtxPos(nIdx1);
				vtx2 = GetVtxPos(nIdx2);

				vec0 = vtx1 - vtx0;
				vec1 = vtx2 - vtx0;

				D3DXVec3Cross(&Normal, &vec0, &vec1);
			}

			// 右上だったら
			if (nCntX == nSegX && nCntZ == 0)
			{
				nIdx0 = nSegX;
				nIdx1 = nSegX - 1;
				nIdx2 = nSegX + 1 + nCntX;

				vtx0 = GetVtxPos(nIdx0);
				vtx1 = GetVtxPos(nIdx1);
				vtx2 = GetVtxPos(nIdx2);

				vec0 = vtx1 - vtx0;
				vec1 = vtx2 - vtx0;

				D3DXVec3Cross(&Normal, &vec1, &vec0);
			}
			else
			{
				// 右の辺(角以外)だったら
				if (nCntX == nSegX && nCnt == (nCntX * (nCntZ + 1)) + nCntZ)
				{
					nIdx0 = nCnt - (nSegX + 1);
					nIdx1 = nCnt - 1;
					nIdx2 = nCnt + (nSegX + 1);

					vtx0 = GetVtxPos(nIdx0);
					vtx1 = GetVtxPos(nIdx1);
					vtx2 = GetVtxPos(nIdx2);
					vtx3 = GetVtxPos(nCnt);

					vec0 = vtx0 - vtx3;
					vec1 = vtx1 - vtx3;
					vec2 = vtx2 - vtx3;

					D3DXVec3Cross(&Nor0, &vec1, &vec0);
					D3DXVec3Cross(&Nor1, &vec2, &vec1);

					Normal = (Nor0 + Nor1) * 0.5f;
				}
			}

			// 右下だったら
			if (nCnt == ((nSegX + 1) * (nSegZ + 1)) - 1)
			{
				nIdx0 = ((nSegX + 1) * (nSegZ + 1)) - 1;
				nIdx1 = nIdx0 - (nSegX + 1);
				nIdx2 = nIdx0 - 1;

				vtx0 = GetVtxPos(nIdx0);
				vtx1 = GetVtxPos(nIdx1);
				vtx2 = GetVtxPos(nIdx2);

				vec0 = vtx1 - vtx0;
				vec1 = vtx2 - vtx0;

				D3DXVec3Cross(&Normal, &vec1, &vec0);
			}


			D3DXVec3Normalize(&Normal, &Normal);

			SetNormal(Normal, nCnt);
			nCnt++;
		}
	}
}

//================================================
// 描画処理
//================================================
void CMeshField::Draw(void)
{
	// 描画処理
	CMesh::Draw();
}

//================================================
// メッシュフィールドの設定処理
//================================================
void CMeshField::SetMeshField(const int nSegX, const int nSegZ, const D3DXVECTOR3 pos,const D3DXVECTOR2 Size)
{
	int nCntVtx = 0;

	float fTexPosX = 1.0f / nSegX;
	float fTexPosY = 1.0f / nSegZ;

	D3DXVECTOR3 posWk;

	int nNumVtx = (nSegX + 1) * (nSegZ + 1);

	m_pNor = new D3DXVECTOR3[nNumVtx];

	for (int nCntZ = 0; nCntZ <= nSegZ; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= nSegX; nCntX++)
		{
			// 位置の設定
			posWk.x = ((Size.x / nSegX) * nCntX) - (Size.x * 0.5f);
			posWk.y = pos.y;
			posWk.z = Size.y - ((Size.y / nSegZ) * nCntZ) - (Size.y * 0.5f);

			// 頂点バッファの設定
			SetVtxBuffer(posWk, nCntVtx, D3DXVECTOR2((fTexPosX * nCntX), (fTexPosY * nCntZ)),D3DXVECTOR3(0.0f,0.0f,0.0f));

			nCntVtx++;
		}
	}

	int IndxNum = nSegX + 1;//X

	int IdxCnt = 0;//配列

	int Num = 0;//

	//インデックスの設定
	for (int IndxCount1 = 0; IndxCount1 < nSegZ; IndxCount1++)
	{
		for (int IndxCount2 = 0; IndxCount2 <= nSegX; IndxCount2++, IndxNum++, Num++)
		{
			// インデックスバッファの設定
			SetIndexBuffer((WORD)IndxNum, IdxCnt);
			SetIndexBuffer((WORD)Num, IdxCnt + 1);
			IdxCnt += 2;
		}

		// NOTE:最後の行じゃなかったら
		if (IndxCount1 < nSegZ - 1)
		{
			SetIndexBuffer((WORD)Num - 1, IdxCnt);
			SetIndexBuffer((WORD)IndxNum, IdxCnt + 1);
			IdxCnt += 2;
		}
	}
}

//================================================
// メッシュフィールドの当たり判定
//================================================
bool CMeshField::Collision(D3DXVECTOR3* pPos)
{
	// 着地判定
	bool bLanding = false;

	int nSegX = GetSegX();
	int nSegZ = GetSegZ();

	// 1マスのサイズ
	float GridSizeX = m_fWidth / (float)nSegX;
	float GridSizeZ = m_fHeight / (float)nSegZ;

	float X = pPos->x + (m_fWidth * 0.5f);
	float Z = (m_fHeight * 0.5f) - pPos->z;

	// 何番目のポリゴンか
	int polyX = (int)(X / GridSizeX);
	int polyZ = (int)(Z / GridSizeZ);

	// 現在のポリゴンのインデックス番号
	int polyIndex = ((polyZ * (nSegX - 1) + polyX) * 2) + (polyZ * 6);

	// ポリゴン数の設定
	int nNumPolygon = ((nSegX * nSegZ) * 2) + (4 * (nSegZ - 1));

	// インデックス数の設定
	int nNumIndex = nNumPolygon + 2;

	for (int nCnt = 0; nCnt < NUM_POLYGON; nCnt++)
	{
		// 頂点のインデックス
		int nCntVertex = (polyIndex + nCnt);

		// マイナスだったら
		if (nCntVertex < 0 || nCntVertex > nNumIndex) break;

		// インデックスを取得
		int nIdx0 = GetIndex(nCntVertex);
		int nIdx1 = GetIndex(nCntVertex + 1);
		int nIdx2 = GetIndex(nCntVertex + 2);

		// 頂点を取得
		D3DXVECTOR3 vtx0 = GetVtxPos(nIdx0);
		D3DXVECTOR3 vtx1 = GetVtxPos(nIdx1);
		D3DXVECTOR3 vtx2 = GetVtxPos(nIdx2);

		D3DXVECTOR3 edge0 = vtx1 - vtx0; // 辺ベクトル0
		D3DXVECTOR3 edge1 = vtx2 - vtx1; // 辺ベクトル1
		D3DXVECTOR3 edge2 = vtx0 - vtx2; // 辺ベクトル2

		D3DXVECTOR3 Normal = {};

		if (nCnt % 2 == 0)
		{
			// 偶数番目の三角形
			D3DXVec3Cross(&Normal, &edge0, &edge1);
		}
		else
		{
			// 奇数番目の三角形（順序が逆になっている）
			D3DXVec3Cross(&Normal, &edge1, &edge0);
		}

		D3DXVec3Normalize(&Normal, &Normal);

		D3DXVECTOR3 PlayerVec0 = *pPos - vtx0;
		D3DXVECTOR3 PlayerVec1 = *pPos - vtx1;
		D3DXVECTOR3 PlayerVec2 = *pPos - vtx2;

		D3DXVECTOR3 Cross0 = {};
		D3DXVECTOR3 Cross1 = {};
		D3DXVECTOR3 Cross2 = {};

		if (nCnt % 2 == 0)
		{
			// 三角形の頂点から外積
			D3DXVec3Cross(&Cross0, &edge0, &PlayerVec0);
			D3DXVec3Normalize(&Cross0, &Cross0);

			D3DXVec3Cross(&Cross1, &edge1, &PlayerVec1);
			D3DXVec3Normalize(&Cross1, &Cross1);

			D3DXVec3Cross(&Cross2, &edge2, &PlayerVec2);
			D3DXVec3Normalize(&Cross2, &Cross2);
		}
		else
		{
			// 三角形の頂点から外積
			D3DXVec3Cross(&Cross0, &PlayerVec0, &edge0);
			D3DXVec3Normalize(&Cross0, &Cross0);

			D3DXVec3Cross(&Cross1, &PlayerVec1, &edge1);
			D3DXVec3Normalize(&Cross1, &Cross1);

			D3DXVec3Cross(&Cross2, &PlayerVec2, &edge2);
			D3DXVec3Normalize(&Cross2, &Cross2);
		}

		if (Cross0.y >= 0.0f && Cross1.y >= 0.0f && Cross2.y >= 0.0f)
		{
			//float xz = ((pPos->x - vtx0.x) * Normal.x + (pPos->z - pPos->z) * Normal.z);

			//float fHeight = vtx0.y - (xz / Normal.y);

			// 平面の方程式のDを計算
			float D = -(Normal.x * vtx0.x + Normal.y * vtx0.y + Normal.z * vtx0.z);

			// プレイヤーの位置に基づいて、プレイヤーのY位置を計算
			float PosY = (Normal.x * pPos->x + Normal.z * pPos->z + D) / -Normal.y;

			D3DXVECTOR3 field = GetPosition();

			D3DXVECTOR3 vec = vtx0 - *pPos;
			D3DXVec3Normalize(&vec, &vec);

			// プレイヤーがポリゴンの裏側かどうかの判定
			float dot = D3DXVec3Dot(&Normal, &vec); // 法線とプレイヤー方向との内積

			if (dot > 0.0f)
			{
				pPos->y = field.y + PosY;

				bLanding = true;
				break;
			}
		}
	}

	return bLanding;//判定を返す
}

//================================================
// 波の設定処理
//================================================
void CMeshField::SetWave(const D3DXVECTOR3 epicenter, const int nTime,const float fSpeed,const float fInRadius,const float fOutRadius,const float fWaveHeight)
{
	if (m_Wave.bUse == false)
	{
		m_Wave.epicenter = epicenter;
		m_Wave.fSpeed = fSpeed;
		m_Wave.nWaveTime = nTime;
		m_Wave.fInRadius = fInRadius;
		m_Wave.fOutRadius = fOutRadius;
		m_Wave.fWaveHeight = fWaveHeight;
		m_Wave.nWaveCounter = NULL;
		m_Wave.fTime = NULL;

		m_Wave.bUse = true;
	}
}
