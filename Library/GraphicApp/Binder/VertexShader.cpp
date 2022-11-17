
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/VertexShader.h>

//===== �N���X���� =====
VERTEX_SHADER::VERTEX_SHADER(GRAPHIC& Gfx, const std::wstring& Path) :
	BINDER(), m_pBytecodeBlob(), m_pVertexShader()
{
	//�G���[�n���h��
	HRESULT hr{};

	//�V�F�[�_�쐬
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

//�o�C���h����
void VERTEX_SHADER::Bind(GRAPHIC& Gfx) noexcept
{
	GetContext(Gfx)->VSSetShader(m_pVertexShader.Get(), nullptr, 0u);
}
