
//===== インクルード部 =====
#include <GraphicApp/Binder/Sampler.h>

//===== クラス実装 =====
SAMPLER::SAMPLER(const CT_GRAPHIC& Gfx, UINT StartSlot, bool IsPixel) :
	CT_BINDER(), m_pSampler(), m_StartSlot(StartSlot)
{
	//エラーハンドル
	HRESULT hr{};

	//サンプラー作成
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

//バインド処理
void SAMPLER::Bind(const CT_GRAPHIC& Gfx) const
{
	GetContext(Gfx)->PSSetSamplers(m_StartSlot, 1u, m_pSampler.GetAddressOf());
}
