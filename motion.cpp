//===================================================
//
// ���[�V���� [motion.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "motion.h"
#include<stdio.h>
#include"object.h"
#include"process.h"
#include "debugproc.h"
#include "manager.h"

//***************************************************
// �}�N����`
//***************************************************
#define NEUTRAL (0) // �j���[�g����

//===================================================
// �R���X�g���N�^
//===================================================
CMotion::CMotion()
{
	m_apMotionInfo = nullptr;
	m_bLoopMotion = false;
	m_nCountMotion = NULL;
	m_nextKey = NULL;
	m_nKey = NULL;
	m_nNumKey = NULL;
	m_nNumMotion = NULL;
	m_nNumModel = NULL;
	m_motiontype = NULL;
	m_motiontypeBlend = NULL;
	m_bFinishMotion = false;
	m_bFirstMotion = false;
	m_bBlendMotion = false;
	m_bLoopMotionBlend = false;
	m_nCounterBlend = NULL;
	m_nCounterMotionBlend = NULL;
	m_nNextKeyBlend = NULL;
	m_nFrameBlend = NULL;
	m_nKeyBlend = NULL;
	m_nNumKeyBlend = NULL;
}

//===================================================
// �f�X�g���N�^
//===================================================
CMotion::~CMotion()
{
}

//===================================================
// ���[�V�����̃��[�h����
//===================================================
CMotion* CMotion::Load(const char* pLoadFileName, CModel** ppModel, const int nMaxSize, int* pOutModel,const int nNumMotion)
{
	// ���[�V�����𐶐�
	CMotion* pMotion = new CMotion;

	// ���[�V�������\���̂̃������̊m��
	pMotion->m_apMotionInfo = new CMotion::Motion_Info[nNumMotion];

	// �t�@�C�������[�h����
	ifstream File(pLoadFileName);
	string line;
	string input;

	int nNumModel = 0;

	D3DXVECTOR3 offset = VEC3_NULL;

	bool bCharacterSet = false;	// �L�����N�^�[�̐ݒ���������ǂ���

	if (File.is_open() == true)
	{
		// �Ō�̍s�ɂȂ�܂œǂݍ���
		while (getline(File, line))
		{
			size_t equal_pos = line.find("="); // =�̈ʒu

			// [=] ���������߂�
			input = line.substr(equal_pos + 1);

			// ���f���̃��[�h����
			if (pMotion->LoadModel(ppModel, nMaxSize, nNumModel, input, line, pMotion))
			{
				nNumModel++;
			}

			// �p�[�c�̐ݒ肪�I����Ė���������
			if (bCharacterSet == false)
			{
				bCharacterSet = pMotion->LoadCharacterSet(ppModel, line,input, pMotion);
			}

			// ���[�V�����̐ݒ�̓ǂݍ���
			pMotion->LoadMotionSet(pMotion, File, line,nNumMotion);

			// ���[�V�����̐����ő�܂ōs������
			if (pMotion->m_nNumMotion >= nNumMotion)
			{
				break;
			}
		}
		// �t�@�C�������
		File.close();
	}
	else
	{
		// �������̔j��
		if (pMotion->m_apMotionInfo != nullptr)
		{
			delete[] pMotion->m_apMotionInfo;

			pMotion->m_apMotionInfo = nullptr;
		}
		
		// �������̔j��
		if (pMotion != nullptr)
		{
			delete pMotion;
			pMotion = nullptr;
		}

		// ���b�Z�[�W�{�b�N�X
		MessageBox(NULL, pLoadFileName, "�t�@�C�����J���܂���ł���", MB_OK);

		return nullptr;
	}

	*pOutModel = nNumModel;

	return pMotion;
}

//===================================================
// ���f���̃��[�h
//===================================================
bool CMotion::LoadModel(CModel** ppModel, const int nMaxSize, int nCnt, string Input, string line, CMotion *pMotion)
{
	if (line.find("MODEL_FILENAME") != string::npos)
	{
		// ���l��ǂݍ��ޏ���
		istringstream value_Input = pMotion->SetInputvalue(Input);

		// ���f���̖��O�i�[�p�ϐ�
		string modelName;

		// ���l��������
		value_Input >> modelName;

		// ���f���̖��O����
		const char* MODEL_NAME = modelName.c_str();

		// �T�C�Y�ȏ�ɓǂݍ��ނƃG���[���o�邽�ߐ���
		if (nCnt < nMaxSize - 1)
		{
			// ���f���̐���
			ppModel[nCnt] = CModel::Create(MODEL_NAME);

			return true;
		}
		else
		{
			MessageBox(NULL, MODEL_NAME, "����ȏ�ǂݍ��߂܂���", MB_OK);
		}
	}
	return false;
}

//===================================================
// �L�����N�^�[�̐ݒ菈��
//===================================================
bool CMotion::LoadCharacterSet(CModel** ppModel,string line, string input, CMotion* pMotion)
{
	static int nIdx = 0;
	int nNumParts = 0;
	int nParent = 0;
	D3DXVECTOR3 offset = VEC3_NULL;

	if (line.find("NUM_PARTS") != string::npos)
	{
		// ���l��ǂݍ��ޏ���
		istringstream value_Input = pMotion->SetInputvalue(input);

		// ���l��������
		value_Input >> nNumParts;

		// �p�[�c�̍ő吔��ݒ�
		m_nNumModel = nNumParts;
	}

	if (line.find("INDEX") != string::npos)
	{
		// ���l��ǂݍ��ޏ���
		istringstream value_Input = pMotion->SetInputvalue(input);

		// ���l��������
		value_Input >> nIdx;
	}

	if (line.find("PARENT") != string::npos)
	{
		// ���l��ǂݍ��ޏ���
		istringstream value_Input = pMotion->SetInputvalue(input);

		// ���l��������
		value_Input >> nParent;

		if (nParent != -1)
		{// �e�����݂��Ă�����
			// �e�̃��f���̐ݒ�
			ppModel[nIdx]->SetParent(ppModel[nParent]);
		}
		else
		{// �e�����݂��Ă��Ȃ�������
			ppModel[nIdx]->SetParent(nullptr);
		}
	}

	if (line.find("POS") != string::npos)
	{
		// ���l��ǂݍ��ޏ���
		istringstream value_Input = pMotion->SetInputvalue(input);

		// ���l��������
		value_Input >> offset.x;
		value_Input >> offset.y;
		value_Input >> offset.z;

		ppModel[nIdx]->SetOffPos(offset);
	}

	if (line.find("ROT") != string::npos)
	{
		// ���l��ǂݍ��ޏ���
		istringstream value_Input = pMotion->SetInputvalue(input);

		// ���l��������
		value_Input >> offset.x;
		value_Input >> offset.y;
		value_Input >> offset.z;

		ppModel[nIdx]->SetOffRot(offset);
	}

	if (line.find("END_CHARACTERSET") != string::npos)
	{
		return true;
	}
	
	return false;
}

//===================================================
// ���[�V�����̃��[�h����
//===================================================
void CMotion::LoadMotionSet(CMotion* pMotion, ifstream& File,string nowLine, const int nNumMotion)
{
	string line,input;

	int loop = 0;
	static int motiontype = 0;
	int nKey = 0;
	int nCntModel = 0;

	if (nowLine.find("MOTIONSET") != string::npos)
	{
		while (getline(File, line))
		{
			size_t equal_pos = line.find("="); // =�̈ʒu

			// [=] ���������߂�
			input = line.substr(equal_pos + 1);

			if (line.find("LOOP") != string::npos)
			{
				// ���l��ǂݍ��ޏ���
				istringstream value_Input = pMotion->SetInputvalue(input);

				// ���l��������
				value_Input >> loop;

				// ���[�v���邩�ǂ���
				pMotion->m_apMotionInfo[motiontype].bLoop = (loop == 1) ? true : false;
			}

			if (line.find("NUM_KEY") != string::npos)
			{
				// = ���������߂�
				input = line.substr(equal_pos + 1);

				// ���l��ǂݍ��ޏ���
				istringstream value_Input = pMotion->SetInputvalue(input);

				// ���l��������
				value_Input >> pMotion->m_apMotionInfo[motiontype].nNumkey;

				// �L�[�̑�������
				int nNumKey = pMotion->m_apMotionInfo[motiontype].nNumkey;

				m_apMotionInfo[motiontype].apKeyInfo = CMotionManager::CreateKeyInfo(nNumKey);

				for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
				{
					m_apMotionInfo[motiontype].apKeyInfo[nCntKey].apKey = CMotionManager::CreateKey(m_nNumModel);
				}
			}

			if (line.find("FRAME") != string::npos)
			{
				// = ���������߂�
				input = line.substr(equal_pos + 1);

				// ���l��ǂݍ��ޏ���
				istringstream value_Input = pMotion->SetInputvalue(input);

				// ���l��������
				value_Input >> pMotion->m_apMotionInfo[motiontype].apKeyInfo[nKey].nFrame;
			}

			if (line.find("POS") != string::npos)
			{
				// = ���������߂�
				input = line.substr(equal_pos + 1);

				// ���l��ǂݍ��ޏ���
				istringstream value_Input = pMotion->SetInputvalue(input);

				// ���l��������
				value_Input >> pMotion->m_apMotionInfo[motiontype].apKeyInfo[nKey].apKey[nCntModel].fPosX;
				value_Input >> pMotion->m_apMotionInfo[motiontype].apKeyInfo[nKey].apKey[nCntModel].fPosY;
				value_Input >> pMotion->m_apMotionInfo[motiontype].apKeyInfo[nKey].apKey[nCntModel].fPosZ;
			}
			if (line.find("ROT") != string::npos)
			{
				// = ���������߂�
				input = line.substr(equal_pos + 1);

				// ���l��ǂݍ��ޏ���
				istringstream value_Input = pMotion->SetInputvalue(input);

				// ���l��������
				value_Input >> pMotion->m_apMotionInfo[motiontype].apKeyInfo[nKey].apKey[nCntModel].fRotX;
				value_Input >> pMotion->m_apMotionInfo[motiontype].apKeyInfo[nKey].apKey[nCntModel].fRotY;
				value_Input >> pMotion->m_apMotionInfo[motiontype].apKeyInfo[nKey].apKey[nCntModel].fRotZ;
			}

			if (line.find("END_KEY") != string::npos)
			{
				nCntModel++;

				nCntModel = Clamp(nCntModel, 0, m_nNumModel - 1);
			}

			if (line.find("END_KEYSET") != string::npos)
			{
				nKey++;
				nCntModel = NULL;
			}
			if (line.find("END_MOTIONSET") != string::npos)
			{
				nKey = NULL;

				if (motiontype >= 0 && motiontype <= nNumMotion - 1)
				{
					motiontype++;
				}
				else
				{
				}

				pMotion->m_nNumMotion = motiontype;

				break;
			}
		}
	}
}

//===================================================
// �I������
//===================================================
void CMotion::Uninit(void)
{
	// �L�[�̔j��
	for (int nCntMotion = 0; nCntMotion < m_nNumMotion; nCntMotion++)
	{
		// �L�[�̔j��
		for (int nCntKey = 0; nCntKey < m_apMotionInfo[nCntMotion].nNumkey; nCntKey++)
		{
			if (m_apMotionInfo[nCntMotion].apKeyInfo[nCntKey].apKey != nullptr)
			{
				delete[] m_apMotionInfo[nCntMotion].apKeyInfo[nCntKey].apKey;
				m_apMotionInfo[nCntMotion].apKeyInfo[nCntKey].apKey = nullptr;
			}
		}
	}

	// �L�[���̔j��
	for (int nCntMotion = 0; nCntMotion < m_nNumMotion; nCntMotion++)
	{
		if (m_apMotionInfo[nCntMotion].apKeyInfo != nullptr)
		{
			delete[] m_apMotionInfo[nCntMotion].apKeyInfo;
			m_apMotionInfo[nCntMotion].apKeyInfo = nullptr;
		}
	}

	// �������̔j��
	if (m_apMotionInfo != nullptr)
	{
		delete[] m_apMotionInfo;

		m_apMotionInfo = nullptr;
	}
}

//===================================================
// �l��ǂݍ��ނ܂ł̏���
//===================================================
istringstream CMotion::SetInputvalue(std::string input)
{
	// �擪�� = ������
	input.erase(0, input.find_first_not_of(" = "));

	// input���琔�l�����o������
	istringstream value_Input(input);

	return value_Input;
}

//===================================================
// �ŏ��̃��[�V�����̃u�����h���I�������
//===================================================
void CMotion::FinishFirstBlend(void)
{
	// ���[�V�����̏o�����̃u�����h���I������
	if (m_bFirstMotion == true && m_nCounterBlend >= m_nFrameBlend && m_bFinishMotion == false)
	{
		// ���Ƃɖ߂�
		m_bFirstMotion = false;

		m_nKeyBlend = 0;
		m_nKey = 0;

		for (int nCnt = 0; nCnt < m_apMotionInfo[m_motiontypeBlend].nNumkey; nCnt++)
		{
			if (m_nCounterBlend >= m_apMotionInfo[m_motiontypeBlend].apKeyInfo[nCnt].nFrame)
			{
				// �L�[�𑝂₷
				m_nKey = (m_nKey + 1) % m_apMotionInfo[m_motiontypeBlend].nNumkey;
				m_nCounterBlend -= m_apMotionInfo[m_motiontypeBlend].apKeyInfo[nCnt].nFrame;
			}
		}

		// ���[�V�������u�����h�������[�V�����ɂ���
		m_motiontype = m_motiontypeBlend;

		m_nCountMotion = m_nCounterBlend;
	}
}

//===================================================
// �f�o�b�O
//===================================================
void CMotion::Debug(void)
{
	CDebugProc::Print("�L�[ %d / %d\n", m_nKey, m_apMotionInfo[m_motiontype].nNumkey);
	CDebugProc::Print("�u�����h�L�[%d / %d\n", m_nKeyBlend, m_apMotionInfo[m_motiontypeBlend].nNumkey);

	CDebugProc::Print("�t���[��%d\n", m_nCountMotion);
	CDebugProc::Print("�u�����h�t���[��%d\n", m_nCounterBlend);

	CDebugProc::Print("first = %d\n", m_bFirstMotion);
	CDebugProc::Print("finish = %d\n", m_bFinishMotion);

	CDebugProc::Print("���[�V���� = %d\n", m_motiontype);
	CDebugProc::Print("���[�V�����u�����h = %d\n", m_motiontypeBlend);
}

//===================================================
// ���݂̃��[�V�����̍X�V����
//===================================================
void CMotion::UpdateCurrentMotion(CModel** pModel, int nIdx)
{
	// ���̃L�[�̃A�h���X�̎擾
	Key* pKeyNext = &m_apMotionInfo[m_motiontype].apKeyInfo[m_nextKey].apKey[nIdx];

	// ���݂̃L�[�̃A�h���X�̎擾
	Key* pKey = &m_apMotionInfo[m_motiontype].apKeyInfo[m_nKey].apKey[nIdx];

	// �L�[���̃A�h���X�̎擾
	Key_Info* pKeyInfo = &m_apMotionInfo[m_motiontype].apKeyInfo[m_nKey];

	// ���̃L�[�܂ł̋��������߂�
	float fDiffPosX = pKeyNext->fPosX - pKey->fPosX;
	float fDiffPosY = pKeyNext->fPosY - pKey->fPosY;
	float fDiffPosZ = pKeyNext->fPosZ - pKey->fPosZ;

	float fDiffRotX = pKeyNext->fRotX - pKey->fRotX;
	float fDiffRotY = pKeyNext->fRotY - pKey->fRotY;
	float fDiffRotZ = pKeyNext->fRotZ - pKey->fRotZ;

	// �p�x�̐��K��
	NormalizeRot(&fDiffRotX);
	NormalizeRot(&fDiffRotY);
	NormalizeRot(&fDiffRotZ);

	// �t���[���ƍő�t���[���̊��������߂�
	float fRateFrame = (float)m_nCountMotion / (float)pKeyInfo->nFrame;

	// ���̃L�[�̈ʒu�܂Ńt���[���ɉ������ʒu��ݒ肷��
	float fPosX = LerpDiff(pKey->fPosX, fDiffPosX, fRateFrame);
	float fPosY = LerpDiff(pKey->fPosY, fDiffPosY, fRateFrame);
	float fPosZ = LerpDiff(pKey->fPosZ, fDiffPosZ, fRateFrame);

	// ���̃L�[�̌����܂Ńt���[���ɉ�����������ݒ肷��
	float fRotX = LerpDiff(pKey->fRotX, fDiffRotX, fRateFrame);
	float fRotY = LerpDiff(pKey->fRotY, fDiffRotY, fRateFrame);
	float fRotZ = LerpDiff(pKey->fRotZ, fDiffRotZ, fRateFrame);

	pModel[nIdx]->SetPosition(D3DXVECTOR3(fPosX, fPosY, fPosZ));

	pModel[nIdx]->SetRotaition(D3DXVECTOR3(fRotX, fRotY, fRotZ));
}

//===================================================
// �u�����h�̃��[�V�����̍X�V����
//===================================================
void CMotion::UpdateBlendMotion(CModel** pModel, int nIdx)
{
	// ���݂̃L�[�̃A�h���X���擾
	Key* pCurrentKey = &m_apMotionInfo[m_motiontype].apKeyInfo[m_nKey].apKey[nIdx];

	// ���̃L�[�̃A�h���X���擾
	Key* pNextKey = &m_apMotionInfo[m_motiontype].apKeyInfo[m_nextKey].apKey[nIdx];

	// �L�[���̃A�h���X�̎擾
	Key_Info* pKeyInfo = &m_apMotionInfo[m_motiontype].apKeyInfo[m_nKey];

	float fRateMotion = (float)m_nCountMotion / (float)pKeyInfo->nFrame; // ���Βl
	float fRateMotionBlend = (float)m_nCounterBlend / (float)m_apMotionInfo[m_motiontypeBlend].apKeyInfo[m_nKeyBlend].nFrame;

	float fRateBlend = (float)m_nCounterBlend / (float)m_nFrameBlend;

	// ���݂̃��[�V�����̊p�x�̍���
	float fDiffMotionRX = pNextKey->fRotX - pCurrentKey->fRotX;
	float fDiffMotionRY = pNextKey->fRotY - pCurrentKey->fRotY;
	float fDiffMotionRZ = pNextKey->fRotZ - pCurrentKey->fRotZ;

	// �p�x�̐��K��
	NormalizeRot(&fDiffMotionRX);
	NormalizeRot(&fDiffMotionRY);
	NormalizeRot(&fDiffMotionRZ);

	// ���݂̃��[�V�����̃t���[���ɉ������p�x��ݒ�
	float fRotXCurrent = LerpDiff(pCurrentKey->fRotX, fDiffMotionRX, fRateMotion);
	float fRotYCurrent = LerpDiff(pCurrentKey->fRotY, fDiffMotionRY, fRateMotion);
	float fRotZCurrent = LerpDiff(pCurrentKey->fRotZ, fDiffMotionRZ, fRateMotion);

	// �u�����h�̃L�[�̃A�h���X���擾
	Key* pCurrentBlendKey = &m_apMotionInfo[m_motiontypeBlend].apKeyInfo[m_nKeyBlend].apKey[nIdx];

	// �u�����h�̎��̃L�[�̃A�h���X���擾
	Key* pNextBlendKey = &m_apMotionInfo[m_motiontypeBlend].apKeyInfo[m_nNextKeyBlend].apKey[nIdx];

	//�u�����h���[�V�����̊p�x�̍��������߂�
	float fDiffMotionBlendRX = pNextBlendKey->fRotX - pCurrentBlendKey->fRotX;
	float fDiffMotionBlendRY = pNextBlendKey->fRotY - pCurrentBlendKey->fRotY;
	float fDiffMotionBlendRZ = pNextBlendKey->fRotZ - pCurrentBlendKey->fRotZ;

	// �p�x�̐��K��
	NormalizeRot(&fDiffMotionBlendRX);
	NormalizeRot(&fDiffMotionBlendRY);
	NormalizeRot(&fDiffMotionBlendRZ);

	// �u�����h���[�V�����̃t���[���ɉ������p�x�̐ݒ�
	float fRotXBlend = LerpDiff(pCurrentBlendKey->fRotX, fDiffMotionBlendRX, fRateMotionBlend);
	float fRotYBlend = LerpDiff(pCurrentBlendKey->fRotY, fDiffMotionBlendRY, fRateMotionBlend);
	float fRotZBlend = LerpDiff(pCurrentBlendKey->fRotZ, fDiffMotionBlendRZ, fRateMotionBlend);

	//���݂̃��[�V�����̈ʒu�̍��������߂�
	float fDiffMotionPX = pNextKey->fPosX - pCurrentKey->fPosX;
	float fDiffMotionPY = pNextKey->fPosY - pCurrentKey->fPosY;
	float fDiffMotionPZ = pNextKey->fPosZ - pCurrentKey->fPosZ;

	// ���݂̃��[�V�����̃t���[���ɉ������ʒu�̐ݒ�
	float fPosXCurrent = LerpDiff(pCurrentKey->fPosX, fDiffMotionPX, fRateMotion);
	float fPosYCurrent = LerpDiff(pCurrentKey->fPosY, fDiffMotionPY, fRateMotion);
	float fPosZCurrent = LerpDiff(pCurrentKey->fPosZ, fDiffMotionPZ, fRateMotion);

	// �u�����h���[�V�����̈ʒu�̍��������߂�
	float fDiffMotionBlendPX = pNextBlendKey->fPosX - pCurrentBlendKey->fPosX;
	float fDiffMotionBlendPY = pNextBlendKey->fPosY - pCurrentBlendKey->fPosY;
	float fDiffMotionBlendPZ = pNextBlendKey->fPosZ - pCurrentBlendKey->fPosZ;

	// �u�����h���[�V�����̃t���[���ɉ������ʒu�̐ݒ�
	float fPosXBlend = LerpDiff(pCurrentBlendKey->fPosX, fDiffMotionBlendPX, fRateMotionBlend);
	float fPosYBlend = LerpDiff(pCurrentBlendKey->fPosY, fDiffMotionBlendPY, fRateMotionBlend);
	float fPosZBlend = LerpDiff(pCurrentBlendKey->fPosZ, fDiffMotionBlendPZ, fRateMotionBlend);

	// ���݂̊p�x�ƃu�����h����p�x�̍��������߂�
	float fDiffBlendRX = fRotXBlend - fRotXCurrent;
	float fDiffBlendRY = fRotYBlend - fRotYCurrent;
	float fDiffBlendRZ = fRotZBlend - fRotZCurrent;

	// �u�����h�̃t���[���ɉ����������̐ݒ�
	float fRotX = LerpDiff(fRotXCurrent, fDiffBlendRX, fRateBlend);
	float fRotY = LerpDiff(fRotYCurrent, fDiffBlendRY, fRateBlend);
	float fRotZ = LerpDiff(fRotZCurrent, fDiffBlendRZ, fRateBlend);

	// ���݂̈ʒu�ƃu�����h����ʒu�̍��������߂�
	float fDiffBlendPX = fPosXBlend - fPosXCurrent;
	float fDiffBlendPY = fPosYBlend - fPosYCurrent;
	float fDiffBlendPZ = fPosZBlend - fPosZCurrent;

	// �u�����h�̃t���[���ɉ������ʒu�̐ݒ�
	float fPosX = LerpDiff(fPosXCurrent, fDiffBlendPX, fRateBlend);
	float fPosY = LerpDiff(fPosYCurrent, fDiffBlendPY, fRateBlend);
	float fPosZ = LerpDiff(fPosZCurrent, fDiffBlendPZ, fRateBlend);

	// �ʒu��ݒ�
	pModel[nIdx]->SetPosition(D3DXVECTOR3(fPosX, fPosY, fPosZ));

	// ������ݒ�
	pModel[nIdx]->SetRotaition(D3DXVECTOR3(fRotX, fRotY, fRotZ));
}

//===================================================
// �X�V����
//===================================================
void CMotion::Update(CModel** pModel,const int nNumModel)
{
	for (int nCntModel = 0; nCntModel < nNumModel; nCntModel++)
	{
		// �L�[�̑���
		m_nNumKey = m_apMotionInfo[m_motiontype].nNumkey;
		m_nNumKeyBlend = m_apMotionInfo[m_motiontypeBlend].nNumkey;

		// ���[�v���邩�ǂ���
		m_bLoopMotion = m_apMotionInfo[m_motiontype].bLoop;

		//if (motiontype < 0 || motiontype >= TYPE::TYPE_MAX)
		//{
		//	return;
		//}
		
		// ���̃L�[�𑝂₷
		m_nextKey = (m_nKey + 1) % m_apMotionInfo[m_motiontype].nNumkey;

		// ���̃u�����h�L�[�𑝂₷
		m_nNextKeyBlend = (m_nKeyBlend + 1) % m_nNumKeyBlend;

		if (m_bFinishMotion == false && m_bFirstMotion == false)
		{
			// ���݂̃��[�V�����̍X�V����
			UpdateCurrentMotion(pModel, nCntModel);
		}
		if ((m_bFinishMotion == true || m_bFirstMotion == true) && m_bBlendMotion == true)
		{
			// �u�����h�̃��[�V�����̍X�V����
			UpdateBlendMotion(pModel, nCntModel);
		}
	}	
	
	// ���[�V�������I��������
	if (IsEndMotion())
	{
		m_nCounterBlend = 0;
		m_bFinishMotion = true;
		m_nFrameBlend = m_apMotionInfo[m_motiontype].apKeyInfo[m_nNumKey - 1].nFrame;
		m_motiontypeBlend = NEUTRAL;
	}

	// ���[�V�����̏o�����̃u�����h���I������
	FinishFirstBlend();

	// �L�[���ő傩�u�����h�̃J�E���g���ő�ɂȂ���
	if (m_bFinishMotion == true && m_nFrameBlend <= m_nCounterBlend && m_bFirstMotion == false)
	{
		m_bFinishMotion = false;			// ���Ƃɖ߂�
		m_bBlendMotion = false;				// ���Ƃɖ߂�
		m_nCountMotion = m_nFrameBlend;	    // �t���[�����u�����h������̃t���[���ɍ��킹��
		m_motiontype = NEUTRAL;				// ���[�V�����^�C�v���j���[�g�����ɂ���
	}

	if (m_nCountMotion >= m_apMotionInfo[m_motiontype].apKeyInfo[m_nKey].nFrame)
	{
		// �L�[�𑝂₷
		m_nKey = (m_nKey + 1) % m_apMotionInfo[m_motiontype].nNumkey;

		m_nCountMotion = 0;
	}

	if (m_bFirstMotion == false)
	{
		m_nCountMotion++;
	}

	// �u�����h���n�܂�����
	if (m_bFinishMotion == true || m_bFirstMotion == true)
	{
		m_nCounterBlend++;
	}


}

//===================================================
// ���[�V�����̐ݒ菈��
//===================================================
void CMotion::SetMotion(const int motiontype,bool bBlend,const int nBlendFrame)
{
	if (m_motiontypeBlend == motiontype || m_motiontype == motiontype) return;

	if (bBlend == true)
	{
		if (m_apMotionInfo[motiontype].bLoop == false || m_bFirstMotion == false)
		{
			m_nFrameBlend = nBlendFrame;
			m_nCounterBlend = 0;
		}

		m_nKeyBlend = 0;
		m_bFirstMotion = true;
		m_bFinishMotion = false;
		m_bBlendMotion = bBlend;
		m_motiontypeBlend = motiontype;
	}
	else
	{
		m_bBlendMotion = bBlend;				// �u�����h�����邩�ǂ���
		m_motiontype = motiontype;			// �u�����h���郂�[�V�����̃^�C�v����
		m_motiontypeBlend = motiontype;		// �u�����h���郂�[�V�����̃^�C�v����
		m_bFinishMotion = false;
	}
}

//===================================================
// ���[�V�������I��������ǂ���
//===================================================
bool CMotion::IsEndMotion(void)
{
	// ���[�V�������I��������
	if (m_bFinishMotion == false &&
		m_nKey >= m_nNumKey - 1 &&
		m_bBlendMotion == true &&
		m_bLoopMotion == false &&
		m_bFirstMotion == false)
	{
		return true;
	}
	return false;
}

//===================================================
// �R���X�g���N�^
//===================================================
CMotionManager::CMotionManager()
{
	
}

//===================================================
// �f�X�g���N�^
//===================================================
CMotionManager::~CMotionManager()
{
}

//===================================================
// ���[�V�������̐���
//===================================================
CMotion::Motion_Info* CMotionManager::CreateMotionInfo(const int nNumMotion)
{
	// �g�����������[�V�������m�ۂ���
	CMotion::Motion_Info* pMotionInfo = new CMotion::Motion_Info[nNumMotion];

	return pMotionInfo;
}

//===================================================
// �L�[���̐���
//===================================================
CMotion::Key_Info* CMotionManager::CreateKeyInfo(const int nNumKey)
{
	// �g���������L�[���m�ۂ���
	CMotion::Key_Info* pKey_Info = new CMotion::Key_Info[nNumKey];

	return pKey_Info;
}

//===================================================
// �L�[�̐���
//===================================================
CMotion::Key* CMotionManager::CreateKey(const int nNumModel)
{
	// �g���������L�[���m�ۂ���
	CMotion::Key* pKey = new CMotion::Key[nNumModel];

	return pKey;
}
