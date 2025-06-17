//===================================================
//
// �v���C���[ [player.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "player.h"
#include "input.h"
#include "manager.h"
#include "effect.h"
#include "particle.h"
#include"model.h"
#include"renderer.h"
#include "impact.h"

//***************************************************
// �}�N����`
//***************************************************
#define MAX_GLABITY (1.0f)			// �d�͂̑傫��
#define PLAYER_JUMP_HEIGHT (25.0f)  // �W�����v��
#define MOVE_SPEED (1.5f)			// �ړ����x

//===================================================
// �R���X�g���N�^
//===================================================
CPlayer::CPlayer(int nPriority) : CCharacter3D(nPriority)
{
	m_move = VEC3_NULL;
	m_fSpeed = NULL;
	m_bJump = false;
	m_pScore = nullptr;
	memset(m_apModel, NULL, sizeof(m_apModel));
	m_nNumModel = NULL;
	m_pMotion = nullptr;
}

//===================================================
// �f�X�g���N�^
//===================================================
CPlayer::~CPlayer()
{
}

//===================================================
// ����������
//===================================================
HRESULT CPlayer::Init(void)
{
	m_pMotion = CMotion::Load("data/MOTION/motionPlayer.txt", &m_apModel[0], NUM_PARTS, &m_nNumModel,MOTIONTYPE_MAX);

	// �X�R�A�̐���
	m_pScore = (CScoreLerper*)CScore::Create(CScore::TYPE_LERPER,D3DXVECTOR3(1150.0f, 50.0f, 0.0f), 180.0f, 30.0f);

	m_fSpeed = MOVE_SPEED;
	return S_OK;
}

//===================================================
// �I������
//===================================================
void CPlayer::Uninit(void)
{
	for (int nCnt = 0; nCnt < NUM_PARTS; nCnt++)
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

	// �X�R�A�̉��
	if (m_pScore != nullptr)
	{
		m_pScore = nullptr;
	}

	// �������g�̔j��
	Release();
}

//===================================================
// �X�V����
//===================================================
void CPlayer::Update(void)
{
	// �L�[�{�[�h�̎擾
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// �R���g���[���[�̎擾
	CInputJoypad* pJoypad = CManager::GetInputJoypad();

	CMeshField* pMesh = CManager::GetMeshField();

	// �ړ�����
	if (pJoypad->GetJoyStickL() == true)
	{
		// �p�b�h�̈ړ�����
		CPlayer::MoveJoypad(pJoypad);
	}
	else
	{
		// �L�[�{�[�h�̈ړ�����
		if(CPlayer::MoveKeyboard(pKeyboard))
		{
			int motiontype = m_bJump ? MOTIONTYPE_MOVE : MOTIONTYPE_JUMP;

			m_pMotion->SetMotion(motiontype, true, 15);
		}
	}

	
	// �ʒu�̎擾
	D3DXVECTOR3 pos = GetPosition();

	// �ړ��ʂ̌���
	m_move.x += (0.0f - m_move.x) * 0.25f;
	m_move.z += (0.0f - m_move.z) * 0.25f;

	float fDiffPosX = 0.0f - pos.x;
	float fDiffPosY = 0.0f - pos.y;
	float fDiffPosZ = 0.0f - pos.z;

	D3DXVECTOR3 DiffPos(fDiffPosX, fDiffPosY, fDiffPosZ);
	D3DXVec3Normalize(&DiffPos, &DiffPos);

	float fDistance = sqrtf(fDiffPosX * fDiffPosX + fDiffPosY * fDiffPosY + fDiffPosZ * fDiffPosZ);

	if (fDistance >= 3100.0f)
	{
		m_move.x += DiffPos.x * 10.0f;
		m_move.z += DiffPos.z * 10.0f;
	}

	// �ʒu�̍X�V
	pos += m_move;

	// ���b�V���t�B�[���h�̓����蔻��
	if (pMesh->Collision(&pos))
	{
		m_bJump = true;
		if (m_pMotion->GetBlendMotionType() == MOTIONTYPE_JUMP) {

			m_pMotion->SetMotion(MOTIONTYPE_LANDING, true, 5);
			CMeshImpact::Create(pos, 18, 1, 10.0f, 80.0f, 10.0f, 60, D3DXCOLOR(1.0f, 1.0f, 0.5f, 1.0f));
		}
	}
	else
	{
		m_bJump = false;
	}
	// �d�͂����Z
	m_move.y += -MAX_GLABITY;


	// �W�����v�ł���Ȃ�
	if ((pKeyboard->GetPress(DIK_SPACE) == true || pJoypad->GetPress(pJoypad->JOYKEY_A) == true) && m_bJump == true)
	{
		m_pMotion->SetMotion(MOTIONTYPE_JUMP, true, 5);

		// �ړ��ʂ�������ɐݒ�
		m_move.y = PLAYER_JUMP_HEIGHT;
		m_bJump = false;
	}

	if (pKeyboard->GetTrigger(DIK_B))
	{
		m_pScore->SetDestScore(100000,120);
	}

	if (pKeyboard->GetTrigger(DIK_RETURN))
	{
		m_pMotion->SetMotion(MOTIONTYPE_ACTION, true, 5);
	}

	// �ʒu�̐ݒ�
	CCharacter3D::SetPosition(pos);

	// �v���C���[�̃��[�V�����̑J��
	TransitionMotion();

	// ���[�V�����̍X�V����
	m_pMotion->Update(&m_apModel[0], m_nNumModel);

	// �p�x�̕��
	CCharacter3D::SmoothAngle(0.1f);
}

//===================================================
// �`�揈��
//===================================================
void CPlayer::Draw(void)
{
	// �L�����N�^�[�̕`��
	CCharacter3D::Draw();

	m_pMotion->Debug();
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
// �L�[�{�[�h�̈ړ�����
//===================================================
bool CPlayer::MoveKeyboard(CInputKeyboard* pKeyboard)
{
	bool bMove = false;
	// �J�����̎擾
	CCamera* pCamera = CManager::GetCamera();

	// �J�����̌���
	D3DXVECTOR3 cameraRot = pCamera->GetRotaition();

	// ����
	float fSpeed = m_fSpeed;

	// �ړI�̊p�x
	float fDestAngle = NULL;

	if (pKeyboard->GetPress(DIK_A))
	{
		//�v���C���[�̈ړ�(��)
		if (pKeyboard->GetPress(DIK_W) == true)
		{
			m_move.x += sinf(cameraRot.y - D3DX_PI * 0.25f) * fSpeed;
			m_move.z += cosf(cameraRot.y - D3DX_PI * 0.25f) * fSpeed;

			fDestAngle = cameraRot.y + D3DX_PI * 0.75f;

			// �ړI�̊p�x�̐ݒ�
			CCharacter3D::SetRotDest(D3DXVECTOR3(0.0f, fDestAngle, 0.0f));

			bMove = true;
		}
		//�v���C���[�̈ړ�(��)
		else if (pKeyboard->GetPress(DIK_S))
		{
			m_move.x += sinf(cameraRot.y - D3DX_PI * 0.75f) * fSpeed;
			m_move.z += cosf(cameraRot.y - D3DX_PI * 0.75f) * fSpeed;

			fDestAngle = cameraRot.y + D3DX_PI * 0.25f;

			// �ړI�̊p�x�̐ݒ�
			CCharacter3D::SetRotDest(D3DXVECTOR3(0.0f, fDestAngle, 0.0f));

			bMove = true;
		}
		// �v���C���[�̈ړ�(��)
		else
		{
			m_move.z += sinf(cameraRot.y) * fSpeed;
			m_move.x -= cosf(cameraRot.y) * fSpeed;

			fDestAngle = cameraRot.y + D3DX_PI * 0.5f;

			// �ړI�̊p�x�̐ݒ�
			CCharacter3D::SetRotDest(D3DXVECTOR3(0.0f, fDestAngle, 0.0f));

			bMove = true;
		}
	}
	//�v���C���[�̈ړ�(�E)
	else if (pKeyboard->GetPress(DIK_D))
	{
		//�v���C���[�̈ړ�(��)
		if (pKeyboard->GetPress(DIK_W))
		{
			m_move.x += sinf(cameraRot.y + D3DX_PI * 0.25f) * fSpeed;
			m_move.z += cosf(cameraRot.y + D3DX_PI * 0.25f) * fSpeed;

			fDestAngle = cameraRot.y - D3DX_PI * 0.75f;

			// �ړI�̊p�x�̐ݒ�
			CCharacter3D::SetRotDest(D3DXVECTOR3(0.0f, fDestAngle, 0.0f));

			bMove = true;
		}
		//�v���C���[�̈ړ�(��)
		else if (pKeyboard->GetPress(DIK_S))
		{
			m_move.x += sinf(cameraRot.y + D3DX_PI * 0.75f) * fSpeed;
			m_move.z += cosf(cameraRot.y + D3DX_PI * 0.75f) * fSpeed;

			fDestAngle = cameraRot.y - D3DX_PI * 0.25f;

			// �ړI�̊p�x�̐ݒ�
			CCharacter3D::SetRotDest(D3DXVECTOR3(0.0f, fDestAngle, 0.0f));

			bMove = true;
		}
		// �v���C���[�̈ړ�(�E)
		else
		{
			m_move.z -= sinf(cameraRot.y) * fSpeed;
			m_move.x += cosf(cameraRot.y) * fSpeed;

			fDestAngle = cameraRot.y - D3DX_PI * 0.5f;

			// �ړI�̊p�x�̐ݒ�
			CCharacter3D::SetRotDest(D3DXVECTOR3(0.0f, fDestAngle, 0.0f));

			bMove = true;
		}
	}
	//�v���C���[�̈ړ�(��)
	else if (pKeyboard->GetPress(DIK_W) == true)
	{
		m_move.x += sinf(cameraRot.y) * fSpeed;
		m_move.z += cosf(cameraRot.y) * fSpeed;

		fDestAngle = cameraRot.y + D3DX_PI;

		// �ړI�̊p�x�̐ݒ�
		CCharacter3D::SetRotDest(D3DXVECTOR3(0.0f, fDestAngle, 0.0f));

		bMove = true;
	}
	//�v���C���[�̈ړ�(��)
	else if (pKeyboard->GetPress(DIK_S) == true)
	{
		m_move.x -= sinf(cameraRot.y) * fSpeed;
		m_move.z -= cosf(cameraRot.y) * fSpeed;

		fDestAngle = cameraRot.y;

		// �ړI�̊p�x�̐ݒ�
		CCharacter3D::SetRotDest(D3DXVECTOR3(0.0f, fDestAngle, 0.0f));

		bMove = true;
	}
	else
	{
		if (m_pMotion->GetBlendMotionType() == MOTIONTYPE_MOVE)
		{
			m_pMotion->SetMotion(MOTIONTYPE_NEUTRAL, true, 15);
		}
	}

	return bMove;
}

//===================================================
// �p�b�h�̈ړ�����
//===================================================
void CPlayer::MoveJoypad(CInputJoypad* pJoypad)
{
	XINPUT_STATE* pStick;

	pStick = pJoypad->GetJoyStickAngle();

	// �J�����̎擾
	CCamera* pCamera = CManager::GetCamera();

	// �J�����̌���
	D3DXVECTOR3 cameraRot = pCamera->GetRotaition();

	// L�X�e�B�b�N�̊p�x
	float LStickAngleY = pStick->Gamepad.sThumbLY;
	float LStickAngleX = pStick->Gamepad.sThumbLX;

	// �f�b�h�]�[��
	float deadzone = 32767.0f * 0.25f;

	// �X�e�B�b�N�̌X�����p�x�����߂�
	float magnitude = sqrtf((LStickAngleX * LStickAngleX) + (LStickAngleY * LStickAngleY));

	// ��������
	if (magnitude > deadzone)
	{
		// �A���O���𐳋K��
		float normalizeX = (LStickAngleX / magnitude);
		float normalizeY = (LStickAngleY / magnitude);

		// �v���C���[�̈ړ���
		float moveX = normalizeX * cosf(-cameraRot.y) - normalizeY * sinf(-cameraRot.y);
		float moveZ = normalizeX * sinf(-cameraRot.y) + normalizeY * cosf(-cameraRot.y);

		// �ړ��ʂ��X�e�B�b�N�̊p�x�ɂ���ĕύX
		float speed = m_fSpeed * ((magnitude - deadzone) / (32767.0f - deadzone));

		// �v���C���[�̈ړ�
		m_move.x += moveX * speed;
		m_move.z += moveZ * speed; 

		// �v���C���[�̊p�x���ړ������ɂ���
		float fDestAngle = atan2f(-moveX, -moveZ);

		CCharacter3D::SetRotDest(D3DXVECTOR3(0.0f, fDestAngle, 0.0f));

		m_pMotion->SetMotion(MOTIONTYPE_MOVE, true, 15);
	}
	else
	{
		if (m_pMotion->GetBlendMotionType() == MOTIONTYPE_MOVE)
		{
			m_pMotion->SetMotion(MOTIONTYPE_NEUTRAL, true, 15);
		}
	}
	
}

//===================================================
// �v���C���[�̃��[�V�����̑J��
//===================================================
void CPlayer::TransitionMotion(void)
{
	// ���[�V�����̎��
	MOTIONTYPE motiontype = (MOTIONTYPE)m_pMotion->GetBlendMotionType();

	// ���[�V�����̑J��
	switch (motiontype)
	{
	case MOTIONTYPE_NEUTRAL:
		break;
	case MOTIONTYPE_MOVE:
		break;
	case MOTIONTYPE_ACTION:
		break;
	default:
		break;
	}
}


//===================================================
// ��������
//===================================================
CPlayer* CPlayer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CPlayer* pPlayer = nullptr;

	// 3D�I�u�W�F�N�g�̐���
	pPlayer = new CPlayer;

	// �D�揇�ʂ̎擾
	int nPriority = pPlayer->GetPriority();

	// ���݂̃I�u�W�F�N�g�̍ő吔
	const int nNumAll = CObject::GetNumObject(nPriority);

	// �I�u�W�F�N�g���ő吔�܂ł�������
	if (nNumAll >= MAX_OBJECT && pPlayer != nullptr)
	{
		// �����̃|�C���^�̉��
		pPlayer->Release();

		// null�ɂ���
		pPlayer = nullptr;

		// �I�u�W�F�N�g������
		return nullptr;
	}

	if (pPlayer == nullptr) return nullptr;

	pPlayer->SetPosition(pos);
	pPlayer->SetRotation(rot);
	pPlayer->Init();

	return pPlayer;
}
