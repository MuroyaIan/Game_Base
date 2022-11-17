
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/Sampler.h>

//===== �N���X���� =====
SAMPLER::SAMPLER(GRAPHIC& Gfx) :
	BINDER(), m_pSampler()
{
	//�G���[�n���h��
	HRESULT hr{};

	//�T���v���[�쐬
	D3D11_SAMPLER_DESC SamplerDesc{};
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
void SAMPLER::Bind(GRAPHIC& Gfx) noexcept
{
	GetContext(Gfx)->PSSetSamplers(0, 1, m_pSampler.GetAddressOf());
}
