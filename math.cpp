//===================================================
//
// 計算 [math.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "math.h"

//===================================================
// 距離の取得
//===================================================
float math::GetDistance(const D3DXVECTOR3 diff)
{
	// 距離を二乗する
	float sqX = diff.x * diff.x;
	float sqY = diff.y * diff.y;
	float sqZ = diff.z * diff.z;
	
	// 距離を求める
	float Out = sqrtf(sqX + sqY + sqZ);
	
	return Out;
}

//===================================================
// 目的の方向ベクトルの取得
//===================================================
D3DXVECTOR3 math::GetTargetDir(const D3DXVECTOR3 dest, const D3DXVECTOR3 pos)
{
	// 方向を求める
	D3DXVECTOR3 diff = dest - pos;

	// 正規化する
	D3DXVec3Normalize(&diff, &diff);

	return diff;
}

//===================================================
// 目的までの角度の取得
//===================================================
float math::GetTargetAngle(const D3DXVECTOR3 dest, const D3DXVECTOR3 pos)
{
	D3DXVECTOR3 diff = dest - pos;

	float fAngle = atan2f(diff.x, diff.z);

	return fAngle;
}
