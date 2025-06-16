//===================================================
//
// キャラクター [character.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _CHARACTER3D_H_
#define _CHARACTER3D_H_

//***************************************************
// インクルードファイル
//***************************************************
#include"main.h"
#include"objectX.h"

//***************************************************
// キャラクター3Dクラスの定義
//***************************************************
class CCharacter3D : public CObject
{
public:
	CCharacter3D(int nPriority = 2);
	~CCharacter3D();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_rot; }
	D3DXVECTOR3 GetDestRot(void) const { return m_rotDest; }

	void SetRotDest(const D3DXVECTOR3 dest) { m_rotDest = dest; }
	void SetPosition(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRotation(const D3DXVECTOR3 rot) { m_rot = rot; }

	void SetCharacter(const int nLife);
	bool HitCharacter(int nDamage);
	void SmoothAngle(const float fCoefficient);
private:
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_rot;		// 向き
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 m_rotDest;	// 目的の角度
	int m_nLife;			// 寿命
};

#endif