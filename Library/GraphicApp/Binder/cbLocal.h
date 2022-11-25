/**
 * @file cbLocal.h
 * @brief �����o�b�t�@
 * @author ���J�C�A��
 * @date 2022/09/29
 * @���� 2022/09/29�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/ConstantBuffer.h>

//===== �N���X��` =====

//***** �����o�b�t�@ *****
class CB_LOCAL : public BINDER
{
public:

	//�v���g�^�C�v�錾
	CB_LOCAL(GRAPHIC& Gfx, const CBD_MTX_LOCAL& LocalData);
	~CB_LOCAL() noexcept override;
	void Bind(const GRAPHIC& Gfx) const noexcept override;				//�o�C���h����

private:

	//�ϐ��錾
	static std::unique_ptr<VERTEX_CBUFFER<CBD_MTX_LOCAL>> m_pVcBuff;	//�萔�o�b�t�@�̃|�C���^
	static int m_RefCount;												//�萔�o�b�t�@�̗��p��
	const CBD_MTX_LOCAL& m_LocalData;									//�����̒񋟐�
};
