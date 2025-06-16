//===================================================
//
// �p�[�e�B�N�� [particle.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "particle.h"
#include "effect.h"

//===================================================
// �R���X�g���N�^
//===================================================
CParticle3D::CParticle3D()
{
	m_col = WHITE;
	m_fRadius = NULL;
	m_move = VEC3_NULL;
	m_pos = VEC3_NULL;
	m_nLife = NULL;
	m_nMaxLife = NULL;
	m_nTime = NULL;
	m_nNumParticle = NULL;
	m_fSpeed = NULL;
}

//===================================================
// �f�X�g���N�^
//===================================================
CParticle3D::~CParticle3D()
{
}

//===================================================
// ��������
//===================================================
CParticle3D* CParticle3D::Create(const D3DXVECTOR3 pos, const D3DXCOLOR col, const int nLife, const float fRadius, const int nNumParticle, const int nTime, const float fSpeed)
{
	CParticle3D* pParticle = nullptr;

	// 3D�I�u�W�F�N�g�̐���
	pParticle = new CParticle3D;

	// �D�揇�ʂ̎擾
	int nPriority = pParticle->GetPriority();

	// ���݂̃I�u�W�F�N�g�̍ő吔
	const int nNumAll = CObject::GetNumObject(nPriority);

	// �I�u�W�F�N�g���ő吔�܂ł�������
	if (nNumAll >= MAX_OBJECT && pParticle != nullptr)
	{
		// �������g�̔j��
		pParticle->Release();

		// null�ɂ��Ă���
		pParticle = nullptr;

		return nullptr;
	}

	if (pParticle == nullptr) return nullptr;

	// ����������
	pParticle->Init();

	// �ݒ菈��
	pParticle->m_pos = pos;
	pParticle->m_fRadius = fRadius;
	pParticle->m_nLife = nLife;
	pParticle->m_nMaxLife = nLife;
	pParticle->m_col = col;
	pParticle->m_nNumParticle = nNumParticle;
	pParticle->m_nTime = nTime;
	pParticle->m_fSpeed = fSpeed;

	return pParticle;
}

//===================================================
// ����������
//===================================================
HRESULT CParticle3D::Init(void)
{
	m_col = WHITE;
	m_fRadius = NULL;
	m_move = VEC3_NULL;
	m_nLife = NULL;

	return S_OK;
}

//===================================================
// �I������
//===================================================
void CParticle3D::Uninit(void)
{
	// �������g�̔j��
	Release();
}

//===================================================
// �X�V����
//===================================================
void CParticle3D::Update(void)
{
	for (int nCnt = 0; nCnt < m_nNumParticle; nCnt++)
	{
		D3DXVECTOR3 pos = m_pos;
		
		// �p�x�̑I�o
		float fAngleX = (float)(rand() % 629 - 314) * 0.01f;
		float fAngleY = (float)(rand() % 629 - 314) * 0.01f;

		// ������int�^�ɕϊ�
		int speed = (int)m_fSpeed;

		// �ړ���
		float fMove = (float)(rand() % speed + m_fSpeed * 0.5f);

		m_move.x = sinf(fAngleX) * sinf(fAngleY) * fMove;
		m_move.y = cosf(fAngleX) * fMove;
		m_move.z = cosf(fAngleX) * sinf(fAngleY) * fMove;

		// ���a��int�^�ɕϊ�
		int radius = (int)m_fRadius;

		// ���a��I�o
		float fRadius = (float)(rand() % radius + m_fRadius * 0.5f);

		// �����̑I�o
		int nLife = rand() % m_nMaxLife - (int)(m_nMaxLife * 0.5f);

		if (m_nTime > 0)
		{
			CEffect3D::Create(pos, fRadius, m_move, m_col, nLife);
		}
	}

	m_nTime--;
	m_nLife--;

	if (m_nLife <= 0)
	{
		Release();
		return;
	}
}

//===================================================
// �`�揈��
//===================================================
void CParticle3D::Draw(void)
{

}
