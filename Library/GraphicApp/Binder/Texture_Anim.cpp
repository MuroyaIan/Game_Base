
//===== インクルード部 =====
#include <GraphicApp/Binder/Texture_Anim.h>

//===== クラス実装 =====
TEXTURE_ANIM::TEXTURE_ANIM(const GRAPHIC& Gfx, const TEX_DATA_ANIM& Data, UINT StartSlot) :
	BINDER(), m_pTextureView(), m_StartSlot(StartSlot)
{
	//エラーハンドル
	HRESULT hr{};

	//バッファ作成
	if (Data.pAnimData != nullptr) {

		//テクスチャリソース作成
		D3D11_TEXTURE2D_DESC td{};
		td.Width = static_cast<UINT>(Data.nWidth) * 4u;		//アニメーション数 * 個々のフレーム数 * float4（1テクセル）
		td.Height = static_cast<UINT>(Data.nHeight);
		td.MipLevels = 1u;
		td.ArraySize = 1u;
		td.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		td.SampleDesc.Count = 1u;
		td.SampleDesc.Quality = 0u;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0u;
		td.MiscFlags = 0u;
		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = Data.pAnimData;
		sd.SysMemPitch = Data.nWidth * sizeof(float) * 16u;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
		hr = GetDevice(Gfx)->CreateTexture2D(&td, &sd, &pTexture);
		ERROR_DX(hr);

		//リソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd{};
		srvd.Format = td.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MostDetailedMip = 0u;
		srvd.Texture2D.MipLevels = 1u;
		hr = GetDevice(Gfx)->CreateShaderResourceView(pTexture.Get(), &srvd, &m_pTextureView);
		ERROR_DX(hr);
	}
	else
		throw ERROR_EX2("テクスチャのポインタはNULLです。");
}

TEXTURE_ANIM::~TEXTURE_ANIM() noexcept
{
}

//バインド処理
void TEXTURE_ANIM::Bind(const GRAPHIC& Gfx) const
{
	GetContext(Gfx)->PSSetShaderResources(m_StartSlot, 1u, m_pTextureView.GetAddressOf());
}
