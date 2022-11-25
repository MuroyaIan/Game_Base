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
struct MATERIAL_DATA;

//===== �N���X��` =====

//***** �}�e���A���o�b�t�@ *****
class CB_MATERIAL : public BINDER
{
public:

	//�v���g�^�C�v�錾
	explicit CB_MATERIAL(const GRAPHIC& Gfx, const MATERIAL_DATA& Material);
	~CB_MATERIAL() noexcept override;
	void Bind(const GRAPHIC& Gfx) const noexcept override;			//�o�C���h����

private:

	//�ϐ��錾
	static std::unique_ptr<PS_CBUFFER<MATERIAL_DATA>> m_pCBuffPS;	//�萔�o�b�t�@�̃|�C���^
	static int m_RefCount;											//�萔�o�b�t�@�̗��p��
	const MATERIAL_DATA& m_Material;								//�}�e���A�����̒񋟐�
};
