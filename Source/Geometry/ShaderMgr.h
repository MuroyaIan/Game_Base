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
class BINDER;
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
		VS_DEFAULT,
		VS_VTX_BLEND,
		VS_TEXTURE,

		VS_MODEL,
		VS_MODEL_NORMAL,
		VS_MODEL_BONE,

		VS_INSTANCE,
		VS_INSTANCE_VTX_BLEND,
		VS_INSTANCE_TEXTURE,
		VS_INSTANCE_PHONG,
		VS_INSTANCE_PHONG_ANIM,

		//���̓��C�A�E�g
		IL_DEFAULT,
		IL_VTX_BLEND,
		IL_TEXTURE,

		IL_MODEL,
		IL_MODEL_NORMAL,
		IL_MODEL_BONE,

		IL_INSTANCE,
		IL_INSTANCE_VTX_BLEND,
		IL_INSTANCE_TEXTURE,
		IL_INSTANCE_PHONG,
		IL_INSTANCE_PHONG_ANIM,

		//�v���~�e�B�u�g�|���W�[
		PT_LINE,
		PT_TRI,

		//�T���v���[
		SAMPLER,

		//�s�N�Z���V�F�[�_
		PS_DEFAULT,
		PS_ONE_COLOR,
		PS_VTX_BLEND,
		PS_TEXTURE,

		PS_MODEL,
		PS_MODEL_NORMAL,

		PS_PHONG,

		//�萔�o�b�t�@
		CB_VS_MTX_VP,
		CB_PS_DEFAULT,
		CB_PS_LIGHT,

		ID_MAX
	};

	//�v���g�^�C�v�錾
	SHADER_MGR(GRAPHIC& Gfx);
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

	BINDER& GetBinder(BINDER_ID id) const noexcept		//�o�C���_�Q�Ƃ��擾
	{
		return *m_aBinder[static_cast<int>(id)];
	}

	void SetConstBufferPtr(BINDER_ID id, CB_PTR* cbPtr) const;	//�萔�o�b�t�@�|�C���^�o�^

private:

	//�ϐ��錾
	GRAPHIC& m_DX;										//DX�Q��
	std::vector<std::unique_ptr<BINDER>> m_aBinder;		//�o�C���_�̃|�C���^�z��
};
