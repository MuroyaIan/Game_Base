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
struct CBD_COLOR						//�|���S���F�p�f�[�^
{
	//�ϐ��錾
	DirectX::XMFLOAT4 FaceColor[24];	//�|���S���F�i�ʂ��Ɓj

	CBD_COLOR() noexcept
	{
		FaceColor[0] = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		FaceColor[1] = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		FaceColor[2] = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
		FaceColor[3] = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
		FaceColor[4] = DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
		FaceColor[5] = DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
		FaceColor[6] = DirectX::XMFLOAT4(1.0f, 0.5f, 0.0f, 1.0f);
		FaceColor[7] = DirectX::XMFLOAT4(0.5f, 1.0f, 0.0f, 1.0f);
		FaceColor[8] = DirectX::XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f);
		FaceColor[9] = DirectX::XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f);
		FaceColor[10] = DirectX::XMFLOAT4(0.0f, 1.0f, 0.5f, 1.0f);
		FaceColor[11] = DirectX::XMFLOAT4(0.0f, 0.5f, 1.0f, 1.0f);
		FaceColor[12] = DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f);
		FaceColor[13] = DirectX::XMFLOAT4(0.7f, 1.0f, 0.0f, 1.0f);
		FaceColor[14] = DirectX::XMFLOAT4(1.0f, 0.0f, 0.7f, 1.0f);
		FaceColor[15] = DirectX::XMFLOAT4(0.7f, 0.0f, 1.0f, 1.0f);
		FaceColor[16] = DirectX::XMFLOAT4(0.0f, 1.0f, 0.7f, 1.0f);
		FaceColor[17] = DirectX::XMFLOAT4(0.0f, 0.7f, 1.0f, 1.0f);
		FaceColor[18] = DirectX::XMFLOAT4(1.0f, 0.3f, 0.0f, 1.0f);
		FaceColor[19] = DirectX::XMFLOAT4(0.3f, 1.0f, 0.0f, 1.0f);
		FaceColor[20] = DirectX::XMFLOAT4(1.0f, 0.0f, 0.3f, 1.0f);
		FaceColor[21] = DirectX::XMFLOAT4(0.3f, 0.0f, 1.0f, 1.0f);
		FaceColor[22] = DirectX::XMFLOAT4(0.0f, 1.0f, 0.3f, 1.0f);
		FaceColor[23] = DirectX::XMFLOAT4(0.0f, 0.3f, 1.0f, 1.0f);
	}

	~CBD_COLOR() noexcept
	{}
};

struct CBD_MTX_VP					//�ϊ��s��iVP�j
{
	//�ϐ��錾
	DirectX::XMFLOAT4X4 mtxView;	//�r���[�s��
	DirectX::XMFLOAT4X4 mtxProj;	//���e�s��

	CBD_MTX_VP() noexcept : mtxView(), mtxProj()
	{
		DirectX::XMStoreFloat4x4(&mtxView, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&mtxProj, DirectX::XMMatrixIdentity());
	}

	CBD_MTX_VP(DirectX::XMFLOAT4X4 mtxV, DirectX::XMFLOAT4X4 mtxP) noexcept : mtxView(mtxV), mtxProj(mtxP)
	{}

	~CBD_MTX_VP() noexcept
	{}
};

struct CBD_MTX_LWVP					//�ϊ��s��iLWVP�j
{
	//�ϐ��錾
	DirectX::XMFLOAT4X4 mtxLocal;	//���[�J���s��
	DirectX::XMFLOAT4X4 mtxWorld;	//���[���h�s��
	DirectX::XMFLOAT4X4 mtxView;	//�r���[�s��
	DirectX::XMFLOAT4X4 mtxProj;	//���e�s��

	CBD_MTX_LWVP() noexcept : mtxLocal(), mtxWorld(), mtxView(), mtxProj()
	{
		DirectX::XMStoreFloat4x4(&mtxLocal, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&mtxWorld, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&mtxView, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&mtxProj, DirectX::XMMatrixIdentity());
	}

	CBD_MTX_LWVP(DirectX::XMFLOAT4X4 mtxL, DirectX::XMFLOAT4X4 mtxW, DirectX::XMFLOAT4X4 mtxV, DirectX::XMFLOAT4X4 mtxP) noexcept :
		mtxLocal(mtxL), mtxWorld(mtxW), mtxView(mtxV), mtxProj(mtxP)
	{}

	~CBD_MTX_LWVP() noexcept
	{}
};

struct CBD_MATERIAL					//�}�e���A�����
{
	DirectX::XMFLOAT4 Ambient;		//����
	DirectX::XMFLOAT4 Diffuse;		//�g�U���ˌ�
	DirectX::XMFLOAT4 Emissive;		//���ˌ�
	DirectX::XMFLOAT4 Transparent;	//���ߓx
	DirectX::XMFLOAT4 Specular;		//���ʔ��ˌ�
	float Shininess;				//����
	float Pad1;
	float Pad2;
	float Pad3;						//���u��

	CBD_MATERIAL() noexcept :
		Ambient(0.0f, 0.0f, 0.0f, 0.0f), Diffuse(0.0f, 0.0f, 0.0f, 0.0f), Emissive(0.0f, 0.0f, 0.0f, 0.0f), Transparent(0.0f, 0.0f, 0.0f, 0.0f),
		Specular(0.0f, 0.0f, 0.0f, 0.0f), Shininess(2.0f), Pad1(0.0f), Pad2(0.0f), Pad3(0.0f)
	{}

	~CBD_MATERIAL() noexcept {}
};

struct CBD_MTX_LOCAL				//�ϊ��s��i���[�J���j
{
	//�ϐ��錾
	DirectX::XMFLOAT4X4 mtxSkin;	//���Ȃ����b�V���p���[�J���s��

	CBD_MTX_LOCAL() noexcept : mtxSkin()
	{
		DirectX::XMStoreFloat4x4(&mtxSkin, DirectX::XMMatrixIdentity());
	}

	~CBD_MTX_LOCAL() noexcept
	{}
};

struct CBD_BONE								//���f�[�^
{
	//�ϐ��錾
	DirectX::XMFLOAT4X4 mtxBone[MAX_BONE];	//���̃��[���h�s��

	CBD_BONE() noexcept : mtxBone()
	{
		for (auto& b : mtxBone)
			DirectX::XMStoreFloat4x4(&b, DirectX::XMMatrixIdentity());
	}

	~CBD_BONE() noexcept
	{}
};
