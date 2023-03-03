/**
 * @file Texture_Anim.h
 * @brief �e�N�X�`���i�A�j���[�V�������j
 * @author ���J�C�A��
 * @date 2023/02/20
 * @���� 2023/02/20�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/Binder.h>
#include <GraphicApp/Drawer/VertexShaderData.h>

//===== �\���̐錾 =====
struct TEX_DATA_ANIM
{
	float* pAnimData;	//�f�[�^�|�C���^
	int nWidth;			//�e�N�X�`�����i�S�A�j���[�V�����̃t���[�����̍��v�j
	int nHeight;		//�e�N�X�`�������i�ő卜���j

	explicit TEX_DATA_ANIM() noexcept : pAnimData(nullptr), nWidth(0), nHeight(MAX_BONE)
	{}
	~TEX_DATA_ANIM() noexcept {}
};

//===== �N���X��` =====
class TEXTURE_ANIM : public BINDER
{
public:

	//�v���g�^�C�v�錾
	explicit TEXTURE_ANIM(const GRAPHIC& Gfx, const TEX_DATA_ANIM& Data, UINT StartSlot = 0u);
	~TEXTURE_ANIM() noexcept override;
	void Bind(const GRAPHIC& Gfx) const noexcept override;				//�o�C���h����

	ID3D11ShaderResourceView* GetSrvPtr() const							//�|�C���^�Q��
	{
		//��O����
		if (m_pTextureView == nullptr)
			throw ERROR_EX2("�|�C���^��NULL�ł��B");

		return m_pTextureView.Get();
	}

private:

	//�ϐ��錾
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTextureView;	//�|�C���^
	UINT m_StartSlot;													//���W�X�^�ԍ�
};
