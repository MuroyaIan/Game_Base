
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/VertexShader.h>

//===== �N���X���� =====
VERTEX_SHADER::VERTEX_SHADER(const GRAPHIC& Gfx, const std::wstring& Path) :
	BINDER(), m_pBlob(), m_pVertexShader()
{
	//�G���[�n���h��
	HRESULT hr{};

	//�V�F�[�_�쐬
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

//�o�C���h����
void VERTEX_SHADER::Bind(const GRAPHIC& Gfx) const noexcept
{
	GetContext(Gfx)->VSSetShader(m_pVertexShader.Get(), nullptr, 0u);
}
