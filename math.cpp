//===================================================
//
// �v�Z [math.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "math.h"

//===================================================
// �����̎擾
//===================================================
float math::GetDistance(const D3DXVECTOR3 diff)
{
	// �������悷��
	float sqX = diff.x * diff.x;
	float sqY = diff.y * diff.y;
	float sqZ = diff.z * diff.z;
	
	// ���������߂�
	float Out = sqrtf(sqX + sqY + sqZ);
	
	return Out;
}

//===================================================
// �ړI�̕����x�N�g���̎擾
//===================================================
D3DXVECTOR3 math::GetTargetDir(const D3DXVECTOR3 dest, const D3DXVECTOR3 pos)
{
	// ���������߂�
	D3DXVECTOR3 diff = dest - pos;

	// ���K������
	D3DXVec3Normalize(&diff, &diff);

	return diff;
}

//===================================================
// �ړI�܂ł̊p�x�̎擾
//===================================================
float math::GetTargetAngle(const D3DXVECTOR3 dest, const D3DXVECTOR3 pos)
{
	D3DXVECTOR3 diff = dest - pos;

	float fAngle = atan2f(diff.x, diff.z);

	return fAngle;
}
