/**
 * @file cbMatrix_LWVP.h
 * @brief �ϊ��s��̏���
 * @author ���J�C�A��
 * @date 2022/06/25
 * @���� 2022/06/25�F�t�@�C���쐬
 *		 2022/11/25�F�������P
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/ConstantBuffer.h>

//===== �\���̐錾 =====
struct CBD_MTX_LWVP
{
	//�ϐ��錾
	DirectX::XMFLOAT4X4 mtxLocal;	//���[�J���s��
	DirectX::XMFLOAT4X4 mtxWorld;	//���[���h�s��
	DirectX::XMFLOAT4X4 mtxView;	//�r���[�s��
	DirectX::XMFLOAT4X4 mtxProj;	//���e�s��

	CBD_MTX_LWVP() noexcept : mtxLocal(), mtxWorld(), mtxView(), mtxProj()
	{
		DirectX::XMStoreFloat4x4(&mtxLocal, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&mtxWorld, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&mtxView, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&mtxProj, DirectX::XMMatrixIdentity());
	}

	CBD_MTX_LWVP(DirectX::XMFLOAT4X4 mtxL, DirectX::XMFLOAT4X4 mtxW, DirectX::XMFLOAT4X4 mtxV, DirectX::XMFLOAT4X4 mtxP) noexcept :
		mtxLocal(mtxL), mtxWorld(mtxW), mtxView(mtxV), mtxProj(mtxP)
	{}

	~CBD_MTX_LWVP() noexcept
	{}
};

//===== �O���錾 =====
class DRAWER;

//===== �N���X��` =====

//***** �ϊ��s��o�b�t�@ *****
class CB_MTX_LWVP : public BINDER
{
public:

	//�v���g�^�C�v�錾
	explicit CB_MTX_LWVP(const GRAPHIC& Gfx, const DRAWER& Parent, const DirectX::XMFLOAT4X4& mtxL);
	~CB_MTX_LWVP() noexcept override;
	void Bind(const GRAPHIC& Gfx) const noexcept override;			//�o�C���h����

private:

	//�ϐ��錾
	static std::unique_ptr<VS_CBUFFER<CBD_MTX_LWVP>> m_pCBuffVS;	//�萔�o�b�t�@�̃|�C���^
	static int m_RefCount;											//�萔�o�b�t�@�̗��p��
	const DRAWER& m_Parent;											//��������h�����[�ihas-a�j
	const DirectX::XMFLOAT4X4& m_mtxL;								//���[�J���s��̒񋟐�
};
