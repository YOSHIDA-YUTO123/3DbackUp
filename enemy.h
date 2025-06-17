//===================================================
//
// �v���C���[ [player.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// ���d�C���N���[�h�h�~
//***************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include"main.h"
#include"character3D.h"
#include"score.h"
#include "motion.h"

//***************************************************
// �}�N����`
//***************************************************
#define NUM_PARTS (15) // ���f���̍ő吔

//***************************************************
// �O���錾
//***************************************************

//***************************************************
// �v���C���[�N���X�̒�`
//***************************************************
class CPlayer : public CCharacter3D
{
public:

	// ���[�V�����̎��
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,
		MOTIONTYPE_MOVE,
		MOTIONTYPE_ACTION,
		MOTIONTYPE_JUMP,
		MOTIONTYPE_LANDING,
		MOTIONTYPE_MAX
	}MOTIONTYPE;

	CPlayer(int nPriority = 3);
	~CPlayer();

	static CPlayer* Create(const D3DXVECTOR3 pos = VEC3_NULL, const D3DXVECTOR3 rot = VEC3_NULL);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
private:
	void TransitionMotion(void);

	CMotion* m_pMotion;				// ���[�V�����̃N���X�ւ̃|�C���^
	CScoreLerper* m_pScore;			// �X�R�A�N���X�ւ̃|�C���^
	CModel* m_apModel[NUM_PARTS];	// ���f���N���X�ւ̃|�C���^
	D3DXVECTOR3 m_move;				// �ړ���
	float m_fSpeed;					// �ړ����x
	int m_nNumModel;				// ���f���̍ő吔
	bool m_bJump;					// �W�����v�ł��邩�ǂ���
};

#endif