
//===== インクルード部 =====
#include <GraphicApp/Binder/PixelShader.h>

//===== クラス実装 =====
PIXEL_SHADER::PIXEL_SHADER(GRAPHIC& Gfx, const std::wstring& Path) :
	BINDER(), m_pPixelShader()
{
	//エラーハンドル
	HRESULT hr{};

	//シェーダ作成
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	hr = D3DReadFileToBlob(Path.c_str(), &pBlob);
	ERROR_DX(hr);
	hr = GetDevice(Gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pPixelShader);
	ERROR_DX(hr);
}

PIXEL_SHADER::~PIXEL_SHADER() noexcept
{
}

//バインド処理
void PIXEL_SHADER::Bind(GRAPHIC& Gfx) noexcept
{
	GetContext(Gfx)->PSSetShader(m_pPixelShader.Get(), nullptr, 0u);
}
