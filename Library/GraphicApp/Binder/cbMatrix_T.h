/**
 * @file cbTransform.h
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
class CB_MTX_T : public BINDER
{
public:

	//�v���g�^�C�v�錾
	CB_MTX_T(GRAPHIC& Gfx, const DRAWER& Parent);
	~CB_MTX_T() noexcept override;
	void Bind(const GRAPHIC& Gfx) const noexcept override;					//�o�C���h����

private:

	//�ϐ��錾
	static std::unique_ptr<VERTEX_CBUFFER<DirectX::XMFLOAT4X4>> m_pVcBuff;	//�萔�o�b�t�@�̃|�C���^
	static int m_RefCount;													//�萔�o�b�t�@�̗��p��
	const DRAWER& m_Parent;													//��������h�����[�ihas-a�j
};
