/**
 * @file InputLayout.h
 * @brief ���̓��C�A�E�g�̏���
 * @author ���J�C�A��
 * @date 2022/06/25
 * @���� 2022/06/25�F�t�@�C���쐬
 *		 2022/11/21�F�������P
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/Binder.h>

//===== �N���X��` =====

//***** ���̓��C�A�E�g *****
class INPUT_LAYOUT : public BINDER
{
public:

	//�v���g�^�C�v�錾
	explicit INPUT_LAYOUT(GRAPHIC& Gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& aLayout, ID3DBlob* pCodeVS);
	~INPUT_LAYOUT() noexcept override;
	void Bind(GRAPHIC& Gfx) noexcept override;					//�o�C���h����

protected:

	//�ϐ��錾
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;	//�|�C���^
};
