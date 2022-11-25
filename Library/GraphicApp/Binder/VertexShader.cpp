
//===== インクルード部 =====
#include <GraphicApp/Binder/VertexShader.h>

//===== クラス実装 =====
VERTEX_SHADER::VERTEX_SHADER(const GRAPHIC& Gfx, const std::wstring& Path) :
	BINDER(), m_pBlob(), m_pVertexShader()
{
	//エラーハンドル
	HRESULT hr{};

	//シェーダ作成
	hr = D3DReadFileToBlob(Path.c_str(), &m_pBlob);
	ERROR_DX(hr);
	hr = GetDevice(Gfx)->CreateVertexShader(
		m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(),
		nullptr, &m_pVertexShader
	);
	ERROR_DX(hr);
}

VERTEX_SHADER::~VERTEX_SHADER() noexcept
{
}

//バインド処理
void VERTEX_SHADER::Bind(const GRAPHIC& Gfx) const noexcept
{
	GetContext(Gfx)->VSSetShader(m_pVertexShader.Get(), nullptr, 0u);
}
