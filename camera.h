//===================================================
//
// カメラ [camera.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _CAMERA_H_
#define _CAMERA_H_

//***************************************************
// インクルードファイル
//***************************************************
#include"main.h"

//***************************************************
// カメラクラスの定義
//***************************************************
class CCamera
{
public:
	CCamera();
	~CCamera();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);
	void MouseWheel(void);
	void MouseView(void);
	void PadView(void);

	D3DXVECTOR3 GetRotaition(void) const { return m_rot; }
	D3DXVECTOR3 GetPosV(void) const { return m_posV; }
	D3DXVECTOR3 GetPosR(void) const { return m_posR; }

private:
	D3DXMATRIX m_mtxView;		// ビューマトリックス
	D3DXMATRIX m_mtxProjection;	// プロジェクションマトリックス
	D3DXVECTOR3 m_posV;			// 視点
	D3DXVECTOR3 m_posR;			// 注視点
	D3DXVECTOR3 m_vecU;			// 上方向ベクトル
	D3DXVECTOR3 m_rot;			// 向き
	D3DXVECTOR3 m_posRDest;		// 目的の注視点
	D3DXVECTOR3 m_posVDest;		// 目的の視点
	float m_fDistance;			// 距離
};

#endif