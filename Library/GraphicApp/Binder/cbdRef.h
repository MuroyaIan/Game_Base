/**
 * @file cbdRef.h
 * @brief �萔�o�b�t�@�f�[�^�܂Ƃ�
 * @author ���J�C�A��
 * @date 2022/12/06
 * @���� 2022/12/06�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Drawer/VertexShaderData.h>

//===== �\���̐錾 =====
struct CBD_COLOR					//�|���S���F�p�f�[�^
{
	//�ϐ��錾
	dx::XMFLOAT4 ms_FaceColor[24];	//�|���S���F�i�ʂ��Ɓj

	CBD_COLOR() noexcept
	{
		ms_FaceColor[0]  = dx::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		ms_FaceColor[1]  = dx::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		ms_FaceColor[2]  = dx::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
		ms_FaceColor[3]  = dx::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
		ms_FaceColor[4]  = dx::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
		ms_FaceColor[5]  = dx::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
		ms_FaceColor[6]  = dx::XMFLOAT4(1.0f, 0.5f, 0.0f, 1.0f);
		ms_FaceColor[7]  = dx::XMFLOAT4(0.5f, 1.0f, 0.0f, 1.0f);
		ms_FaceColor[8]  = dx::XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f);
		ms_FaceColor[9]  = dx::XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f);
		ms_FaceColor[10] = dx::XMFLOAT4(0.0f, 1.0f, 0.5f, 1.0f);
		ms_FaceColor[11] = dx::XMFLOAT4(0.0f, 0.5f, 1.0f, 1.0f);
		ms_FaceColor[12] = dx::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f);
		ms_FaceColor[13] = dx::XMFLOAT4(0.7f, 1.0f, 0.0f, 1.0f);
		ms_FaceColor[14] = dx::XMFLOAT4(1.0f, 0.0f, 0.7f, 1.0f);
		ms_FaceColor[15] = dx::XMFLOAT4(0.7f, 0.0f, 1.0f, 1.0f);
		ms_FaceColor[16] = dx::XMFLOAT4(0.0f, 1.0f, 0.7f, 1.0f);
		ms_FaceColor[17] = dx::XMFLOAT4(0.0f, 0.7f, 1.0f, 1.0f);
		ms_FaceColor[18] = dx::XMFLOAT4(1.0f, 0.3f, 0.0f, 1.0f);
		ms_FaceColor[19] = dx::XMFLOAT4(0.3f, 1.0f, 0.0f, 1.0f);
		ms_FaceColor[20] = dx::XMFLOAT4(1.0f, 0.0f, 0.3f, 1.0f);
		ms_FaceColor[21] = dx::XMFLOAT4(0.3f, 0.0f, 1.0f, 1.0f);
		ms_FaceColor[22] = dx::XMFLOAT4(0.0f, 1.0f, 0.3f, 1.0f);
		ms_FaceColor[23] = dx::XMFLOAT4(0.0f, 0.3f, 1.0f, 1.0f);
	}

	~CBD_COLOR() noexcept = default;
};

struct CBD_MTX_VP				//�ϊ��s��iVP�j
{
	//�ϐ��錾
	dx::XMFLOAT4X4 ms_MtxView;	//�r���[�s��
	dx::XMFLOAT4X4 ms_MtxProj;	//���e�s��

	CBD_MTX_VP() noexcept : ms_MtxView(), ms_MtxProj()
	{
		dx::XMStoreFloat4x4(&ms_MtxView, dx::XMMatrixIdentity());
		dx::XMStoreFloat4x4(&ms_MtxProj, dx::XMMatrixIdentity());
	}

	CBD_MTX_VP(dx::XMFLOAT4X4 mtxV, dx::XMFLOAT4X4 mtxP) noexcept : ms_MtxView(mtxV), ms_MtxProj(mtxP)
	{}

	~CBD_MTX_VP() noexcept = default;
};

struct CBD_MTX_LWVP					//�ϊ��s��iLWVP�j
{
	//�ϐ��錾
	dx::XMFLOAT4X4 ms_MtxLocal;		//���[�J���s��
	dx::XMFLOAT4X4 ms_MtxWorld;		//���[���h�s��
	dx::XMFLOAT4X4 ms_MtxView;		//�r���[�s��
	dx::XMFLOAT4X4 ms_MtxProj;		//���e�s��

	CBD_MTX_LWVP() noexcept : ms_MtxLocal(), ms_MtxWorld(), ms_MtxView(), ms_MtxProj()
	{
		dx::XMStoreFloat4x4(&ms_MtxLocal, dx::XMMatrixIdentity());
		dx::XMStoreFloat4x4(&ms_MtxWorld, dx::XMMatrixIdentity());
		dx::XMStoreFloat4x4(&ms_MtxView, dx::XMMatrixIdentity());
		dx::XMStoreFloat4x4(&ms_MtxProj, dx::XMMatrixIdentity());
	}

	CBD_MTX_LWVP(dx::XMFLOAT4X4 mtxL, dx::XMFLOAT4X4 mtxW, dx::XMFLOAT4X4 mtxV, dx::XMFLOAT4X4 mtxP) noexcept :
		ms_MtxLocal(mtxL), ms_MtxWorld(mtxW), ms_MtxView(mtxV), ms_MtxProj(mtxP)
	{}

	~CBD_MTX_LWVP() noexcept = default;
};

struct CBD_MATERIAL					//�}�e���A�����
{
	dx::XMFLOAT4 ms_Ambient;		//����
	dx::XMFLOAT4 ms_Diffuse;		//�g�U���ˌ�
	dx::XMFLOAT4 ms_Emissive;		//���ˌ�
	dx::XMFLOAT4 ms_Transparent;	//���ߓx
	dx::XMFLOAT4 ms_Specular;		//���ʔ��ˌ�
	float ms_Shininess;				//����
	float ms_Disp_MinLayerNum;
	float ms_Disp_MaxLayerNum;		//�����}�b�v�̃T���v�����O�͈�
	float ms_Disp_DepthScale;		//�����}�b�v�̐[���W��

	CBD_MATERIAL() noexcept :
		ms_Ambient(0.0f, 0.0f, 0.0f, 0.0f), ms_Diffuse(0.0f, 0.0f, 0.0f, 0.0f), ms_Emissive(0.0f, 0.0f, 0.0f, 0.0f), ms_Transparent(0.0f, 0.0f, 0.0f, 0.0f),
		ms_Specular(0.0f, 0.0f, 0.0f, 0.0f), ms_Shininess(2.0f), ms_Disp_MinLayerNum(1.0f), ms_Disp_MaxLayerNum(2.0f), ms_Disp_DepthScale(0.0f)
	{}

	~CBD_MATERIAL() noexcept = default;
};

struct CBD_BONE								//���f�[�^
{
	//�ϐ��錾
	dx::XMFLOAT4X4 ms_MtxBone[MAX_BONE];	//���̃��[���h�s��

	CBD_BONE() noexcept : ms_MtxBone()
	{
		for (auto& l_MtxBone : ms_MtxBone)
			dx::XMStoreFloat4x4(&l_MtxBone, dx::XMMatrixIdentity());
	}

	~CBD_BONE() noexcept = default;
};
