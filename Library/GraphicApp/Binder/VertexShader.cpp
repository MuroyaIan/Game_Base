
//===== インクルード部 =====
#include <GraphicApp/Binder/VertexShader.h>

//===== クラス実装 =====
VERTEX_SHADER::VERTEX_SHADER(GRAPHIC& Gfx, const std::wstring& Path) :
	BINDER(), m_pBytecodeBlob(), m_pVertexShader()
{
	//エラーハンドル
	HRESULT hr{};

	//シェーダ作成
	hr = D3DReadFileToBlob(Path.c_str(), &m_pBytecodeBlob);
	ERROR_DX(hr);
	hr = GetDevice(Gfx)->CreateVertexShader(
		m_pBytecodeBlob->GetBufferPointer(), m_pBytecodeBlob->GetBufferSize(),
		nullptr, &m_pVertexShader);
	ERROR_DX(hr);
}

VERTEX_SHADER::~VERTEX_SHADER() noexcept
{
}

//バインド処理
void VERTEX_SHADER::Bind(GRAPHIC& Gfx) noexcept
{
	GetContext(Gfx)->VSSetShader(m_pVertexShader.Get(), nullptr, 0u);
}
