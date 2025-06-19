//===================================================
//
// �G [enemy.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "enemy.h"
#include "player.h"
#include"manager.h"
#include"renderer.h"
#include "math.h"

//***************************************************
// �}�N����`
//***************************************************

using namespace math;

//===================================================
// �R���X�g���N�^
//===================================================
CEnemy::CEnemy(int nPriority) : CCharacter3D(nPriority)
{
	memset(m_apModel, NULL, sizeof(m_apModel));
	m_fSpeed = NULL;
	m_move = VEC3_NULL;
	m_nNumModel = NULL;
	m_pMotion = nullptr;
	m_pAI = nullptr;
}

//===================================================
// �f�X�g���N�^
//===================================================
CEnemy::~CEnemy()
{
}

//===================================================
// ��������
//===================================================
CEnemy* CEnemy::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CEnemy* pEnemy = nullptr;

	// �G�I�u�W�F�N�g�̐���
	pEnemy = new CEnemy;

	// �D�揇�ʂ̎擾
	int nPriority = pEnemy->GetPriority();

	// ���݂̃I�u�W�F�N�g�̍ő吔
	const int nNumAll = CObject::GetNumObject(nPriority);

	// �I�u�W�F�N�g���ő吔�܂ł�������
	if (nNumAll >= MAX_OBJECT && pEnemy != nullptr)
	{
		// �����̃|�C���^�̉��
		pEnemy->Release();

		// null�ɂ���
		pEnemy = nullptr;

		// �I�u�W�F�N�g������
		return nullptr;
	}

	if (pEnemy == nullptr) return nullptr;

	pEnemy->SetPosition(pos);
	pEnemy->SetRotation(rot);
	pEnemy->Init();

	return pEnemy;
}

//===================================================
// ����������
//===================================================
HRESULT CEnemy::Init(void)
{
	m_pMotion = CMotion::Load("data/MOTION/motionEnemy000.txt", &m_apModel[0], ENEMY_MAX_PARTS, &m_nNumModel, MOTIONTYPE_MAX);

	// �G��AI�̐���
	m_pAI = new CEnemyAI;

	// �G��AI����������
	m_pAI->Init(m_pMotion);

	return S_OK;
}

//===================================================
// �I������
//===================================================
void CEnemy::Uninit(void)
{
	for (int nCnt = 0; nCnt < ENEMY_MAX_PARTS; nCnt++)
	{
		// ���f���̔j��
		if (m_apModel[nCnt] != nullptr)
		{
			// �I������
			m_apModel[nCnt]->Uninit();

			delete m_apModel[nCnt];

			m_apModel[nCnt] = nullptr;
		}
	}

	// ���[�V�����̏I������
	m_pMotion->Uninit();

	// ���[�V�����̔j��
	if (m_pMotion != nullptr)
	{
		delete m_pMotion;

		m_pMotion = nullptr;
	}

	if (m_pAI != nullptr)
	{
		delete m_pAI;
		m_pAI = nullptr;
	}
	CCharacter3D::Uninit();
}

//===================================================
// �X�V����
//===================================================
void CEnemy::Update(void)
{
	CPlayer* pPlayer = CManager::GetPlayer();

	D3DXVECTOR3 pos = GetPosition();

	D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();

	// �ړ��ʂ̌���
	m_move.x += (0.0f - m_move.x) * 0.25f;
	m_move.z += (0.0f - m_move.z) * 0.25f;

	pos += m_move;

	// ���b�V���t�B�[���h�̎擾
	CMeshField* pMesh = CManager::GetMeshField();

	pMesh->Collision(&pos);
	
	m_move.y += -MAX_GLABITY;

	if (m_pMotion != nullptr)
	{
		float fAngle = GetTargetAngle(PlayerPos, pos);		  // �^�[�Q�b�g�܂ł̊p�x�̎擾

		// �ҋ@����Ȃ�������
		if (m_pAI != nullptr && m_pAI->Wait() == false)
		{
			// ���̋����ɂ�����
			if (m_pAI->CheckDistance(PlayerPos, pos, 1000.0f) == MOTIONTYPE_MOVE)
			{
				m_pMotion->SetMotion(MOTIONTYPE_MOVE, true, 10);
			}
			// ���Ȃ�������
			else if (m_pAI->CheckDistance(PlayerPos, pos, 1000.0f) == MOTIONTYPE_NEUTRAL)
			{
				m_pMotion->SetMotion(MOTIONTYPE_NEUTRAL, true, 10);
			}

			// �U���\��������
			if (m_pAI->IsAttack() == MOTIONTYPE_ACTION)
			{
				m_pMotion->SetMotion(MOTIONTYPE_ACTION, true, 10);

				// �ړI�̊p�x�̐ݒ�
				SetRotDest(D3DXVECTOR3(0.0f, fAngle + D3DX_PI, 0.0f));
			}
		}

		// AI�̍X�V����
		if (m_pAI != nullptr)
		{
			m_pAI->Update();
		}

		// ���[�V�����̍X�V����
		m_pMotion->Update(&m_apModel[0], m_nNumModel);
	}

	// ���[�V�����̑J��
	TransitionMotion();

	SetPosition(pos);

	SmoothAngle(0.1f);
}

//===================================================
// �`�揈��
//===================================================
void CEnemy::Draw(void)
{
	// �L�����N�^�[�̕`��
	CCharacter3D::Draw();

	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		if (m_apModel[nCnt] != nullptr)
		{
			// �`�揈��
			m_apModel[nCnt]->Draw();

			// ���f���̉e�̕`�揈��
			m_apModel[nCnt]->DrawShadow();
		}
	}
}

//===================================================
// �G�̈ړ�����
//===================================================
void CEnemy::UpdateMoveMotion(void)
{
	CPlayer* pPlayer = CManager::GetPlayer();

	D3DXVECTOR3 pos = GetPosition();

	D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();

	D3DXVECTOR3 TargetDir = GetTargetDir(PlayerPos, pos); // �^�[�Q�b�g�܂ł̕������擾
	float fAngle = GetTargetAngle(PlayerPos, pos);		  // �^�[�Q�b�g�܂ł̊p�x�̎擾

	// �ړ��ʂ̐ݒ�
	m_move.x = TargetDir.x * 10.0f;
	m_move.z = TargetDir.z * 10.0f;

	// �ړI�̊p�x�̐ݒ�
	SetRotDest(D3DXVECTOR3(0.0f, fAngle + D3DX_PI, 0.0f));
}

//===================================================
// ���[�V�����̑J��
//===================================================
void CEnemy::TransitionMotion(void)
{
	// ���[�V�����̎�ނ̎擾
	MOTIONTYPE type = (MOTIONTYPE)m_pMotion->GetBlendMotionType();

	switch (type)
	{
	case MOTIONTYPE_NEUTRAL:
		break;
	case MOTIONTYPE_MOVE:

		// �G�̈ړ�����
		UpdateMoveMotion();

		break;
	case MOTIONTYPE_ACTION:
		break;
	default:
		break;
	}
}

//===================================================
// �G��AI�̃R���X�g���N�^
//===================================================
CEnemyAI::CEnemyAI()
{
	m_pMotion = nullptr;
	m_Action = ACTION_IDLE;
	m_nCounterAction = NULL;
}

//===================================================
// �G��AI�̃f�X�g���N�^
//===================================================
CEnemyAI::~CEnemyAI()
{

}

//===================================================
// �G��AI�̏���������
//===================================================
void CEnemyAI::Init(CMotion* pMotion, const int nBlendFrame, const int nFirstMotion)
{
	// ���[�V�����̏��̎擾
	m_pMotion = pMotion;

	m_pMotion->SetMotion(nFirstMotion, true, nBlendFrame);
}

//===================================================
// �X�V����
//===================================================
void CEnemyAI::Update(void)
{
	switch (m_Action)
	{
	case ACTION_WAIT:
		m_nCounterAction--;

		if (m_nCounterAction <= 0)
		{
			m_Action = ACTION_IDLE;
		}
		break;
	default:
		break;
	}
}

//===================================================
// �G�̈ړ���AI
//===================================================
int CEnemyAI::CheckDistance(const D3DXVECTOR3 dest, const D3DXVECTOR3 pos,const float fRadius)
{
	if (m_Action != ACTION_IDLE && m_Action != ACTION_MOVE) return -1;

	// ���������߂�
	D3DXVECTOR3 Diff = dest - pos;

	// ���������߂�
	float fDistance = GetDistance(Diff);

	if (fDistance >= fRadius)
	{
		m_Action = ACTION_MOVE;

		return CEnemy::MOTIONTYPE_MOVE;
	}

	m_Action = ACTION_IDLE;

	return CEnemy::MOTIONTYPE_NEUTRAL;
}

//===================================================
// �U���J�n����
//===================================================
int CEnemyAI::IsAttack(void)
{
	if (m_Action != ACTION_IDLE) return -1;

	int random = rand() % 100;

	m_Action = ACTION_ATTACK;

	return CEnemy::MOTIONTYPE_ACTION;
}

//===================================================
// �ҋ@���
//===================================================
bool CEnemyAI::Wait(void)
{
	if (m_pMotion->IsFinishEndBlend() == true)
	{
		m_Action = ACTION_WAIT;

		m_pMotion->SetMotion(CEnemy::MOTIONTYPE_NEUTRAL, true, 10);

		m_nCounterAction = 60;

		return true;
	}

	return false;
}
