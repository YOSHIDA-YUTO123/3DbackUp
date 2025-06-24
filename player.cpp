//===================================================
//
// プレイヤー [player.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "player.h"
#include "input.h"
#include "manager.h"
#include "effect.h"
#include "particle.h"
#include"model.h"
#include"renderer.h"
#include "impact.h"
#include"motion.h"
#include"score.h"
#include"shadow.h"
#include "explosion.h"
#include "dust.h"

//***************************************************
// マクロ定義
//***************************************************
#define PLAYER_JUMP_HEIGHT (25.0f)  // ジャンプ量
#define MOVE_SPEED (10.5f)			// 移動速度
#define SHADOW_SIZE (100.0f)		// 影の大きさ
#define SHADOW_MAX_HEIGHT (500.0f)  // 影が見える最大の高さ
#define SHADOW_A_LEVEL (0.9f)       // 影のアルファ値のオフセット

//===================================================
// コンストラクタ
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
	m_bDash = false;
}

//===================================================
// デストラクタ
//===================================================
CPlayer::~CPlayer()
{
}

//===================================================
// 初期化処理
//===================================================
HRESULT CPlayer::Init(void)
{
	// モーションのロード処理
	m_pMotion = CMotion::Load("data/MOTION/motionPlayer.txt", &m_apModel[0], NUM_PARTS, &m_nNumModel, MOTIONTYPE_MAX, CMotion::LOAD_TEXT);

	// スコアの生成
	m_pScore = (CScoreLerper*)CScore::Create(CScore::TYPE_LERPER,D3DXVECTOR3(1150.0f, 50.0f, 0.0f), 180.0f, 30.0f);

	m_pShadow = CShadow::Create(VEC3_NULL, 50.0f, 50.0f, WHITE);

	m_fSpeed = MOVE_SPEED;
	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CPlayer::Uninit(void)
{
	for (int nCnt = 0; nCnt < NUM_PARTS; nCnt++)
	{
		// モデルの破棄
		if (m_apModel[nCnt] != nullptr)
		{
			// 終了処理
			m_apModel[nCnt]->Uninit();

			delete m_apModel[nCnt];

			m_apModel[nCnt] = nullptr;
		}
	}

	// モーションの終了処理
	m_pMotion->Uninit();

	// モーションの破棄
	if (m_pMotion != nullptr)
	{
		delete m_pMotion;

		m_pMotion = nullptr;
	}

	// スコアの解放
	if (m_pScore != nullptr)
	{
		m_pScore = nullptr;
	}

	CCharacter3D::Uninit();
}

//===================================================
// 更新処理
//===================================================
void CPlayer::Update(void)
{
	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// コントローラーの取得
	CInputJoypad* pJoypad = CManager::GetInputJoypad();

	// メッシュフィールドの取得
	CMeshField* pMesh = CManager::GetMeshField();

	// カメラの取得処理
	CCamera* pCamera = CManager::GetCamera();

	// 移動処理
	if (pJoypad->GetJoyStickL() == true)
	{
		// パッドの移動処理
		CPlayer::MoveJoypad(pJoypad);
	}
	else
	{
		// キーボードの移動処理
		if(CPlayer::MoveKeyboard(pKeyboard))
		{
			// ダッシュモーションか歩きモーションかを判定
			int isDashMotion = (m_bDash ? MOTIONTYPE_DASH : MOTIONTYPE_MOVE);

			// ジャンプかjumpじゃないかを判定
			int motiontype = m_bJump ? isDashMotion : MOTIONTYPE_JUMP;

			// モーションの設定
			m_pMotion->SetMotion(motiontype, true, 5);
		}
	}

	if (pKeyboard->GetPress(DIK_LSHIFT) || pJoypad->GetPress(pJoypad->JOYKEY_RIGHT_SHOULDER))
	{
		m_bDash = true;
	}
	else
	{
		m_bDash = false;
	}
	// 位置の取得
	D3DXVECTOR3 pos = GetPosition();

	// 移動量の減衰
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

	// 位置の更新
	pos += m_move;

	float fHeight = 0.0f;

	// メッシュフィールドの当たり判定
	if (pMesh->Collision(pos,&fHeight))
	{
		pos.y = fHeight;

		m_bJump = true;
		
		if (m_pMotion->GetBlendMotionType() == MOTIONTYPE_JUMP) {

			m_pMotion->SetMotion(MOTIONTYPE_LANDING, true, 5);
			CMeshCircle::Create(pos, 18, 1, 10.0f, 80.0f, 10.0f, 60, D3DXCOLOR(1.0f, 1.0f, 0.5f, 1.0f));
		}
	}
	else
	{
		m_bJump = false;
	}

	// 重力を加算
	m_move.y += -MAX_GLABITY;

	if (m_pShadow != nullptr)
	{
		pMesh = CManager::GetMeshField();

		D3DXVECTOR3 FieldNor = pMesh->GetNor(); 				// 地面の法線ベクトルの取得
		D3DXVECTOR3 PlayerRay = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  // 上方向ベクトルの作成

		// 地面の角度に合わせた角度を取得
		D3DXVECTOR3 rot = m_pShadow->GetFieldAngle(FieldNor, PlayerRay);

		// 影の設定処理
		m_pShadow->Setting(D3DXVECTOR3(pos.x,pos.y - fHeight,pos.z),D3DXVECTOR3(pos.x, fHeight + 2.0f, pos.z), SHADOW_SIZE, SHADOW_SIZE, SHADOW_MAX_HEIGHT,SHADOW_A_LEVEL);

		m_pShadow->SetRotaition(rot);
	}


	// ジャンプできるなら
	if ((pKeyboard->GetPress(DIK_SPACE) == true || pJoypad->GetPress(pJoypad->JOYKEY_A) == true) && m_bJump == true)
	{
		m_pMotion->SetMotion(MOTIONTYPE_JUMP, true, 5);

		// 移動量を上方向に設定
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

	if (pKeyboard->GetTrigger(DIK_V))
	{
		pMesh->SetWave(pos, 120, 15.0f, 20.0f, 300.0f, 380.0f,0.01f);
	}

	if (pKeyboard->GetTrigger(DIK_R) || pJoypad->GetTrigger(pJoypad->JOYKEY_RIGHT_THUMB))
	{
		CCamera::STATE state; // カメラの状態

		// カメラの状態を判定
		const bool rockon = pCamera->GetState() == CCamera::STATE_ROCKON;

		// ロックオンじゃなかったらロックオン
		state = rockon ? CCamera::STATE_TRACKING : CCamera::STATE_ROCKON;

		pCamera->SetState(state);
	}
	// 位置の設定
	CCharacter3D::SetPosition(pos);

	// プレイヤーのモーションの遷移
	TransitionMotion();


	if (m_pMotion != nullptr)
	{
		// モーションの更新処理
		m_pMotion->Update(&m_apModel[0], m_nNumModel);
	}


	// 角度の補間
	CCharacter3D::SmoothAngle(0.1f);

	D3DXVECTOR3 posRDest;

	D3DXVECTOR3 rot = GetDestRot();

	posRDest.x = pos.x + sinf(rot.y) * 1.0f;
	posRDest.y = (pos.y + 200.0f) + sinf(rot.y) * 1.0f;
	posRDest.z = pos.z + cosf(rot.y) * 1.0f;

	// カメラの追従処理
	pCamera->SetTracking(posRDest,1.0f,0.1f);
}

//===================================================
// 描画処理
//===================================================
void CPlayer::Draw(void)
{
	// キャラクターの描画
	CCharacter3D::Draw();

	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		if (m_apModel[nCnt] != nullptr)
		{
			// 描画処理
			m_apModel[nCnt]->Draw();
		}
	}
}

//===================================================
// キーボードの移動処理
//===================================================
bool CPlayer::MoveKeyboard(CInputKeyboard* pKeyboard)
{
	bool bMove = false;

	// カメラの取得
	CCamera* pCamera = CManager::GetCamera();

	// カメラの向き
	D3DXVECTOR3 cameraRot = pCamera->GetRotaition();

	// 速さ
	float fSpeed = m_bDash ? MOVE_SPEED : 2.0f;

	// 目的の角度
	float fDestAngle = NULL;

	if (pKeyboard->GetPress(DIK_A))
	{
		//プレイヤーの移動(上)
		if (pKeyboard->GetPress(DIK_W) == true)
		{
			m_move.x += sinf(cameraRot.y - D3DX_PI * 0.25f) * fSpeed;
			m_move.z += cosf(cameraRot.y - D3DX_PI * 0.25f) * fSpeed;

			fDestAngle = cameraRot.y + D3DX_PI * 0.75f;

			// 目的の角度の設定
			CCharacter3D::SetRotDest(D3DXVECTOR3(0.0f, fDestAngle, 0.0f));

			bMove = true;
		}
		//プレイヤーの移動(下)
		else if (pKeyboard->GetPress(DIK_S))
		{
			m_move.x += sinf(cameraRot.y - D3DX_PI * 0.75f) * fSpeed;
			m_move.z += cosf(cameraRot.y - D3DX_PI * 0.75f) * fSpeed;

			fDestAngle = cameraRot.y + D3DX_PI * 0.25f;

			// 目的の角度の設定
			CCharacter3D::SetRotDest(D3DXVECTOR3(0.0f, fDestAngle, 0.0f));

			bMove = true;
		}
		// プレイヤーの移動(左)
		else
		{
			m_move.z += sinf(cameraRot.y) * fSpeed;
			m_move.x -= cosf(cameraRot.y) * fSpeed;

			fDestAngle = cameraRot.y + D3DX_PI * 0.5f;

			// 目的の角度の設定
			CCharacter3D::SetRotDest(D3DXVECTOR3(0.0f, fDestAngle, 0.0f));

			bMove = true;
		}
	}
	//プレイヤーの移動(右)
	else if (pKeyboard->GetPress(DIK_D))
	{
		//プレイヤーの移動(上)
		if (pKeyboard->GetPress(DIK_W))
		{
			m_move.x += sinf(cameraRot.y + D3DX_PI * 0.25f) * fSpeed;
			m_move.z += cosf(cameraRot.y + D3DX_PI * 0.25f) * fSpeed;

			fDestAngle = cameraRot.y - D3DX_PI * 0.75f;

			// 目的の角度の設定
			CCharacter3D::SetRotDest(D3DXVECTOR3(0.0f, fDestAngle, 0.0f));

			bMove = true;
		}
		//プレイヤーの移動(下)
		else if (pKeyboard->GetPress(DIK_S))
		{
			m_move.x += sinf(cameraRot.y + D3DX_PI * 0.75f) * fSpeed;
			m_move.z += cosf(cameraRot.y + D3DX_PI * 0.75f) * fSpeed;

			fDestAngle = cameraRot.y - D3DX_PI * 0.25f;

			// 目的の角度の設定
			CCharacter3D::SetRotDest(D3DXVECTOR3(0.0f, fDestAngle, 0.0f));

			bMove = true;
		}
		// プレイヤーの移動(右)
		else
		{
			m_move.z -= sinf(cameraRot.y) * fSpeed;
			m_move.x += cosf(cameraRot.y) * fSpeed;

			fDestAngle = cameraRot.y - D3DX_PI * 0.5f;

			// 目的の角度の設定
			CCharacter3D::SetRotDest(D3DXVECTOR3(0.0f, fDestAngle, 0.0f));

			bMove = true;
		}
	}
	//プレイヤーの移動(上)
	else if (pKeyboard->GetPress(DIK_W) == true)
	{
		m_move.x += sinf(cameraRot.y) * fSpeed;
		m_move.z += cosf(cameraRot.y) * fSpeed;

		fDestAngle = cameraRot.y + D3DX_PI;

		// 目的の角度の設定
		CCharacter3D::SetRotDest(D3DXVECTOR3(0.0f, fDestAngle, 0.0f));

		bMove = true;
	}
	//プレイヤーの移動(下)
	else if (pKeyboard->GetPress(DIK_S) == true)
	{
		m_move.x -= sinf(cameraRot.y) * fSpeed;
		m_move.z -= cosf(cameraRot.y) * fSpeed;

		fDestAngle = cameraRot.y;

		// 目的の角度の設定
		CCharacter3D::SetRotDest(D3DXVECTOR3(0.0f, fDestAngle, 0.0f));

		bMove = true;
	}
	else
	{
		int motiontype = m_pMotion->GetBlendMotionType();

		if (motiontype == MOTIONTYPE_MOVE || motiontype == MOTIONTYPE_DASH)
		{
			m_pMotion->SetMotion(MOTIONTYPE_NEUTRAL, true, 15);
		}
	}

	return bMove;
}

//===================================================
// パッドの移動処理
//===================================================
void CPlayer::MoveJoypad(CInputJoypad* pJoypad)
{
	XINPUT_STATE* pStick;

	pStick = pJoypad->GetJoyStickAngle();

	// カメラの取得
	CCamera* pCamera = CManager::GetCamera();

	// カメラの向き
	D3DXVECTOR3 cameraRot = pCamera->GetRotaition();

	// 速さ
	float fSpeed = m_bDash ? MOVE_SPEED : 2.0f;

	// Lスティックの角度
	float LStickAngleY = pStick->Gamepad.sThumbLY;
	float LStickAngleX = pStick->Gamepad.sThumbLX;

	// デッドゾーン
	float deadzone = 32767.0f * 0.25f;

	// スティックの傾けた角度を求める
	float magnitude = sqrtf((LStickAngleX * LStickAngleX) + (LStickAngleY * LStickAngleY));

	// 動かせる
	if (magnitude > deadzone)
	{
		// アングルを正規化
		float normalizeX = (LStickAngleX / magnitude);
		float normalizeY = (LStickAngleY / magnitude);

		// プレイヤーの移動量
		float moveX = normalizeX * cosf(-cameraRot.y) - normalizeY * sinf(-cameraRot.y);
		float moveZ = normalizeX * sinf(-cameraRot.y) + normalizeY * cosf(-cameraRot.y);

		// 移動量をスティックの角度によって変更
		float speedWk = fSpeed * ((magnitude - deadzone) / (32767.0f - deadzone));

		// プレイヤーの移動
		m_move.x += moveX * speedWk;
		m_move.z += moveZ * speedWk;

		// プレイヤーの角度を移動方向にする
		float fDestAngle = atan2f(-moveX, -moveZ);

		CCharacter3D::SetRotDest(D3DXVECTOR3(0.0f, fDestAngle, 0.0f));

		// ダッシュモーションか歩きモーションかを判定
		int isDashMotion = (m_bDash ? MOTIONTYPE_DASH : MOTIONTYPE_MOVE);

		// ジャンプかjumpじゃないかを判定
		int motiontype = m_bJump ? isDashMotion : MOTIONTYPE_JUMP;

		m_pMotion->SetMotion(motiontype, true, 5);
	}
	else
	{
		int motiontype = m_pMotion->GetBlendMotionType();

		if (motiontype == MOTIONTYPE_MOVE || motiontype == MOTIONTYPE_DASH)
		{
			m_pMotion->SetMotion(MOTIONTYPE_NEUTRAL, true, 15);
		}
	}
	
}

//===================================================
// プレイヤーのモーションの遷移
//===================================================
void CPlayer::TransitionMotion(void)
{
	// モーションの種類
	MOTIONTYPE motiontype = (MOTIONTYPE)m_pMotion->GetBlendMotionType();

	// モーションの遷移
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
// 生成処理
//===================================================
CPlayer* CPlayer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CPlayer* pPlayer = nullptr;

	// 3Dオブジェクトの生成
	pPlayer = new CPlayer;

	// 優先順位の取得
	int nPriority = pPlayer->GetPriority();

	// 現在のオブジェクトの最大数
	const int nNumAll = CObject::GetNumObject(nPriority);

	// オブジェクトが最大数まであったら
	if (nNumAll >= MAX_OBJECT && pPlayer != nullptr)
	{
		// 自分のポインタの解放
		pPlayer->Release();

		// nullにする
		pPlayer = nullptr;

		// オブジェクトを消す
		return nullptr;
	}

	if (pPlayer == nullptr) return nullptr;

	pPlayer->SetPosition(pos);
	pPlayer->SetRotation(rot);
	pPlayer->Init();

	return pPlayer;
}
