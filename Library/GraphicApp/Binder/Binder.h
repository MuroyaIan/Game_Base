/**
 * @file Binder.h
 * @brief DirectX�̃o�C���h����
 * @author ���J�C�A��
 * @date 2022/06/24
 * @���� 2022/06/24�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Graphic.h>

//===== �N���X��` =====

//***** �o�C���_ *****
class BINDER
{
public:

	//�v���g�^�C�v�錾
	BINDER() noexcept;
	virtual ~BINDER() noexcept;
	virtual void Bind(GRAPHIC& Gfx) noexcept = 0;					//�o�C���h����

protected:

	//�v���g�^�C�v�錾
	static ID3D11Device* GetDevice(GRAPHIC& Gfx) noexcept			//�f�o�C�X�擾
	{
		return Gfx.m_pDevice.Get();
	}

	static ID3D11DeviceContext* GetContext(GRAPHIC& Gfx) noexcept	//�R���e�L�X�g�擾
	{
		return Gfx.m_pContext.Get();
	}
};
