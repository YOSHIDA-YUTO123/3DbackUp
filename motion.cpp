//===================================================
//
// モーション [motion.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "motion.h"
#include<stdio.h>
#include"object.h"
#include"process.h"
#include "debugproc.h"
#include "manager.h"

//***************************************************
// マクロ定義
//***************************************************
#define NEUTRAL (0) // ニュートラル

//===================================================
// コンストラクタ
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
// デストラクタ
//===================================================
CMotion::~CMotion()
{
}

//===================================================
// モーションのロード処理
//===================================================
CMotion* CMotion::Load(const char* pLoadFileName, CModel** ppModel, const int nMaxSize, int* pOutModel,const int nNumMotion)
{
	// モーションを生成
	CMotion* pMotion = new CMotion;

	// モーション情報構造体のメモリの確保
	pMotion->m_apMotionInfo = new CMotion::Motion_Info[nNumMotion];

	// ファイルをロードする
	ifstream File(pLoadFileName);
	string line;
	string input;

	int nNumModel = 0;

	D3DXVECTOR3 offset = VEC3_NULL;

	bool bCharacterSet = false;	// キャラクターの設定をしたかどうか

	if (File.is_open() == true)
	{
		// 最後の行になるまで読み込む
		while (getline(File, line))
		{
			size_t equal_pos = line.find("="); // =の位置

			// [=] から先を求める
			input = line.substr(equal_pos + 1);

			// モデルのロード処理
			if (pMotion->LoadModel(ppModel, nMaxSize, nNumModel, input, line, pMotion))
			{
				nNumModel++;
			}

			// パーツの設定が終わって無かったら
			if (bCharacterSet == false)
			{
				bCharacterSet = pMotion->LoadCharacterSet(ppModel, line,input, pMotion);
			}

			// モーションの設定の読み込み
			pMotion->LoadMotionSet(pMotion, File, line,nNumMotion);

			// モーションの数が最大まで行ったら
			if (pMotion->m_nNumMotion >= nNumMotion)
			{
				break;
			}
		}
		// ファイルを閉じる
		File.close();
	}
	else
	{
		// メモリの破棄
		if (pMotion->m_apMotionInfo != nullptr)
		{
			delete[] pMotion->m_apMotionInfo;

			pMotion->m_apMotionInfo = nullptr;
		}
		
		// メモリの破棄
		if (pMotion != nullptr)
		{
			delete pMotion;
			pMotion = nullptr;
		}

		// メッセージボックス
		MessageBox(NULL, pLoadFileName, "ファイルが開けませんでした", MB_OK);

		return nullptr;
	}

	*pOutModel = nNumModel;

	return pMotion;
}

//===================================================
// モデルのロード
//===================================================
bool CMotion::LoadModel(CModel** ppModel, const int nMaxSize, int nCnt, string Input, string line, CMotion *pMotion)
{
	if (line.find("MODEL_FILENAME") != string::npos)
	{
		// 数値を読み込む準備
		istringstream value_Input = pMotion->SetInputvalue(Input);

		// モデルの名前格納用変数
		string modelName;

		// 数値を代入する
		value_Input >> modelName;

		// モデルの名前を代入
		const char* MODEL_NAME = modelName.c_str();

		// サイズ以上に読み込むとエラーが出るため制限
		if (nCnt < nMaxSize - 1)
		{
			// モデルの生成
			ppModel[nCnt] = CModel::Create(MODEL_NAME);

			return true;
		}
		else
		{
			MessageBox(NULL, MODEL_NAME, "これ以上読み込めません", MB_OK);
		}
	}
	return false;
}

//===================================================
// キャラクターの設定処理
//===================================================
bool CMotion::LoadCharacterSet(CModel** ppModel,string line, string input, CMotion* pMotion)
{
	static int nIdx = 0;
	int nNumParts = 0;
	int nParent = 0;
	D3DXVECTOR3 offset = VEC3_NULL;

	if (line.find("NUM_PARTS") != string::npos)
	{
		// 数値を読み込む準備
		istringstream value_Input = pMotion->SetInputvalue(input);

		// 数値を代入する
		value_Input >> nNumParts;

		// パーツの最大数を設定
		m_nNumModel = nNumParts;
	}

	if (line.find("INDEX") != string::npos)
	{
		// 数値を読み込む準備
		istringstream value_Input = pMotion->SetInputvalue(input);

		// 数値を代入する
		value_Input >> nIdx;
	}

	if (line.find("PARENT") != string::npos)
	{
		// 数値を読み込む準備
		istringstream value_Input = pMotion->SetInputvalue(input);

		// 数値を代入する
		value_Input >> nParent;

		if (nParent != -1)
		{// 親が存在していたら
			// 親のモデルの設定
			ppModel[nIdx]->SetParent(ppModel[nParent]);
		}
		else
		{// 親が存在していなかったら
			ppModel[nIdx]->SetParent(nullptr);
		}
	}

	if (line.find("POS") != string::npos)
	{
		// 数値を読み込む準備
		istringstream value_Input = pMotion->SetInputvalue(input);

		// 数値を代入する
		value_Input >> offset.x;
		value_Input >> offset.y;
		value_Input >> offset.z;

		ppModel[nIdx]->SetOffPos(offset);
	}

	if (line.find("ROT") != string::npos)
	{
		// 数値を読み込む準備
		istringstream value_Input = pMotion->SetInputvalue(input);

		// 数値を代入する
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
// モーションのロード処理
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
			size_t equal_pos = line.find("="); // =の位置

			// [=] から先を求める
			input = line.substr(equal_pos + 1);

			if (line.find("LOOP") != string::npos)
			{
				// 数値を読み込む準備
				istringstream value_Input = pMotion->SetInputvalue(input);

				// 数値を代入する
				value_Input >> loop;

				// ループするかどうか
				pMotion->m_apMotionInfo[motiontype].bLoop = (loop == 1) ? true : false;
			}

			if (line.find("NUM_KEY") != string::npos)
			{
				// = から先を求める
				input = line.substr(equal_pos + 1);

				// 数値を読み込む準備
				istringstream value_Input = pMotion->SetInputvalue(input);

				// 数値を代入する
				value_Input >> pMotion->m_apMotionInfo[motiontype].nNumkey;

				// キーの総数を代入
				int nNumKey = pMotion->m_apMotionInfo[motiontype].nNumkey;

				m_apMotionInfo[motiontype].apKeyInfo = CMotionManager::CreateKeyInfo(nNumKey);

				for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
				{
					m_apMotionInfo[motiontype].apKeyInfo[nCntKey].apKey = CMotionManager::CreateKey(m_nNumModel);
				}
			}

			if (line.find("FRAME") != string::npos)
			{
				// = から先を求める
				input = line.substr(equal_pos + 1);

				// 数値を読み込む準備
				istringstream value_Input = pMotion->SetInputvalue(input);

				// 数値を代入する
				value_Input >> pMotion->m_apMotionInfo[motiontype].apKeyInfo[nKey].nFrame;
			}

			if (line.find("POS") != string::npos)
			{
				// = から先を求める
				input = line.substr(equal_pos + 1);

				// 数値を読み込む準備
				istringstream value_Input = pMotion->SetInputvalue(input);

				// 数値を代入する
				value_Input >> pMotion->m_apMotionInfo[motiontype].apKeyInfo[nKey].apKey[nCntModel].fPosX;
				value_Input >> pMotion->m_apMotionInfo[motiontype].apKeyInfo[nKey].apKey[nCntModel].fPosY;
				value_Input >> pMotion->m_apMotionInfo[motiontype].apKeyInfo[nKey].apKey[nCntModel].fPosZ;
			}
			if (line.find("ROT") != string::npos)
			{
				// = から先を求める
				input = line.substr(equal_pos + 1);

				// 数値を読み込む準備
				istringstream value_Input = pMotion->SetInputvalue(input);

				// 数値を代入する
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
// 終了処理
//===================================================
void CMotion::Uninit(void)
{
	// キーの破棄
	for (int nCntMotion = 0; nCntMotion < m_nNumMotion; nCntMotion++)
	{
		// キーの破棄
		for (int nCntKey = 0; nCntKey < m_apMotionInfo[nCntMotion].nNumkey; nCntKey++)
		{
			if (m_apMotionInfo[nCntMotion].apKeyInfo[nCntKey].apKey != nullptr)
			{
				delete[] m_apMotionInfo[nCntMotion].apKeyInfo[nCntKey].apKey;
				m_apMotionInfo[nCntMotion].apKeyInfo[nCntKey].apKey = nullptr;
			}
		}
	}

	// キー情報の破棄
	for (int nCntMotion = 0; nCntMotion < m_nNumMotion; nCntMotion++)
	{
		if (m_apMotionInfo[nCntMotion].apKeyInfo != nullptr)
		{
			delete[] m_apMotionInfo[nCntMotion].apKeyInfo;
			m_apMotionInfo[nCntMotion].apKeyInfo = nullptr;
		}
	}

	// メモリの破棄
	if (m_apMotionInfo != nullptr)
	{
		delete[] m_apMotionInfo;

		m_apMotionInfo = nullptr;
	}
}

//===================================================
// 値を読み込むまでの準備
//===================================================
istringstream CMotion::SetInputvalue(std::string input)
{
	// 先頭の = を消す
	input.erase(0, input.find_first_not_of(" = "));

	// inputから数値を取り出す準備
	istringstream value_Input(input);

	return value_Input;
}

//===================================================
// 最初のモーションのブレンドが終わったら
//===================================================
void CMotion::FinishFirstBlend(void)
{
	// モーションの出だしのブレンドが終了した
	if (m_bFirstMotion == true && m_nCounterBlend >= m_nFrameBlend && m_bFinishMotion == false)
	{
		// もとに戻す
		m_bFirstMotion = false;

		m_nKeyBlend = 0;
		m_nKey = 0;

		for (int nCnt = 0; nCnt < m_apMotionInfo[m_motiontypeBlend].nNumkey; nCnt++)
		{
			if (m_nCounterBlend >= m_apMotionInfo[m_motiontypeBlend].apKeyInfo[nCnt].nFrame)
			{
				// キーを増やす
				m_nKey = (m_nKey + 1) % m_apMotionInfo[m_motiontypeBlend].nNumkey;
				m_nCounterBlend -= m_apMotionInfo[m_motiontypeBlend].apKeyInfo[nCnt].nFrame;
			}
		}

		// モーションをブレンドしたモーションにする
		m_motiontype = m_motiontypeBlend;

		m_nCountMotion = m_nCounterBlend;
	}
}

//===================================================
// デバッグ
//===================================================
void CMotion::Debug(void)
{
	CDebugProc::Print("キー %d / %d\n", m_nKey, m_apMotionInfo[m_motiontype].nNumkey);
	CDebugProc::Print("ブレンドキー%d / %d\n", m_nKeyBlend, m_apMotionInfo[m_motiontypeBlend].nNumkey);

	CDebugProc::Print("フレーム%d\n", m_nCountMotion);
	CDebugProc::Print("ブレンドフレーム%d\n", m_nCounterBlend);

	CDebugProc::Print("first = %d\n", m_bFirstMotion);
	CDebugProc::Print("finish = %d\n", m_bFinishMotion);

	CDebugProc::Print("モーション = %d\n", m_motiontype);
	CDebugProc::Print("モーションブレンド = %d\n", m_motiontypeBlend);
}

//===================================================
// 現在のモーションの更新処理
//===================================================
void CMotion::UpdateCurrentMotion(CModel** pModel, int nIdx)
{
	// 次のキーのアドレスの取得
	Key* pKeyNext = &m_apMotionInfo[m_motiontype].apKeyInfo[m_nextKey].apKey[nIdx];

	// 現在のキーのアドレスの取得
	Key* pKey = &m_apMotionInfo[m_motiontype].apKeyInfo[m_nKey].apKey[nIdx];

	// キー情報のアドレスの取得
	Key_Info* pKeyInfo = &m_apMotionInfo[m_motiontype].apKeyInfo[m_nKey];

	// 次のキーまでの距離を求める
	float fDiffPosX = pKeyNext->fPosX - pKey->fPosX;
	float fDiffPosY = pKeyNext->fPosY - pKey->fPosY;
	float fDiffPosZ = pKeyNext->fPosZ - pKey->fPosZ;

	float fDiffRotX = pKeyNext->fRotX - pKey->fRotX;
	float fDiffRotY = pKeyNext->fRotY - pKey->fRotY;
	float fDiffRotZ = pKeyNext->fRotZ - pKey->fRotZ;

	// 角度の正規化
	NormalizeRot(&fDiffRotX);
	NormalizeRot(&fDiffRotY);
	NormalizeRot(&fDiffRotZ);

	// フレームと最大フレームの割合を求める
	float fRateFrame = (float)m_nCountMotion / (float)pKeyInfo->nFrame;

	// 次のキーの位置までフレームに応じた位置を設定する
	float fPosX = LerpDiff(pKey->fPosX, fDiffPosX, fRateFrame);
	float fPosY = LerpDiff(pKey->fPosY, fDiffPosY, fRateFrame);
	float fPosZ = LerpDiff(pKey->fPosZ, fDiffPosZ, fRateFrame);

	// 次のキーの向きまでフレームに応じた向きを設定する
	float fRotX = LerpDiff(pKey->fRotX, fDiffRotX, fRateFrame);
	float fRotY = LerpDiff(pKey->fRotY, fDiffRotY, fRateFrame);
	float fRotZ = LerpDiff(pKey->fRotZ, fDiffRotZ, fRateFrame);

	pModel[nIdx]->SetPosition(D3DXVECTOR3(fPosX, fPosY, fPosZ));

	pModel[nIdx]->SetRotaition(D3DXVECTOR3(fRotX, fRotY, fRotZ));
}

//===================================================
// ブレンドのモーションの更新処理
//===================================================
void CMotion::UpdateBlendMotion(CModel** pModel, int nIdx)
{
	// 現在のキーのアドレスを取得
	Key* pCurrentKey = &m_apMotionInfo[m_motiontype].apKeyInfo[m_nKey].apKey[nIdx];

	// 次のキーのアドレスを取得
	Key* pNextKey = &m_apMotionInfo[m_motiontype].apKeyInfo[m_nextKey].apKey[nIdx];

	// キー情報のアドレスの取得
	Key_Info* pKeyInfo = &m_apMotionInfo[m_motiontype].apKeyInfo[m_nKey];

	float fRateMotion = (float)m_nCountMotion / (float)pKeyInfo->nFrame; // 相対値
	float fRateMotionBlend = (float)m_nCounterBlend / (float)m_apMotionInfo[m_motiontypeBlend].apKeyInfo[m_nKeyBlend].nFrame;

	float fRateBlend = (float)m_nCounterBlend / (float)m_nFrameBlend;

	// 現在のモーションの角度の差分
	float fDiffMotionRX = pNextKey->fRotX - pCurrentKey->fRotX;
	float fDiffMotionRY = pNextKey->fRotY - pCurrentKey->fRotY;
	float fDiffMotionRZ = pNextKey->fRotZ - pCurrentKey->fRotZ;

	// 角度の正規化
	NormalizeRot(&fDiffMotionRX);
	NormalizeRot(&fDiffMotionRY);
	NormalizeRot(&fDiffMotionRZ);

	// 現在のモーションのフレームに応じた角度を設定
	float fRotXCurrent = LerpDiff(pCurrentKey->fRotX, fDiffMotionRX, fRateMotion);
	float fRotYCurrent = LerpDiff(pCurrentKey->fRotY, fDiffMotionRY, fRateMotion);
	float fRotZCurrent = LerpDiff(pCurrentKey->fRotZ, fDiffMotionRZ, fRateMotion);

	// ブレンドのキーのアドレスを取得
	Key* pCurrentBlendKey = &m_apMotionInfo[m_motiontypeBlend].apKeyInfo[m_nKeyBlend].apKey[nIdx];

	// ブレンドの次のキーのアドレスを取得
	Key* pNextBlendKey = &m_apMotionInfo[m_motiontypeBlend].apKeyInfo[m_nNextKeyBlend].apKey[nIdx];

	//ブレンドモーションの角度の差分を求める
	float fDiffMotionBlendRX = pNextBlendKey->fRotX - pCurrentBlendKey->fRotX;
	float fDiffMotionBlendRY = pNextBlendKey->fRotY - pCurrentBlendKey->fRotY;
	float fDiffMotionBlendRZ = pNextBlendKey->fRotZ - pCurrentBlendKey->fRotZ;

	// 角度の正規化
	NormalizeRot(&fDiffMotionBlendRX);
	NormalizeRot(&fDiffMotionBlendRY);
	NormalizeRot(&fDiffMotionBlendRZ);

	// ブレンドモーションのフレームに応じた角度の設定
	float fRotXBlend = LerpDiff(pCurrentBlendKey->fRotX, fDiffMotionBlendRX, fRateMotionBlend);
	float fRotYBlend = LerpDiff(pCurrentBlendKey->fRotY, fDiffMotionBlendRY, fRateMotionBlend);
	float fRotZBlend = LerpDiff(pCurrentBlendKey->fRotZ, fDiffMotionBlendRZ, fRateMotionBlend);

	//現在のモーションの位置の差分を求める
	float fDiffMotionPX = pNextKey->fPosX - pCurrentKey->fPosX;
	float fDiffMotionPY = pNextKey->fPosY - pCurrentKey->fPosY;
	float fDiffMotionPZ = pNextKey->fPosZ - pCurrentKey->fPosZ;

	// 現在のモーションのフレームに応じた位置の設定
	float fPosXCurrent = LerpDiff(pCurrentKey->fPosX, fDiffMotionPX, fRateMotion);
	float fPosYCurrent = LerpDiff(pCurrentKey->fPosY, fDiffMotionPY, fRateMotion);
	float fPosZCurrent = LerpDiff(pCurrentKey->fPosZ, fDiffMotionPZ, fRateMotion);

	// ブレンドモーションの位置の差分を求める
	float fDiffMotionBlendPX = pNextBlendKey->fPosX - pCurrentBlendKey->fPosX;
	float fDiffMotionBlendPY = pNextBlendKey->fPosY - pCurrentBlendKey->fPosY;
	float fDiffMotionBlendPZ = pNextBlendKey->fPosZ - pCurrentBlendKey->fPosZ;

	// ブレンドモーションのフレームに応じた位置の設定
	float fPosXBlend = LerpDiff(pCurrentBlendKey->fPosX, fDiffMotionBlendPX, fRateMotionBlend);
	float fPosYBlend = LerpDiff(pCurrentBlendKey->fPosY, fDiffMotionBlendPY, fRateMotionBlend);
	float fPosZBlend = LerpDiff(pCurrentBlendKey->fPosZ, fDiffMotionBlendPZ, fRateMotionBlend);

	// 現在の角度とブレンドする角度の差分を求める
	float fDiffBlendRX = fRotXBlend - fRotXCurrent;
	float fDiffBlendRY = fRotYBlend - fRotYCurrent;
	float fDiffBlendRZ = fRotZBlend - fRotZCurrent;

	// ブレンドのフレームに応じた向きの設定
	float fRotX = LerpDiff(fRotXCurrent, fDiffBlendRX, fRateBlend);
	float fRotY = LerpDiff(fRotYCurrent, fDiffBlendRY, fRateBlend);
	float fRotZ = LerpDiff(fRotZCurrent, fDiffBlendRZ, fRateBlend);

	// 現在の位置とブレンドする位置の差分を求める
	float fDiffBlendPX = fPosXBlend - fPosXCurrent;
	float fDiffBlendPY = fPosYBlend - fPosYCurrent;
	float fDiffBlendPZ = fPosZBlend - fPosZCurrent;

	// ブレンドのフレームに応じた位置の設定
	float fPosX = LerpDiff(fPosXCurrent, fDiffBlendPX, fRateBlend);
	float fPosY = LerpDiff(fPosYCurrent, fDiffBlendPY, fRateBlend);
	float fPosZ = LerpDiff(fPosZCurrent, fDiffBlendPZ, fRateBlend);

	// 位置を設定
	pModel[nIdx]->SetPosition(D3DXVECTOR3(fPosX, fPosY, fPosZ));

	// 向きを設定
	pModel[nIdx]->SetRotaition(D3DXVECTOR3(fRotX, fRotY, fRotZ));
}

//===================================================
// 更新処理
//===================================================
void CMotion::Update(CModel** pModel,const int nNumModel)
{
	for (int nCntModel = 0; nCntModel < nNumModel; nCntModel++)
	{
		// キーの総数
		m_nNumKey = m_apMotionInfo[m_motiontype].nNumkey;
		m_nNumKeyBlend = m_apMotionInfo[m_motiontypeBlend].nNumkey;

		// ループするかどうか
		m_bLoopMotion = m_apMotionInfo[m_motiontype].bLoop;

		//if (motiontype < 0 || motiontype >= TYPE::TYPE_MAX)
		//{
		//	return;
		//}
		
		// 次のキーを増やす
		m_nextKey = (m_nKey + 1) % m_apMotionInfo[m_motiontype].nNumkey;

		// 次のブレンドキーを増やす
		m_nNextKeyBlend = (m_nKeyBlend + 1) % m_nNumKeyBlend;

		if (m_bFinishMotion == false && m_bFirstMotion == false)
		{
			// 現在のモーションの更新処理
			UpdateCurrentMotion(pModel, nCntModel);
		}
		if ((m_bFinishMotion == true || m_bFirstMotion == true) && m_bBlendMotion == true)
		{
			// ブレンドのモーションの更新処理
			UpdateBlendMotion(pModel, nCntModel);
		}
	}	
	
	// モーションが終了したら
	if (IsEndMotion())
	{
		m_nCounterBlend = 0;
		m_bFinishMotion = true;
		m_nFrameBlend = m_apMotionInfo[m_motiontype].apKeyInfo[m_nNumKey - 1].nFrame;
		m_motiontypeBlend = NEUTRAL;
	}

	// モーションの出だしのブレンドが終了した
	FinishFirstBlend();

	// キーが最大かつブレンドのカウントが最大になった
	if (m_bFinishMotion == true && m_nFrameBlend <= m_nCounterBlend && m_bFirstMotion == false)
	{
		m_bFinishMotion = false;			// もとに戻す
		m_bBlendMotion = false;				// もとに戻す
		m_nCountMotion = m_nFrameBlend;	    // フレームをブレンドした先のフレームに合わせる
		m_motiontype = NEUTRAL;				// モーションタイプをニュートラルにする
	}

	if (m_nCountMotion >= m_apMotionInfo[m_motiontype].apKeyInfo[m_nKey].nFrame)
	{
		// キーを増やす
		m_nKey = (m_nKey + 1) % m_apMotionInfo[m_motiontype].nNumkey;

		m_nCountMotion = 0;
	}

	if (m_bFirstMotion == false)
	{
		m_nCountMotion++;
	}

	// ブレンドが始まったら
	if (m_bFinishMotion == true || m_bFirstMotion == true)
	{
		m_nCounterBlend++;
	}


}

//===================================================
// モーションの設定処理
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
		m_bBlendMotion = bBlend;				// ブレンドがあるかどうか
		m_motiontype = motiontype;			// ブレンドするモーションのタイプを代入
		m_motiontypeBlend = motiontype;		// ブレンドするモーションのタイプを代入
		m_bFinishMotion = false;
	}
}

//===================================================
// モーションが終わったかどうか
//===================================================
bool CMotion::IsEndMotion(void)
{
	// モーションが終了したら
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
// コンストラクタ
//===================================================
CMotionManager::CMotionManager()
{
	
}

//===================================================
// デストラクタ
//===================================================
CMotionManager::~CMotionManager()
{
}

//===================================================
// モーション情報の生成
//===================================================
CMotion::Motion_Info* CMotionManager::CreateMotionInfo(const int nNumMotion)
{
	// 使う分だけモーションを確保する
	CMotion::Motion_Info* pMotionInfo = new CMotion::Motion_Info[nNumMotion];

	return pMotionInfo;
}

//===================================================
// キー情報の生成
//===================================================
CMotion::Key_Info* CMotionManager::CreateKeyInfo(const int nNumKey)
{
	// 使う分だけキーを確保する
	CMotion::Key_Info* pKey_Info = new CMotion::Key_Info[nNumKey];

	return pKey_Info;
}

//===================================================
// キーの生成
//===================================================
CMotion::Key* CMotionManager::CreateKey(const int nNumModel)
{
	// 使う分だけキーを確保する
	CMotion::Key* pKey = new CMotion::Key[nNumModel];

	return pKey;
}
