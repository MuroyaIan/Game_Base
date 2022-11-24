/**
 * @file cbMatrix_VP.h
 * @brief �ϊ��s��̏���
 * @author ���J�C�A��
 * @date 2022/08/11
 * @���� 2022/08/11�F�t�@�C���쐬
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
	CB_MTX_VP(GRAPHIC& Gfx);
	~CB_MTX_VP() noexcept override;
	void Bind(const GRAPHIC& Gfx) noexcept override;		//�o�C���h����

private:

	//�ϐ��錾
	static std::unique_ptr<VERTEX_CBUFFER<CBD_MTX_VP>> m_pVcBuff;	//�萔�o�b�t�@�̃|�C���^
	static int m_RefCount;											//�萔�o�b�t�@�̗��p��
};
