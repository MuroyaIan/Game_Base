/**
 * @file ShaderMgr.h
 * @brief �V�F�[�_�Ǘ�
 * @author ���J�C�A��
 * @date 2022/07/21
 * @���� 2022/07/21�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Graphic.h>

//===== �O���錾 =====
class CT_BINDER;
struct CB_PTR;

//===== �N���X��` =====

//***** �V�F�[�_�Ǘ� *****
class SHADER_MGR
{
public:

	//�o�C���_�ꗗ
	enum class BINDER_ID
	{
		//���_�V�F�[�_
		VS_Default,
		VS_VtxBlend,
		VS_Texture,

		VS_Model,
		VS_Model_Normal,
		VS_Model_Bone,

		VS_Instance,
		VS_Instance_VtxBlend,
		VS_Instance_Texture,
		VS_Instance_Phong,
		VS_Instance_Phong_Anim,
		VS_Instance_Phong_Anim_NoSkin,
		VS_Instance_PhongN,

		//���̓��C�A�E�g
		IL_Default,
		IL_VtxBlend,
		IL_Texture,

		IL_Model,
		IL_Model_Normal,
		IL_Model_Bone,

		IL_Instance,
		IL_Instance_VtxBlend,
		IL_Instance_Texture,
		IL_Instance_Phong,
		IL_Instance_Phong_Anim,
		IL_Instance_Phong_Anim_NoSkin,
		IL_Instance_PhongN,

		//�v���~�e�B�u�g�|���W�[
		PT_Line,
		PT_Tri,

		//�T���v���[
		Sampler,

		//�s�N�Z���V�F�[�_
		PS_Default,
		PS_OneColor,
		PS_VtxBlend,
		PS_Texture,

		PS_Model,
		PS_Model_Normal,
		PS_Model_Disp,

		PS_Phong,

		//�萔�o�b�t�@
		CB_VS_MtxVP,
		CB_PS_Default,
		CB_Light,

		ID_Max
	};

	//�v���g�^�C�v�錾
	explicit SHADER_MGR(CT_GRAPHIC& Gfx);
	~SHADER_MGR() noexcept;
	void Bind(BINDER_ID id) const noexcept;				//�o�C���h����
	void Bind_Default() const noexcept;					//�o�C���h�����i�f�t�H���g�j
	void Bind_VertexBlend() const noexcept;				//�o�C���h�����i���_�u�����h�j
	void Bind_Texture() const noexcept;					//�o�C���h�����i�e�N�X�`���j
	void Bind_Instance() const noexcept;				//�o�C���h�����i�C���X�^���V���O�j
	void Bind_Instance_VertexBlend() const noexcept;	//�o�C���h�����i�C���X�^���V���O�Q���_�u�����h�j
	void Bind_Instance_Texture() const noexcept;		//�o�C���h�����i�C���X�^���V���O�Q�e�N�X�`���j
	void Bind_Instance_Phong() const noexcept;			//�o�C���h�����i�C���X�^���V���O�QPhong���f���j
	void Bind_Instance_Phong_Anim() const noexcept;		//�o�C���h�����i�C���X�^���V���O�QPhong���f���Q�A�j���t�j

	CT_BINDER& GetBinder(BINDER_ID id) const noexcept		//�o�C���_�Q�Ƃ��擾
	{
		return *m_aBinder[static_cast<int>(id)];
	}

	void SetConstBufferPtr(BINDER_ID id, CB_PTR* cbPtr) const;	//�萔�o�b�t�@�|�C���^�o�^

private:

	//�ϐ��錾
	CT_GRAPHIC& m_DX;										//DX�Q��
	std::vector<std::unique_ptr<CT_BINDER>> m_aBinder;		//�o�C���_�̃|�C���^�z��
};
