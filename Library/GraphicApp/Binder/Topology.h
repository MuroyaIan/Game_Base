/**
 * @file Topology.h
 * @brief �g�|���W�[�̏���
 * @author ���J�C�A��
 * @date 2022/06/25
 * @���� 2022/06/25�F�t�@�C���쐬
 *		 2022/11/21�F�������P
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/Binder.h>

//===== �N���X��` =====

//***** �g�|���W�[ *****
class TOPOLOGY : public BINDER
{
public:

	//�v���g�^�C�v�錾
	explicit TOPOLOGY(D3D11_PRIMITIVE_TOPOLOGY Type) noexcept;
	~TOPOLOGY() noexcept override;
	void Bind(const GRAPHIC& Gfx) const noexcept override;	//�o�C���h����

protected:

	//�ϐ��錾
	D3D11_PRIMITIVE_TOPOLOGY m_Type;						//�g�|���W�[�^�C�v
};
