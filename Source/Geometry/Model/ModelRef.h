/**
 * @file ModelRef.h
 * @brief ���f���Q�Ƃ܂Ƃ�
 * @author ���J�C�A��
 * @date 2022/09/15
 * @���� 2022/09/15�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Drawer/vsdRef.h>
#include <Tool/TextureLoader.h>
#include <GraphicApp/Binder/cbdRef.h>

//===== �\���̐錾 =====
namespace ModelRef {

	//�A�j���[�V�����p�b�N
	struct ANIM_PACK
	{
		std::vector<DirectX::XMFLOAT4X4> aMatrix;	//�t���[���p���z��

		ANIM_PACK() noexcept : aMatrix(0)
		{}
		~ANIM_PACK() noexcept {}
	};

	//���b�V���p�b�N
	struct MESH_PACK
	{
		std::string Name;					//���b�V����
		VS_DATA<VERTEX_MB> vsData;			//���_���
		CBD_MATERIAL MaterialData;			//�}�e���A�����
		std::string Tex_D;					//�e�N�X�`���F�g�U���ˌ�
		std::vector<ANIM_PACK> aNoSkin;		//���Ȃ����b�V���p�A�j���[�V�����z��

		MESH_PACK() noexcept : Name(""), vsData(), MaterialData(), Tex_D(""), aNoSkin(0)
		{}
		~MESH_PACK() noexcept {}
	};

	//���p�b�N
	struct BONE_PACK
	{
		std::string BoneName;				//�{�[����
		DirectX::XMFLOAT4X4 InitMatrix;		//�����p��
		std::vector<ANIM_PACK> aBoneSkin;	//���p�A�j���[�V�����z��
		std::vector<ANIM_PACK> aSkin;		//�A�j���[�V�����z��

		BONE_PACK() noexcept : BoneName(""), aBoneSkin(0), aSkin(0)
		{
			DirectX::XMStoreFloat4x4(&InitMatrix, DirectX::XMMatrixIdentity());
		}
		~BONE_PACK() noexcept {}
	};

	//�e�N�X�`���p�b�N
	struct TEX_PACK
	{
		std::string Name;				//�e�N�X�`����
		TEX_LOADER::TEX_DATA TexData;	//�e�N�X�`�����

		TEX_PACK() noexcept : Name(""), TexData()
		{}
		~TEX_PACK() noexcept {}
	};

	//���f���p�b�N
	struct MODEL_PACK
	{
		std::vector<MESH_PACK> aMesh;			//���b�V���z��
		std::vector<BONE_PACK> aBone;			//���z��
		std::vector<int> aNoSkinIndex;			//���Ȃ����b�V���ԍ�
		std::vector<std::string> aAnimName;		//�A�j���[�V������
		std::vector<int> aAnimFrame;			//�A�j���[�V�����t���[����
		std::vector<bool> aIsFPS_30;			//�A�j���[�V����FPS��
		std::vector<TEX_PACK> m_aTexPack;		//�e�N�X�`���p�b�N�z��
		DirectX::XMFLOAT4X4 InitMtxWorld;		//�������[���h�s��

		MODEL_PACK() noexcept : aMesh(0), aBone(0), aNoSkinIndex(0), aAnimName(0), aAnimFrame(0), aIsFPS_30(false), m_aTexPack(0)
		{
			DirectX::XMStoreFloat4x4(&InitMtxWorld, DirectX::XMMatrixIdentity());
		}
		~MODEL_PACK() noexcept {}
	};
}
