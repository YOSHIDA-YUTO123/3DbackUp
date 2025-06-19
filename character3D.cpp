//===================================================
//
// �L�����N�^�[3D [character.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "character3D.h"
#include "textureManager.h"
#include"manager.h"
#include"renderer.h"
#include"math.h"

//===================================================
// �R���X�g���N�^
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
// �f�X�g���N�^
//===================================================
CCharacter3D::~CCharacter3D()
{
}

//===================================================
// ����������
//===================================================
HRESULT CCharacter3D::Init(void)
{
	return S_OK;
}

//===================================================
// �I������
//===================================================
void CCharacter3D::Uninit(void)
{
	// �������g�̔j��
	Release();
}

//===================================================
// �X�V����
//===================================================
void CCharacter3D::Update(void)
{

}

//===================================================
// �`�揈��
//===================================================
void CCharacter3D::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//===================================================
// �L�����N�^�[�̐ݒ�
//===================================================
void CCharacter3D::SetCharacter(const int nLife)
{
	m_nLife = nLife;
}

//===================================================
// �L�����N�^�[�̃q�b�g����
//===================================================
bool CCharacter3D::HitCharacter(int nDamage)
{
	// ���񂾂��ǂ���
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
// �ړI�̊p�x�ɋ߂Â��鏈��S
//===================================================
void CCharacter3D::SmoothAngle(const float fCoefficient)
{
	// �����̍���
	float fDiffRot = m_rotDest.y - m_rot.y;

	// �p�x�̐��K��
	NormalizeDiffRot(fDiffRot, &m_rot.y);

	// ������ړI�̌����ɋ߂Â���
	m_rot = LerpDest(m_rotDest, m_rot, fCoefficient);
}

