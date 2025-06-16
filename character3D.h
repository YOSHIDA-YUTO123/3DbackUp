//===================================================
//
// �L�����N�^�[ [character.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _CHARACTER3D_H_
#define _CHARACTER3D_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include"main.h"
#include"objectX.h"

//***************************************************
// �L�����N�^�[3D�N���X�̒�`
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
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_rot;		// ����
	D3DXMATRIX m_mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_rotDest;	// �ړI�̊p�x
	int m_nLife;			// ����
};

#endif