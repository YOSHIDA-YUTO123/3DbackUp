//================================================
//
// トランスフォームを制御する処理 [transform.h]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// 多重インクルード防止
//*************************************************
#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

//*************************************************
// インクルードファイル
//*************************************************
#include"main.h"

//*************************************************
// 位置のクラスの定義
//*************************************************
class CPosition
{
public:
	CPosition();
	~CPosition();

	D3DXVECTOR3 Get(void) const { return m_pos; }
	void Set(const D3DXVECTOR3 pos) { m_pos = pos; }

	void UpdatePosition(const D3DXVECTOR3 move);
private:
	D3DXVECTOR3 m_pos; // 位置
};

//*************************************************
// 回転のクラスの定義
//*************************************************
class CRotation
{
public:
	CRotation();
	~CRotation();

	D3DXVECTOR3 Get(void) const { return m_rot; }
	void Set(const D3DXVECTOR3 rot) { m_rot = rot; }
	void Add(const D3DXVECTOR3 rot) { m_rot += rot; }
private:
	D3DXVECTOR3 m_rot; // 向き
};

//*************************************************
// 移動のクラスの定義
//*************************************************
class CVelocity
{
public:
	CVelocity();
	~CVelocity();

	D3DXVECTOR3 Get(void) const { return m_move; }
	void Set(const D3DXVECTOR3 move) { m_move = move; }
	void Add(const D3DXVECTOR3 move) { m_move += move; }

	void SetInertia(const float fcoef);
private:
	D3DXVECTOR3 m_move; // 移動量
};

//*************************************************
// 大きさのクラスの定義
//*************************************************
class CScal
{
public:
	CScal();
	~CScal();

	D3DXVECTOR3 Get(void) const { return m_Size; }
	void Set(const D3DXVECTOR3 move) { m_Size = move; }
private:
	D3DXVECTOR3 m_Size;
};

#endif