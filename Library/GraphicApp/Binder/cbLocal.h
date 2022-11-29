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

//===== �\���̐錾 =====
struct CBD_MTX_LOCAL
{
	//�ϐ��錾
	DirectX::XMFLOAT4X4 mtxSkin;	//���Ȃ����b�V���p���[�J���s��

	CBD_MTX_LOCAL() noexcept : mtxSkin()
	{
		DirectX::XMStoreFloat4x4(&mtxSkin, DirectX::XMMatrixIdentity());
	}

	~CBD_MTX_LOCAL() noexcept
	{}
};

//===== �N���X��` =====

//***** �����o�b�t�@ *****
class CB_LOCAL : public BINDER
{
public:

	//�v���g�^�C�v�錾
	explicit CB_LOCAL(const GRAPHIC& Gfx, const CBD_MTX_LOCAL& LocalData);
	~CB_LOCAL() noexcept override;
	void Bind(const GRAPHIC& Gfx) const noexcept override;			//�o�C���h����

private:

	//�ϐ��錾
	static std::unique_ptr<VS_CBUFFER<CBD_MTX_LOCAL>> m_pCBuffVS;	//�萔�o�b�t�@�̃|�C���^
	static int m_RefCount;											//�萔�o�b�t�@�̗��p��
	const CBD_MTX_LOCAL& m_LocalData;								//�����̒񋟐�
};
