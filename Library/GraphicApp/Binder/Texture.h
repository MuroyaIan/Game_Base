/**
 * @file Texture.h
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
class TEXTURE : public BINDER
{
public:

	//�v���g�^�C�v�錾
	explicit TEXTURE(const GRAPHIC& Gfx, const TEX_LOADER::TEX_DATA& Data, UINT StartSlot = 0u);
	~TEXTURE() noexcept override;
	void Bind(const GRAPHIC& Gfx) const noexcept override;				//�o�C���h����

protected:

	//�ϐ��錾
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTextureView;	//�|�C���^
	UINT m_StartSlot;													//���W�X�^�ԍ�
};
