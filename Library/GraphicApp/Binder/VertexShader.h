/**
 * @file VertexShader.h
 * @brief ���_�V�F�[�_�̏���
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

//***** ���_�V�F�[�_ *****
class VERTEX_SHADER : public BINDER
{
public:

	//�v���g�^�C�v�錾
	explicit VERTEX_SHADER(const CT_GRAPHIC& Gfx, const std::wstring& Path);
	~VERTEX_SHADER() noexcept override;
	void Bind(const CT_GRAPHIC& Gfx) const override;	//�o�C���h����

	ID3DBlob* GetBytecode() const					//�V�F�[�_�t�@�C���擾
	{
		//��O����
		if (m_pBlob == nullptr)
			throw ERROR_EX2("�|�C���^��NULL�ł��B");

		return m_pBlob.Get();
	}

private:

	//�ϐ��錾
	ComPtr<ID3DBlob> m_pBlob;						//�V�F�[�_���\�[�X
	ComPtr<ID3D11VertexShader> m_pVertexShader;		//�|�C���^
};
