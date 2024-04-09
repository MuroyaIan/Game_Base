/**
 * @file Sampler.h
 * @brief �T���v���[�̏���
 * @author ���J�C�A��
 * @date 2022/07/16
 * @���� 2022/07/16�F�t�@�C���쐬
 *		 2022/11/24�F�������P
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/Binder.h>

//===== �N���X��` =====

//***** �T���v���[ *****
class SAMPLER : public BINDER
{
public:

	//�v���g�^�C�v�錾
	explicit SAMPLER(const CT_GRAPHIC& Gfx, UINT StartSlot = 0u, bool IsPixel = false);
	~SAMPLER() noexcept override;
	void Bind(const CT_GRAPHIC& Gfx) const override;	//�o�C���h����

private:

	//�ϐ��錾
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSampler;	//�|�C���^
	UINT m_StartSlot;										//���W�X�^�ԍ�
};
