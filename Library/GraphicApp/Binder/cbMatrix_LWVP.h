/**
 * @file cbMatrix_LWVP.h
 * @brief �ϊ��s��̏���
 * @author ���J�C�A��
 * @date 2022/06/25
 * @���� 2022/06/25�F�t�@�C���쐬
 *		 2022/11/25�F�������P
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/ConstantBuffer.h>

//===== �O���錾 =====
class DRAWER;

//===== �N���X��` =====

//***** �ϊ��s��o�b�t�@ *****
class CB_MTX_LWVP : public BINDER
{
public:

	//�v���g�^�C�v�錾
	explicit CB_MTX_LWVP(const CT_GRAPHIC& Gfx, CB_PTR* cbPtr, const DRAWER& Parent, const DirectX::XMFLOAT4X4& mtxL);
	~CB_MTX_LWVP() noexcept override;
	void Bind(const CT_GRAPHIC& Gfx) const override;	//�o�C���h����(�f�[�^�X�V)

	void SetBuffPtr(CB_PTR* cbPtr) const noexcept	//�o�b�t�@�|�C���^�ݒ�
	{
		m_pCBuff->SetBuffPtr(cbPtr);
	}

private:

	//�ϐ��錾
	static std::unique_ptr<CONSTANT_BUFFER<CBD_MTX_LWVP>> m_pCBuff;		//�萔�o�b�t�@�̃|�C���^
	static int m_RefCount;												//�萔�o�b�t�@�̗��p��
	const DRAWER& m_Parent;												//��������h�����[�ihas-a�j
	const DirectX::XMFLOAT4X4& m_mtxL;									//���[�J���s��̒񋟐�
};
