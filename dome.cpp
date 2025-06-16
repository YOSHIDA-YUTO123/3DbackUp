//================================================
//
// ���b�V���h�[�� [dome.cpp]
// Author: YUTO YOSHIDA
//
//================================================

//************************************************
// �C���N���[�h�t�@�C��
//************************************************
#include "dome.h"
#include"manager.h"
#include"renderer.h"

//================================================
// �R���X�g���N�^
//================================================
CMeshDome::CMeshDome()
{
	m_pIdxBuffMeshDome = nullptr;
}

//================================================
// �f�X�g���N�^
//================================================
CMeshDome::~CMeshDome()
{
}

//================================================
// ��������
//================================================
CMeshDome* CMeshDome::Create(const D3DXVECTOR3 pos, const int nSegX, const int nSegZ, const float fRadius, const float fHeight, const D3DXVECTOR3 rot)
{
	// ���b�V���h�[���𐶐�
	CMeshDome* pMesh = new CMeshDome;

	// �D�揇�ʂ̎擾
	int nPriority = pMesh->GetPriority();

	// ���݂̃I�u�W�F�N�g�̍ő吔
	const int nNumAll = CObject::GetNumObject(nPriority);

	// �I�u�W�F�N�g���ő吔�܂ł�������
	if (nNumAll >= MAX_OBJECT && pMesh != nullptr)
	{
		// �����̃|�C���^�̉��
		pMesh->Release();

		// null�ɂ���
		pMesh = nullptr;

		// �I�u�W�F�N�g������
		return nullptr;
	}

	if (pMesh == nullptr) return nullptr;

	// ���_���̐ݒ�
	int nNumFanVtx = (nSegX + 1) + 1;

	// ���_���̐ݒ�
	int nNumDomeVtx = (nSegX + 1) * (nSegZ + 1);

	// �|���S�����̐ݒ�
	int nNumDomePolygon = (((nSegX * nSegZ) * 2)) + (4 * (nSegZ - 1));

	// �C���f�b�N�X���̐ݒ�
	int nNumDomeIndex = nNumDomePolygon + 2;

	// ���_���̐ݒ�
	int nNumVtx = nNumFanVtx + nNumDomeVtx;

	// ���_�̐ݒ�
	pMesh->SetVtxElement(nNumVtx, nNumDomePolygon, nNumDomeIndex);
	pMesh->SetSegment(nSegX, nSegZ);
	
	// ����������
	pMesh->Init();

	// �ݒ菈��
	pMesh->SetPosition(pos);
	pMesh->SetRotation(rot);

	pMesh->SetDome(nSegX, nSegZ, fRadius, fHeight);
	return pMesh;
}

//================================================
// ����������
//================================================
HRESULT CMeshDome::Init(void)
{
	// ����������
	if (FAILED(CMesh::Init()))
	{
		return E_FAIL;
	}

	// �e�N�X�`����ID�̐ݒ�
	CMesh::SetTextureID("data/TEXTURE/sky.jpg");

	return E_FAIL;
}

//================================================
// �I������
//================================================
void CMeshDome::Uninit(void)
{
	if (m_pIdxBuffMeshDome != nullptr)
	{
		m_pIdxBuffMeshDome->Release();
		m_pIdxBuffMeshDome = nullptr;
	}

	// �I������
	CMesh::Uninit();
}

//================================================
// �X�V����
//================================================
void CMeshDome::Update(void)
{
	UpdateRotation(D3DXVECTOR3(0.0f,0.001f,0.0f));
}

//================================================
// �`�揈��
//================================================
void CMeshDome::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	pDevice->SetRenderState(D3DRS_CULLMODE, TRUE);

	// �`�揈��
	CMesh::Draw();

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuffMeshDome);

	//�e�N�X�`���t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	LPDIRECT3DTEXTURE9 pTexture = nullptr;

	// �e�N�X�`���̎擾
	pDevice->GetTexture(0, (IDirect3DBaseTexture9**)&pTexture);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture);

	int nSegX = GetSegX();

	// ���_���̐ݒ�
	int nNumFanVtx = (nSegX + 1) + 1;

	// �|���S�����̐ݒ�
	int nNumFanPolygon = (nNumFanVtx - 1) * 3;

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, nNumFanVtx, 0, nNumFanPolygon);
	
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//================================================
// �h�[���̐ݒ菈��
//================================================
void CMeshDome::SetDome(const int nSegX, const int nSegZ, const float fRadius, const float fHeight)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	int nCntVtx = 0;
	float fTexX = 1.0f / (nSegX);
	float fTexY = 1.0f / (nSegZ);

	float fNowRadius = fRadius / (nSegZ + 1);

	// �W�̕����̍쐬
	for (int nCntX = 0; nCntX <= nSegX; nCntX++)
	{
		float fAngle = (D3DX_PI * 2.0f) / nSegX * nCntX;

		D3DXVECTOR3 posWk = VEC3_NULL,posOld = VEC3_NULL;

		posWk.x = sinf(fAngle) * fNowRadius;
		posWk.y = fHeight;
		posWk.z = cosf(fAngle) * fNowRadius;

		float fPosTexX = fTexX + cosf(fAngle) * fTexX;
		float fPosTexY = fTexY - sinf(fAngle) * fTexY;

		SetVtxBuffer(posWk, nCntVtx, D3DXVECTOR2(fPosTexX, fPosTexY));

		// �Ō�܂ōs������
		if (nCntX == nSegX)
		{
			// �Ō���Ȃ���
			SetVtxBuffer(posOld, nCntVtx + 1, D3DXVECTOR2(fPosTexX, fPosTexY));
		}

		// �O��̌v�Z��ۑ�
		posOld = posWk;

		nCntVtx++;
	}
	
	int OffsetIdx = nCntVtx;

	// �h�[���̂Ă��؂�̈ʒu�̐ݒ�
	SetVtxBuffer(D3DXVECTOR3(0.0f, fHeight + (fHeight / nSegZ + 1),0.0f), 0, D3DXVECTOR2(fTexX, fTexY));

	int nNumVertex = (nSegX + 1) + 1;
	int nNumPolygon = (nNumVertex - 1) * 3;
	int nNumidx = nNumPolygon + 2;

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * nNumidx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuffMeshDome,
		NULL);

	WORD* pIdx;

	//�C���f�b�N�X�o�b�t�@�̃��b�N
	m_pIdxBuffMeshDome->Lock(0, 0, (void**)&pIdx, 0);

	int nCntIdx = 0;

	for (int IndxCount2 = 0; IndxCount2 < nSegX; IndxCount2++)
	{
		pIdx[nCntIdx] = (WORD)0;
		pIdx[nCntIdx + 1] = (WORD)IndxCount2 + 1;
		pIdx[nCntIdx + 2] = (WORD)IndxCount2 + 2;

		nCntIdx += 3;
	}

	pIdx[nCntIdx] = (WORD)0;
	pIdx[nCntIdx + 1] = (WORD)nSegX;
	pIdx[nCntIdx + 2] = (WORD)1;

	nCntIdx += 3;

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	m_pIdxBuffMeshDome->Unlock();

	// �e�N�X�`���̃I�t�Z�b�g
	fTexX = 1.0f / nSegX;
	fTexY = 1.0f / nSegZ;

	// ���a
	fNowRadius = fRadius / (nSegZ + 1);
	float fRateRadius = fNowRadius;

	//�c
	for (int nCntZ = 0; nCntZ <= nSegZ; nCntZ++)
	{
		//��
		for (int nCntX = 0; nCntX <= nSegX; nCntX++)
		{
			float fAngel = (D3DX_PI * 2.0f) / nSegX * nCntX;

			D3DXVECTOR3 posWk = VEC3_NULL;

			posWk.x = sinf(fAngel) * fNowRadius;
			posWk.y = fHeight - (fHeight / nSegZ) * nCntZ;
			posWk.z = cosf(fAngel) * fNowRadius;
			
			SetVtxBuffer(posWk, nCntVtx, D3DXVECTOR2(fTexX * nCntX, fTexY * nCntZ));

			nCntVtx++;//���Z
		}
		fNowRadius += fRateRadius / (nCntZ + 1);
	}

	int IndxCount3 = nSegX + 1;//X

	int IdxCnt = 0;//�z��

	int Num = 0;//

	//�C���f�b�N�X�̐ݒ�
	for (int IndxCount1 = 0; IndxCount1 < nSegZ; IndxCount1++)
	{
		for (int IndxCount2 = 0; IndxCount2 <= nSegX; IndxCount2++, IndxCount3++, Num++)
		{
			// �C���f�b�N�X�o�b�t�@�̐ݒ�
			SetIndexBuffer((WORD)(IndxCount3 + OffsetIdx), IdxCnt);
			SetIndexBuffer((WORD)(Num + OffsetIdx), IdxCnt + 1);
			IdxCnt += 2;
		}

		if (IndxCount1 < nSegZ - 1)
		{
			// �C���f�b�N�X�o�b�t�@�̐ݒ�
			SetIndexBuffer((WORD)(Num - 1 + OffsetIdx), IdxCnt);
			SetIndexBuffer((WORD)(IndxCount3 + OffsetIdx), IdxCnt + 1);
			IdxCnt += 2;
		}
	}

}
