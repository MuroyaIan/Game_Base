
//===== インクルード部 =====
#include <GraphicApp/Binder/Texture_Model.h>

//===== クラス実装 =====
TEXTURE_MODEL::TEXTURE_MODEL(GRAPHIC& Gfx, std::vector<TEX_LOADER::TEX_DATA>& aData) : BINDER(), m_aTextureViewPtr(static_cast<int>(TEX_TYPE::MAX_TYPE))
{
	//バッファ作成
	for (size_t i = 0, Cnt = m_aTextureViewPtr.size(); i < Cnt; i++)
		MakeBuffer(Gfx, aData[i], static_cast<TEX_TYPE>(i));
}

TEXTURE_MODEL::~TEXTURE_MODEL() noexcept
{
	//メモリ解放
	m_aTextureViewPtr.clear();
}

//バインド処理
void TEXTURE_MODEL::Bind(GRAPHIC& Gfx) noexcept
{
	ID3D11ShaderResourceView* aBufferPtr[static_cast<int>(TEX_TYPE::MAX_TYPE)]{ nullptr };
	for (size_t i = 0, Cnt = m_aTextureViewPtr.size(); i < Cnt; i++)
		aBufferPtr[i] = m_aTextureViewPtr[i].Get();
	GetContext(Gfx)->PSSetShaderResources(1u, static_cast<UINT>(m_aTextureViewPtr.size()), aBufferPtr);
}

//バッファ作成
void TEXTURE_MODEL::MakeBuffer(GRAPHIC& Gfx, TEX_LOADER::TEX_DATA& Data, TEX_TYPE Type)
{
	//エラーハンドル
	HRESULT hr{};

	//バッファ作成
	if (Data.pImageData != nullptr) {

		//テクスチャリソース作成
		D3D11_TEXTURE2D_DESC TextureDesc{};
		TextureDesc.Width = static_cast<UINT>(Data.nWidth);
		TextureDesc.Height = static_cast<UINT>(Data.nHeight);
		TextureDesc.MipLevels = 1;
		TextureDesc.ArraySize = 1;
		TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		TextureDesc.SampleDesc.Count = 1;
		TextureDesc.SampleDesc.Quality = 0;
		TextureDesc.Usage = D3D11_USAGE_DEFAULT;
		TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		TextureDesc.CPUAccessFlags = 0;
		TextureDesc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = Data.pImageData;
		sd.SysMemPitch = Data.nWidth * sizeof(uint32_t);
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
		hr = GetDevice(Gfx)->CreateTexture2D(&TextureDesc, &sd, &pTexture);
		ERROR_DX(hr);

		//リソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = TextureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		hr = GetDevice(Gfx)->CreateShaderResourceView(pTexture.Get(), &srvDesc, &m_aTextureViewPtr[static_cast<int>(Type)]);
		ERROR_DX(hr);
	}
	else
		throw ERROR_EX2("テクスチャのポインタはNULLです。");
}
