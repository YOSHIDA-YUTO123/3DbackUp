//================================================
//
// 数字を操作する処理 [number.h]
// Author: YUTO YOSHIDA
//
//================================================

//*************************************************
// 多重インクルード防止
//*************************************************
#ifndef _NUMBER_H_
#define _NUMBER_H_

//************************************************
// インクルードファイル
//************************************************
#include"main.h"
#include"object.h"

//************************************************
// ナンバークラスの定義
//************************************************
class CNumber
{
public:

	CNumber();
	~CNumber();

	HRESULT Init(const int nPosX, const int nPosY, const D3DXVECTOR3 pos, const float fWidth, const float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetSize(const float fWIdth, const float fHeight);
	void SetPos(const D3DXVECTOR3 pos);
	void SetUV(const int nNum);
	void SetColor(const D3DXCOLOR col);
	void SetTextureID(const char* pFileName);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// 頂点へのポインタ
	D3DXVECTOR3 m_pos;						// 位置
	float m_fWidth, m_fHeight;				// 幅、高さ
	int m_nTextureIdx;						// テクスチャのインデックス
};
#endif