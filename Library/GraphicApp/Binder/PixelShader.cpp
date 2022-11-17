
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/PixelShader.h>

//===== �N���X���� =====
PIXEL_SHADER::PIXEL_SHADER(GRAPHIC& Gfx, const std::wstring& Path) :
	BINDER(), m_pPixelShader()
{
	//�G���[�n���h��
	HRESULT hr{};

	//�V�F�[�_�쐬
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	hr = D3DReadFileToBlob(Path.c_str(), &pBlob);
	ERROR_DX(hr);
	hr = GetDevice(Gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pPixelShader);
	ERROR_DX(hr);
}

PIXEL_SHADER::~PIXEL_SHADER() noexcept
{
}

//�o�C���h����
void PIXEL_SHADER::Bind(GRAPHIC& Gfx) noexcept
{
	GetContext(Gfx)->PSSetShader(m_pPixelShader.Get(), nullptr, 0u);
}
