/**
 * @file IndexBuffer.h
 * @brief �C���f�b�N�X�o�b�t�@�̏���
 * @author ���J�C�A��
 * @date 2022/06/25
 * @���� 2022/06/25�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/Binder.h>

//===== �N���X��` =====

//***** �C���f�b�N�X�o�b�t�@ *****
class INDEX_BUFFER : public BINDER
{
public:

	//�v���g�^�C�v�錾
	INDEX_BUFFER(GRAPHIC& Gfx, const std::vector<unsigned short>& Indices);
	~INDEX_BUFFER() noexcept override;
	void Bind(GRAPHIC& Gfx) noexcept override;				//�o�C���h����

	UINT GetIndexNum() const noexcept						//�C���f�b�N�X���擾
	{
		return m_Count;
	}

protected:

	//�ϐ��錾
	UINT m_Count;											//�C���f�b�N�X��
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;	//�|�C���^
};
