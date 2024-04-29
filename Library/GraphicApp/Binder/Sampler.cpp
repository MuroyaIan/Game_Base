
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/Sampler.h>

//===== �N���X���� =====
SAMPLER::SAMPLER(const CT_GRAPHIC& Gfx, UINT StartSlot, bool IsPixel) :
	CT_BINDER(), m_pSampler(), m_StartSlot(StartSlot)
{
	//�G���[�n���h��
	HRESULT hr{};

	//�T���v���[�쐬
	D3D11_SAMPLER_DESC SamplerDesc{};
	if (IsPixel)
		SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	else
		SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = GetDevice(Gfx)->CreateSamplerState(&SamplerDesc, &m_pSampler);
	ERROR_DX(hr);
}

SAMPLER::~SAMPLER() noexcept
{
}

//�o�C���h����
void SAMPLER::Bind(const CT_GRAPHIC& Gfx) const
{
	GetContext(Gfx)->PSSetSamplers(m_StartSlot, 1u, m_pSampler.GetAddressOf());
}
