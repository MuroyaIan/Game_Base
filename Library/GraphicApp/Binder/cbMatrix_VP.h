/**
 * @file cbMatrix_VP.h
 * @brief �ϊ��s��̏���
 * @author ���J�C�A��
 * @date 2022/08/11
 * @���� 2022/08/11�F�t�@�C���쐬
 *		 2022/11/25�F�������P
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/ConstantBuffer.h>

//===== �\���̐錾 =====
struct CBD_MTX_VP
{
	//�ϐ��錾
	DirectX::XMFLOAT4X4 mtxView;	//�r���[�s��
	DirectX::XMFLOAT4X4 mtxProj;	//���e�s��

	CBD_MTX_VP() noexcept : mtxView(), mtxProj()
	{
		DirectX::XMStoreFloat4x4(&mtxView, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&mtxProj, DirectX::XMMatrixIdentity());
	}
	CBD_MTX_VP(DirectX::XMFLOAT4X4 mtxV, DirectX::XMFLOAT4X4 mtxP) noexcept : mtxView(mtxV), mtxProj(mtxP)
	{}
	~CBD_MTX_VP() noexcept
	{}
};

//===== �N���X��` =====

//***** �ϊ��s��o�b�t�@�i�r���[�s��A���e�s��j *****
class CB_MTX_VP : public BINDER
{
public:

	//�v���g�^�C�v�錾
	explicit CB_MTX_VP(const GRAPHIC& Gfx);
	~CB_MTX_VP() noexcept override;
	void Bind(const GRAPHIC& Gfx) const noexcept override;		//�o�C���h����

private:

	//�ϐ��錾
	static std::unique_ptr<VS_CBUFFER<CBD_MTX_VP>> m_pCBuffVS;	//�萔�o�b�t�@�̃|�C���^
	static int m_RefCount;										//�萔�o�b�t�@�̗��p��
};
