/**
 * @file cbMgr.h
 * @brief �萔�o�b�t�@�}�l�[�W��
 * @author ���J�C�A��
 * @date 2023/01/13
 * @���� 2023/01/13�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/ConstantBuffer.h>

//===== �N���X��` =====
class CBUFF_MGR : public BINDER
{
public:

	//�v���g�^�C�v�錾
	explicit CBUFF_MGR(CB_PTR& cbPtrRef) noexcept;
	~CBUFF_MGR() noexcept override;

	void Bind(const GRAPHIC& Gfx) const noexcept override;	//�o�C���h����

private:

	//�ϐ��錾
	std::vector<ID3D11Buffer*> m_aBuffPtrVS;
	std::vector<ID3D11Buffer*> m_aBuffPtrPS;	//�萔�o�b�t�@�̃|�C���^�z��
	UINT m_BuffSizeVS;
	UINT m_BuffSizePS;							//�萔�o�b�t�@�̃T�C�Y
};
