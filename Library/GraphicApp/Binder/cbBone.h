/**
 * @file cbBone.h
 * @brief �����o�b�t�@
 * @author ���J�C�A��
 * @date 2022/09/28
 * @���� 2022/09/28�F�t�@�C���쐬
 *		 2022/11/25�F�������P
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/ConstantBuffer.h>

//===== �N���X��` =====

//***** �����o�b�t�@ *****
class CB_BONE : public BINDER
{
public:

	//�v���g�^�C�v�錾
	explicit CB_BONE(const CT_GRAPHIC& Gfx, CB_PTR* cbPtr, const CBD_BONE& aMtxBone, bool Transpose = false);
	~CB_BONE() noexcept override;
	void Bind(const CT_GRAPHIC& Gfx) const override;	//�o�C���h����(�f�[�^�X�V)

	void SetBuffPtr(CB_PTR* cbPtr) const noexcept	//�o�b�t�@�|�C���^�ݒ�
	{
		m_pCBuff->SetBuffPtr(cbPtr);
	}

private:

	//�ϐ��錾
	static std::unique_ptr<CONSTANT_BUFFER<CBD_BONE>> m_pCBuff;		//�萔�o�b�t�@�̃|�C���^
	static int m_RefCount;											//�萔�o�b�t�@�̗��p��
	const CBD_BONE& m_aMtxBone;										//�����̒񋟐�
	bool m_bTransposeMatrix;										//�]�u�������s�t���O
};
