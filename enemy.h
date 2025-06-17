//===================================================
//
// プレイヤー [player.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include"main.h"
#include"character3D.h"
#include"score.h"
#include "motion.h"

//***************************************************
// マクロ定義
//***************************************************
#define NUM_PARTS (15) // モデルの最大数

//***************************************************
// 前方宣言
//***************************************************

//***************************************************
// プレイヤークラスの定義
//***************************************************
class CPlayer : public CCharacter3D
{
public:

	// モーションの種類
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

	CMotion* m_pMotion;				// モーションのクラスへのポインタ
	CScoreLerper* m_pScore;			// スコアクラスへのポインタ
	CModel* m_apModel[NUM_PARTS];	// モデルクラスへのポインタ
	D3DXVECTOR3 m_move;				// 移動量
	float m_fSpeed;					// 移動速度
	int m_nNumModel;				// モデルの最大数
	bool m_bJump;					// ジャンプできるかどうか
};

#endif