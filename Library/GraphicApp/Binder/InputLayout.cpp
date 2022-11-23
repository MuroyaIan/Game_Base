
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/InputLayout.h>

//===== �N���X���� =====
INPUT_LAYOUT::INPUT_LAYOUT(GRAPHIC& Gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& aLayout, ID3DBlob* pCodeVS) :
	BINDER(), m_pInputLayout()
{
	//�G���[�n���h��
	HRESULT hr{};

	//���C�A�E�g�쐬
	hr = GetDevice(Gfx)->CreateInputLayout(
		aLayout.data(), static_cast<UINT>(aLayout.size()),
		pCodeVS->GetBufferPointer(), pCodeVS->GetBufferSize(),
		&m_pInputLayout);
	ERROR_DX(hr);
}

INPUT_LAYOUT::~INPUT_LAYOUT() noexcept
{
}

//�o�C���h����
void INPUT_LAYOUT::Bind(GRAPHIC& Gfx) noexcept
{
	GetContext(Gfx)->IASetInputLayout(m_pInputLayout.Get());
}
