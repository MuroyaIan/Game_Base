/**
 * @file Texture_Model.h
 * @brief �e�N�X�`���̏���
 * @author ���J�C�A��
 * @date 2022/07/16
 * @���� 2022/07/16�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/Binder.h>
#include <Tool/TextureLoader.h>

//===== �N���X��` =====

//***** �e�N�X�`�� *****
class TEXTURE_MODEL : public BINDER
{
public:

	//�e�N�X�`���^�C�v
	enum class TEX_TYPE
	{
		DIFFUSE,
		SPECULAR,
		NORMAL,

		MAX_TYPE
	};

	//�v���g�^�C�v�錾
	TEXTURE_MODEL(GRAPHIC& Gfx, std::vector<TEX_LOADER::TEX_DATA>& aData);
	~TEXTURE_MODEL() noexcept override;
	void Bind(GRAPHIC& Gfx) noexcept override;	//�o�C���h����

protected:

	using com_pSRV = Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>;

	//�ϐ��錾
	std::vector<com_pSRV> m_aTextureViewPtr;	//�|�C���^�z��

	//�v���g�^�C�v�錾
	void MakeBuffer(GRAPHIC& Gfx, TEX_LOADER::TEX_DATA& Data, TEX_TYPE Type);	//�o�b�t�@�쐬
};
