
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/PixelShader.h>

//===== �N���X���� =====
PIXEL_SHADER::PIXEL_SHADER(const CT_GRAPHIC& Gfx, const std::wstring& Path) :
	BINDER(), m_pPixelShader()
{
	//�G���[�n���h��
	HRESULT hr{};
	std::wstring strPath = Path;

	//�V�F�[�_�쐬
	ComPtr<ID3DBlob> pBlob;
	hr = D3DReadFileToBlob(strPath.c_str(), &pBlob);
	ERROR_DX(hr);
	hr = GetDevice(Gfx)->CreatePixelShader(
		pBlob->GetBufferPointer(), pBlob->GetBufferSize(),
		nullptr, &m_pPixelShader
	);
	ERROR_DX(hr);
}

PIXEL_SHADER::~PIXEL_SHADER() noexcept
{
}

//�o�C���h����
void PIXEL_SHADER::Bind(const CT_GRAPHIC& Gfx) const
{
	GetContext(Gfx)->PSSetShader(m_pPixelShader.Get(), nullptr, 0u);
}
