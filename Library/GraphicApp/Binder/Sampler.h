/**
 * @file Sampler.h
 * @brief �T���v���[�̏���
 * @author ���J�C�A��
 * @date 2022/07/16
 * @���� 2022/07/16�F�t�@�C���쐬
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
	SAMPLER(GRAPHIC& Gfx);
	~SAMPLER() noexcept override;
	void Bind(GRAPHIC& Gfx) noexcept override;			//�o�C���h����

protected:

	//�ϐ��錾
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSampler;	//�|�C���^
};
