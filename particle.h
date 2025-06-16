//===================================================
//
// �p�[�e�B�N�� [particle.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include"main.h"
#include"object.h"

//***************************************************
// �I�u�W�F�N�g3D�N���X�̒�`
//***************************************************
class CParticle3D : public CObject
{
public:
	CParticle3D();
	~CParticle3D();

	static CParticle3D* Create(const D3DXVECTOR3 pos, const D3DXCOLOR col, const int nLife, const float fRadius, const int nNumParticle,const int nTime,const float fSpeed);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_move;		// �ړ���
	D3DXCOLOR m_col;		// �F
	float m_fRadius;		// ���a
	float m_fSpeed;			// ����
	int m_nLife;			// ����
	int m_nMaxLife;			// �ő��HP
	int m_nNumParticle;		// ���q�̐�
	int m_nTime;			// �p�[�e�B�N�����o������
};
#endif