
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/VertexShader.h>

//===== �N���X���� =====
VERTEX_SHADER::VERTEX_SHADER(const CT_GRAPHIC& Gfx, const std::wstring& Path) :
	CT_BINDER(), m_pBlob(), m_pVertexShader()
{
	//�G���[�n���h��
	HRESULT hr{};
	std::wstring strPath = Path;

	//�V�F�[�_�쐬
	hr = D3DReadFileToBlob(strPath.c_str(), &m_pBlob);
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
void VERTEX_SHADER::Bind(const CT_GRAPHIC& Gfx) const
{
	GetContext(Gfx)->VSSetShader(m_pVertexShader.Get(), nullptr, 0u);
}
