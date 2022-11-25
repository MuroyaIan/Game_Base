/**
 * @file Binder.h
 * @brief DirectX�̃o�C���h����
 * @author ���J�C�A��
 * @date 2022/06/24
 * @���� 2022/06/24�F�t�@�C���쐬
 *		 2022/11/21�F�������P
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
	explicit BINDER() noexcept;
	virtual ~BINDER() noexcept;
	virtual void Bind(const GRAPHIC& Gfx) const noexcept = 0;	//�o�C���h����

protected:

	//�v���g�^�C�v�錾
	template <typename D>
	void MapBuffer(const GRAPHIC& Gfx, const D& Data, ID3D11Resource* pBuffer) const	//�o�b�t�@�X�V
	{
		//�G���[�n���h��
		HRESULT hr{};

		//�X�V����
		D3D11_MAPPED_SUBRESOURCE msr{};
		hr = GetContext(Gfx)->Map(
			pBuffer, 0u,
			D3D11_MAP_WRITE_DISCARD, 0u,
			&msr);								//GPU�̃A�N�Z�X�����b�N
		ERROR_DX(hr);
		memcpy(msr.pData, &Data, sizeof(D));	//�f�[�^������
		GetContext(Gfx)->Unmap(pBuffer, 0u);	//GPU�̃A�N�Z�X�����
	}

	template <typename D>
	void MapBuffer(const GRAPHIC& Gfx, const std::vector<D>& aData, ID3D11Resource* pBuffer) const	//�o�b�t�@�X�V(�z��)
	{
		//�G���[�n���h��
		HRESULT hr{};

		//�X�V����
		D3D11_MAPPED_SUBRESOURCE msr{};
		hr = GetContext(Gfx)->Map(
			pBuffer, 0u,
			D3D11_MAP_WRITE_DISCARD, 0u,
			&msr);													//GPU�̃A�N�Z�X�����b�N
		ERROR_DX(hr);
		memcpy(msr.pData, aData.data(), sizeof(D) * aData.size());	//�f�[�^������
		GetContext(Gfx)->Unmap(pBuffer, 0u);						//GPU�̃A�N�Z�X�����
	}

	static ID3D11Device* GetDevice(const GRAPHIC& Gfx) noexcept				//�f�o�C�X�擾
	{
		return Gfx.m_pDevice.Get();
	}

	static ID3D11DeviceContext* GetContext(const GRAPHIC& Gfx) noexcept		//�R���e�L�X�g�擾
	{
		return Gfx.m_pContext.Get();
	}
};
