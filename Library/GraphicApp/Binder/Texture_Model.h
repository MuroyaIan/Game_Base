/**
 * @file Texture_Model.h
 * @brief �e�N�X�`���̏���
 * @author ���J�C�A��
 * @date 2022/07/16
 * @���� 2022/07/16�F�t�@�C���쐬
 *		 2022/11/24�F�������P
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/Binder.h>
#include <Tool/TextureLoader.h>

//===== �N���X��` =====

//***** �e�N�X�`�� *****
class TEXTURE_MODEL : public CT_BINDER
{
public:

	//�e�N�X�`���^�C�v
	enum class TEX_TYPE
	{
		Diffuse,		//�g�U�F
		Specular,		//���ːF
		Normal,			//�m�[�}���}�b�v
		Displacement,	//�����}�b�v

		MaxType
	};

	//�v���g�^�C�v�錾
	explicit TEXTURE_MODEL(const CT_GRAPHIC& Gfx, const std::vector<TEX_LOADER::TEX_DATA>& aData, UINT StartSlot = 0u);
	~TEXTURE_MODEL() noexcept override;
	void Bind(const CT_GRAPHIC& Gfx) const override;	//�o�C���h����

private:

	//�ϐ��錾
	std::vector<ComPtr<ID3D11ShaderResourceView>> m_pTextureViews;	//�|�C���^�z��

	//�v���g�^�C�v�錾
	void MakeBuffer(const CT_GRAPHIC& Gfx, const TEX_LOADER::TEX_DATA& Data, TEX_TYPE Type);	//�o�b�t�@�쐬
	UINT m_StartSlot;																		//���W�X�^�ԍ�
};
