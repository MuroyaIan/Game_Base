/**
 * @file PixelShader.h
 * @brief �s�N�Z���V�F�[�_�̏���
 * @author ���J�C�A��
 * @date 2022/06/25
 * @���� 2022/06/25�F�t�@�C���쐬
 *		 2022/11/24�F�������P
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/Binder.h>

//===== �N���X��` =====

//***** �s�N�Z���V�F�[�_ *****
class PIXEL_SHADER : public BINDER
{
public:

	//�v���g�^�C�v�錾
	explicit PIXEL_SHADER(const CT_GRAPHIC& Gfx, const std::wstring& Path);
	~PIXEL_SHADER() noexcept override;
	void Bind(const CT_GRAPHIC& Gfx) const override;	//�o�C���h����

private:

	//�ϐ��錾
	ComPtr<ID3D11PixelShader> m_pPixelShader;	//�|�C���^
};
