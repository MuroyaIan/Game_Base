/**
 * @file cbMatrix_LWVP.h
 * @brief �ϊ��s��̏���
 * @author ���J�C�A��
 * @date 2022/06/25
 * @���� 2022/06/25�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/ConstantBuffer.h>
#include <GraphicApp/Drawer/Drawer.h>

//===== �N���X��` =====

//***** �ϊ��s��o�b�t�@ *****
class CB_MTX_LWVP : public BINDER
{
public:

	//�v���g�^�C�v�錾
	CB_MTX_LWVP(GRAPHIC& Gfx, const DRAWER& Parent, const DirectX::XMFLOAT4X4& mtxL);
	~CB_MTX_LWVP() noexcept override;
	void Bind(GRAPHIC& Gfx) noexcept override;							//�o�C���h����

private:

	//�ϐ��錾
	static std::unique_ptr<VERTEX_CBUFFER<CB_MTX_L_W_V_P>> m_pVcBuff;	//�萔�o�b�t�@�̃|�C���^
	const DRAWER& m_Parent;												//��������h�����[�ihas-a�j
	const DirectX::XMFLOAT4X4& m_mtxL;									//���[�J���s��̒񋟐�
};
