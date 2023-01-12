/**
 * @file cbLocal.h
 * @brief �����o�b�t�@
 * @author ���J�C�A��
 * @date 2022/09/29
 * @���� 2022/09/29�F�t�@�C���쐬
 *		 2022/11/25�F�������P
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
	explicit CB_LOCAL(const GRAPHIC& Gfx, CB_PTR* cbPtr, const CBD_MTX_LOCAL& LocalData);
	~CB_LOCAL() noexcept override;
	void Bind(const GRAPHIC& Gfx) const noexcept override;				//�o�C���h����

	void SetBuffPtr(CB_PTR* cbPtr) const noexcept						//�o�b�t�@�|�C���^�ݒ�
	{
		m_pCBuff->SetBuffPtr(cbPtr);
	}

private:

	//�ϐ��錾
	static std::unique_ptr<CONSTANT_BUFFER<CBD_MTX_LOCAL>> m_pCBuff;	//�萔�o�b�t�@�̃|�C���^
	static int m_RefCount;												//�萔�o�b�t�@�̗��p��
	const CBD_MTX_LOCAL& m_LocalData;									//�����̒񋟐�
};
