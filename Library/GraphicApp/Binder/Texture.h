/**
 * @file Texture.h
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
class TEXTURE : public BINDER
{
public:

	//�v���g�^�C�v�錾
	TEXTURE(GRAPHIC& Gfx, TEX_LOADER::TEX_DATA& Data);
	~TEXTURE() noexcept override;
	void Bind(GRAPHIC& Gfx) noexcept override;	//�o�C���h����

protected:

	//�ϐ��錾
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTextureView;	//�|�C���^
};
