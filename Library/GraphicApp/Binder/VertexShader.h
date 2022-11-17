/**
 * @file VertexShader.h
 * @brief ���_�V�F�[�_�̏���
 * @author ���J�C�A��
 * @date 2022/06/25
 * @���� 2022/06/25�F�t�@�C���쐬
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
	VERTEX_SHADER(GRAPHIC& Gfx, const std::wstring& Path);
	~VERTEX_SHADER() noexcept override;
	void Bind(GRAPHIC& Gfx) noexcept override;						//�o�C���h����

	ID3DBlob* GetBytecode() const noexcept							//�V�F�[�_�t�@�C���擾
	{
		return m_pBytecodeBlob.Get();
	}

protected:

	//�ϐ��錾
	Microsoft::WRL::ComPtr<ID3DBlob> m_pBytecodeBlob;				//�V�F�[�_�t�@�C���Ǎ��ݗp
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;		//�|�C���^
};
