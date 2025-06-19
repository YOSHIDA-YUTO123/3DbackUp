//===================================================
//
// モーション [motion.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MOTION_H_
#define _MOTION_H_

//***************************************************
// インクルードファイル
//***************************************************
#include"main.h"
#include"model.h"
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

using namespace std;

//***************************************************
// マクロ定義
//***************************************************
#define MAX_PARTS (56) // パーツの最大数
#define MAX_KEY (20)   // キーの最大数

//***************************************************
// 前方宣言
//***************************************************
class CModel;

//***************************************************
// モーションクラスの定義
//***************************************************
class CMotion
{
public:

	// キーの構造体
	struct Key
	{
		float fPosX; // 位置X
		float fPosY; // 位置Y
		float fPosZ; // 位置Z

		float fRotX; // 向きX
		float fRotY; // 向きY
		float fRotZ; // 向きZ

	};

	// キー情報の構造体
	struct Key_Info
	{
		int nFrame;							// 再生フレーム
		Key *apKey;							// 各キーパーツのキー要素
	};

	// モーション情報の構造体
	struct Motion_Info
	{
		bool bLoop;							// ループするかどうか
		int nNumkey;						// キーの総数
		Key_Info *apKeyInfo;				// キー情報
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

	Motion_Info *m_apMotionInfo;		// モーション情報へのポインタ
	int m_motiontype;					// モーションの種類
	int m_nNumKey;						// キーの総数
	int m_nNumModel;					// モデルの最大数
	int m_nKey;							// 現在のキーNo.
	int m_nCountMotion;					// モーションのカウンター
	int m_nextKey;						// 次のキー
	int m_nNumMotion;					// モーションの総数
	bool m_bLoopMotion;					// ループするかどうか

	bool m_bBlendMotion;				// ブレンドがあるかどうか
	bool m_bLoopMotionBlend;			// ループするかどうか
	bool m_bFinishMotion;				// モーションが終わったかどうか
	bool m_bFirstMotion;                // モーションが始まったフラグ

	int m_motiontypeBlend;				// ブレンドの種類
	int m_nFrameBlend;					// ブレンドのフレーム数
	int m_nCounterBlend;				// ブレンドカウンター
	int m_nNumKeyBlend;					// ブレンドモーションの最大のキー
	int m_nKeyBlend;					// ブレンドモーションの現在のキー
	int m_nNextKeyBlend;				// ブレンドモーションの次のキー
	int m_nCounterMotionBlend;			// ブレンドのカウンター
};

//***************************************************
// モーションのマネージャクラスの定義
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