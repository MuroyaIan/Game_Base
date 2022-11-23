
//===== インクルード部 =====
#include <GraphicApp/Binder/InputLayout.h>

//===== クラス実装 =====
INPUT_LAYOUT::INPUT_LAYOUT(GRAPHIC& Gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& aLayout, ID3DBlob* pCodeVS) :
	BINDER(), m_pInputLayout()
{
	//エラーハンドル
	HRESULT hr{};

	//レイアウト作成
	hr = GetDevice(Gfx)->CreateInputLayout(
		aLayout.data(), static_cast<UINT>(aLayout.size()),
		pCodeVS->GetBufferPointer(), pCodeVS->GetBufferSize(),
		&m_pInputLayout);
	ERROR_DX(hr);
}

INPUT_LAYOUT::~INPUT_LAYOUT() noexcept
{
}

//バインド処理
void INPUT_LAYOUT::Bind(GRAPHIC& Gfx) noexcept
{
	GetContext(Gfx)->IASetInputLayout(m_pInputLayout.Get());
}
