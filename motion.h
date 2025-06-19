//===================================================
//
// ���[�V���� [motion.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _MOTION_H_
#define _MOTION_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include"main.h"
#include"model.h"
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

using namespace std;

//***************************************************
// �}�N����`
//***************************************************
#define MAX_PARTS (56) // �p�[�c�̍ő吔
#define MAX_KEY (20)   // �L�[�̍ő吔

//***************************************************
// �O���錾
//***************************************************
class CModel;

//***************************************************
// ���[�V�����N���X�̒�`
//***************************************************
class CMotion
{
public:

	// �L�[�̍\����
	struct Key
	{
		float fPosX; // �ʒuX
		float fPosY; // �ʒuY
		float fPosZ; // �ʒuZ

		float fRotX; // ����X
		float fRotY; // ����Y
		float fRotZ; // ����Z

	};

	// �L�[���̍\����
	struct Key_Info
	{
		int nFrame;							// �Đ��t���[��
		Key *apKey;							// �e�L�[�p�[�c�̃L�[�v�f
	};

	// ���[�V�������̍\����
	struct Motion_Info
	{
		bool bLoop;							// ���[�v���邩�ǂ���
		int nNumkey;						// �L�[�̑���
		Key_Info *apKeyInfo;				// �L�[���
	};

	CMotion();
	~CMotion();

	static CMotion* Load(const char* pLoadFileName, CModel** ppModel, const int nMaxSize, int* pOutModel,const int nNumMotion);
	void Uninit(void);
	void Update(CModel** pModel, const int nNumModel);
	void SetMotion(const int motiontype, bool bBlend, const int nBlendFrame);
	bool IsEndMotion(void);
	bool IsFinishEndBlend(void);
	int GetMotionType(void) const { return m_motiontype; }
	int GetBlendMotionType(void) const { return m_motiontypeBlend; }
	void Debug(void);

private:
	bool LoadModel(CModel** ppModel, const int nMaxSize, int nCnt, string input, string line, CMotion* pMotion);
	bool LoadCharacterSet(CModel** ppModel, string line, string input, CMotion* pMotion);
	void LoadMotionSet(CMotion* pMotion, ifstream& File, string nowLine,const int nNumMotion);
	void UpdateCurrentMotion(CModel** pModel, int nIdx);
	void UpdateBlendMotion(CModel** pModel, int nIdx);
	istringstream SetInputvalue(string input);
	void FinishFirstBlend(void);

	Motion_Info *m_apMotionInfo;		// ���[�V�������ւ̃|�C���^
	int m_motiontype;					// ���[�V�����̎��
	int m_nNumKey;						// �L�[�̑���
	int m_nNumModel;					// ���f���̍ő吔
	int m_nKey;							// ���݂̃L�[No.
	int m_nCountMotion;					// ���[�V�����̃J�E���^�[
	int m_nextKey;						// ���̃L�[
	int m_nNumMotion;					// ���[�V�����̑���
	bool m_bLoopMotion;					// ���[�v���邩�ǂ���

	bool m_bBlendMotion;				// �u�����h�����邩�ǂ���
	bool m_bLoopMotionBlend;			// ���[�v���邩�ǂ���
	bool m_bFinishMotion;				// ���[�V�������I��������ǂ���
	bool m_bFirstMotion;                // ���[�V�������n�܂����t���O

	int m_motiontypeBlend;				// �u�����h�̎��
	int m_nFrameBlend;					// �u�����h�̃t���[����
	int m_nCounterBlend;				// �u�����h�J�E���^�[
	int m_nNumKeyBlend;					// �u�����h���[�V�����̍ő�̃L�[
	int m_nKeyBlend;					// �u�����h���[�V�����̌��݂̃L�[
	int m_nNextKeyBlend;				// �u�����h���[�V�����̎��̃L�[
	int m_nCounterMotionBlend;			// �u�����h�̃J�E���^�[
};

//***************************************************
// ���[�V�����̃}�l�[�W���N���X�̒�`
//***************************************************
class CMotionManager
{
public:
	typedef enum
	{
		CATEGORY_PLAYER = 0,
		CATEGORY_MAX
	}CATEGORY;

	CMotionManager();
	~CMotionManager();

	static CMotion::Motion_Info* CreateMotionInfo(const int nNumMotion);
	static CMotion::Key_Info *CreateKeyInfo(const int nNumKey);
	static CMotion::Key* CreateKey(const int nNumModel);

private:
};
#endif