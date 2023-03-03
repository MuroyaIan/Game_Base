/**
 * @file cbMatrix_VP.h
 * @brief �ϊ��s��̏���
 * @author ���J�C�A��
 * @date 2022/08/11
 * @���� 2022/08/11�F�t�@�C���쐬
 *		 2022/11/25�F�������P
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/ConstantBuffer.h>

//===== �N���X��` =====

//***** �ϊ��s��o�b�t�@�i�r���[�s��A���e�s��j *****
class CB_MTX_VP : public BINDER
{
public:

	//�v���g�^�C�v�錾
	explicit CB_MTX_VP(const GRAPHIC& Gfx, CB_PTR* cbPtr);
	~CB_MTX_VP() noexcept override;
	void Bind(const GRAPHIC& Gfx) const override;	//�o�C���h����(�f�[�^�X�V)

	void SetBuffPtr(CB_PTR* cbPtr) const noexcept	//�o�b�t�@�|�C���^�ݒ�
	{
		m_pCBuff->SetBuffPtr(cbPtr);
	}

private:

	//�ϐ��錾
	static std::unique_ptr<CONSTANT_BUFFER<CBD_MTX_VP>> m_pCBuff;	//�萔�o�b�t�@�̃|�C���^
	static int m_RefCount;											//�萔�o�b�t�@�̗��p��
};
