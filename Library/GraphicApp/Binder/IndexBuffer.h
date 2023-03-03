/**
 * @file IndexBuffer.h
 * @brief �C���f�b�N�X�o�b�t�@�̏���
 * @author ���J�C�A��
 * @date 2022/06/25
 * @���� 2022/06/25�F�t�@�C���쐬
 *		 2022/11/22�F�������P
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
	explicit INDEX_BUFFER(const GRAPHIC& Gfx, const std::vector<UINT>& aIndex);
	~INDEX_BUFFER() noexcept override;
	void Bind(const GRAPHIC& Gfx) const override;	//�o�C���h����

	UINT GetIndexNum() const noexcept				//�C���f�b�N�X���擾
	{
		return m_IndexNum;
	}

private:

	//�ϐ��錾
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;	//�|�C���^
	UINT m_IndexNum;										//�C���f�b�N�X��
};
