/**
 * @file cbBone.h
 * @brief �����o�b�t�@
 * @author ���J�C�A��
 * @date 2022/09/28
 * @���� 2022/09/28�F�t�@�C���쐬
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
	CB_BONE(GRAPHIC& Gfx, const CB_MTX_BONE& aMtxBone, bool Transpose = false);
	~CB_BONE() noexcept override;
	void Bind(GRAPHIC& Gfx) noexcept override;						//�o�C���h����

private:

	//�ϐ��錾
	static std::unique_ptr<VERTEX_CBUFFER<CB_MTX_BONE>> m_pVcBuff;	//�萔�o�b�t�@�̃|�C���^
	const CB_MTX_BONE& m_aMtxBone;									//�����̒񋟐�
	bool m_bTransposeMatrix;										//�]�u�������s�t���O
};
