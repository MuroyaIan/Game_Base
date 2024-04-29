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
class CT_BINDER
{
public:

	//�R�s�[�����[�u
	CT_BINDER(const CT_BINDER&) = delete;
	CT_BINDER& operator =(const CT_BINDER&) = delete;
	CT_BINDER(CT_BINDER&&) noexcept = delete;
	CT_BINDER& operator=(CT_BINDER&&) noexcept = delete;

	//�v���g�^�C�v�錾
	explicit CT_BINDER() noexcept;
	virtual ~CT_BINDER() noexcept;
	virtual void Bind(const CT_GRAPHIC& gfx) const = 0;	//�o�C���h����

protected:

	//�v���g�^�C�v�錾
	template <typename t_Data>
	static void MapBuffer(const CT_GRAPHIC& gfx, const t_Data& data, ID3D11Resource* pBuffer) //�o�b�t�@�X�V
	{
		//��O����
		if (pBuffer == nullptr)
			throw ERROR_EX2("�|�C���^(pBuffer)��NULL�ł��B");

		//�G���[�n���h��
		HRESULT l_hr{};

		//�X�V����
		D3D11_MAPPED_SUBRESOURCE l_msr{};
		l_hr = GetContext(gfx)->Map(
			pBuffer, 0u,
			D3D11_MAP_WRITE_DISCARD, 0u,
			&l_msr);								//GPU�̃A�N�Z�X�����b�N
		ERROR_DX(l_hr);
		memcpy(l_msr.pData, &data, sizeof(t_Data));	//�f�[�^������
		GetContext(gfx)->Unmap(pBuffer, 0u);	//GPU�̃A�N�Z�X�����
	}

	template <typename t_Data>
	static void MapBuffer(const CT_GRAPHIC& gfx, const std::vector<t_Data>& aData, ID3D11Resource* pBuffer) //�o�b�t�@�X�V(�z��)
	{
		//��O����
		if (pBuffer == nullptr)
			throw ERROR_EX2("�|�C���^(pBuffer)��NULL�ł�");

		//�G���[�n���h��
		HRESULT l_hr{};

		//�X�V����
		D3D11_MAPPED_SUBRESOURCE l_msr{};
		l_hr = GetContext(gfx)->Map(
			pBuffer, 0u,
			D3D11_MAP_WRITE_DISCARD, 0u,
			&l_msr);													//GPU�̃A�N�Z�X�����b�N
		ERROR_DX(l_hr);
		memcpy(l_msr.pData, aData.data(), sizeof(t_Data) * aData.size());	//�f�[�^������
		GetContext(gfx)->Unmap(pBuffer, 0u);						//GPU�̃A�N�Z�X�����
	}

	static ID3D11Device* GetDevice(const CT_GRAPHIC& gfx)				//�f�o�C�X�擾
	{
		//��O����
		if (gfx.m_pDevice == nullptr)
			throw ERROR_EX2("�|�C���^(m_pDevice)��NULL�ł�");

		return gfx.m_pDevice.Get();
	}

	static ID3D11DeviceContext* GetContext(const CT_GRAPHIC& gfx)		//�R���e�L�X�g�擾
	{
		//��O����
		if (gfx.m_pContext == nullptr)
			throw ERROR_EX2("�|�C���^(m_pDevice)��NULL�ł�");

		return gfx.m_pContext.Get();
	}
};
