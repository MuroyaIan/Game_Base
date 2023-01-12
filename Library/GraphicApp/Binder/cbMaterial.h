/**
 * @file cbMaterial.h
 * @brief �}�e���A���o�b�t�@
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
struct CBD_MATERIAL;

//===== �N���X��` =====

//***** �}�e���A���o�b�t�@ *****
class CB_MATERIAL : public BINDER
{
public:

	//�v���g�^�C�v�錾
	explicit CB_MATERIAL(const GRAPHIC& Gfx, CB_PTR* cbPtr, const CBD_MATERIAL& Material);
	~CB_MATERIAL() noexcept override;
	void Bind(const GRAPHIC& Gfx) const noexcept override;				//�o�C���h����

	void SetBuffPtr(CB_PTR* cbPtr) const noexcept						//�o�b�t�@�|�C���^�ݒ�
	{
		m_pCBuff->SetBuffPtr(cbPtr);
	}

private:

	//�ϐ��錾
	static std::unique_ptr<CONSTANT_BUFFER<CBD_MATERIAL>> m_pCBuff;		//�萔�o�b�t�@�̃|�C���^
	static int m_RefCount;												//�萔�o�b�t�@�̗��p��
	const CBD_MATERIAL& m_Material;										//�}�e���A�����̒񋟐�
};
