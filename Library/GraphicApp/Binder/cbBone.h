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
	explicit CB_BONE(const GRAPHIC& Gfx, const CBD_BONE& aMtxBone, bool Transpose = false);
	~CB_BONE() noexcept override;
	void Bind(const GRAPHIC& Gfx) const noexcept override;		//�o�C���h����

private:

	//�ϐ��錾
	static std::unique_ptr<VS_CBUFFER<CBD_BONE>> m_pCBuffVS;	//�萔�o�b�t�@�̃|�C���^
	static int m_RefCount;										//�萔�o�b�t�@�̗��p��
	const CBD_BONE& m_aMtxBone;									//�����̒񋟐�
	bool m_bTransposeMatrix;									//�]�u�������s�t���O
};
