
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/PixelShader.h>

namespace wrl = Microsoft::WRL;

//===== �N���X���� =====
PIXEL_SHADER::PIXEL_SHADER(const GRAPHIC& Gfx, const std::wstring& Path) :
	BINDER(), m_pPixelShader()
{
	//�G���[�n���h��
	HRESULT hr{};
	std::wstring strPath = Path;

	//�V�F�[�_�쐬
	wrl::ComPtr<ID3DBlob> pBlob;
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
void PIXEL_SHADER::Bind(const GRAPHIC& Gfx) const
{
	GetContext(Gfx)->PSSetShader(m_pPixelShader.Get(), nullptr, 0u);
}
