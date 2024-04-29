
//===== インクルード部 =====
#include <GraphicApp/Binder/Texture_Model.h>

//===== クラス実装 =====
TEXTURE_MODEL::TEXTURE_MODEL(const CT_GRAPHIC& Gfx, const std::vector<TEX_LOADER::TEX_DATA>& aData, UINT StartSlot) :
	CT_BINDER(), m_pTextureViews(static_cast<int>(TEX_TYPE::MaxType)), m_StartSlot(StartSlot)
{
	//バッファ作成
	for (size_t i = 0, Cnt = m_pTextureViews.size(); i < Cnt; i++)
		MakeBuffer(Gfx, aData[i], static_cast<TEX_TYPE>(i));
}

TEXTURE_MODEL::~TEXTURE_MODEL() noexcept
{
}

//バインド処理
void TEXTURE_MODEL::Bind(const CT_GRAPHIC& Gfx) const
{
	std::vector<ID3D11ShaderResourceView*> pBuffers(0);
	for (auto& p : m_pTextureViews)
		pBuffers.emplace_back(p.Get());

	GetContext(Gfx)->PSSetShaderResources(m_StartSlot, static_cast<UINT>(pBuffers.size()), pBuffers.data());
}

//バッファ作成
void TEXTURE_MODEL::MakeBuffer(const CT_GRAPHIC& Gfx, const TEX_LOADER::TEX_DATA& Data, TEX_TYPE Type)
{
	//エラーハンドル
	HRESULT hr{};

	//バッファ作成
	if (Data.pImageData != nullptr) {

		//テクスチャリソース作成
		D3D11_TEXTURE2D_DESC td{};
		td.Width = static_cast<UINT>(Data.nWidth);
		td.Height = static_cast<UINT>(Data.nHeight);
		td.MipLevels = 1u;
		td.ArraySize = 1u;
		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		td.SampleDesc.Count = 1u;
		td.SampleDesc.Quality = 0u;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0u;
		td.MiscFlags = 0u;
		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = Data.pImageData;
		sd.SysMemPitch = Data.nWidth * sizeof(uint32_t);
		ComPtr<ID3D11Texture2D> pTexture;
		hr = GetDevice(Gfx)->CreateTexture2D(&td, &sd, &pTexture);
		ERROR_DX(hr);

		//リソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd{};
		srvd.Format = td.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MostDetailedMip = 0u;
		srvd.Texture2D.MipLevels = 1u;
		hr = GetDevice(Gfx)->CreateShaderResourceView(pTexture.Get(), &srvd, &m_pTextureViews[static_cast<int>(Type)]);
		ERROR_DX(hr);
	}
	else
		throw ERROR_EX2("テクスチャのポインタはNULLです。");
}
