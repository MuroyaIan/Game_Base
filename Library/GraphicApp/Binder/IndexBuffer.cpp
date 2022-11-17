
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/IndexBuffer.h>

//===== �N���X���� =====
INDEX_BUFFER::INDEX_BUFFER(GRAPHIC& Gfx, const std::vector<unsigned short>& Indices) :
	BINDER(), m_Count(static_cast<UINT>(Indices.size())), m_pIndexBuffer()
{
	//�G���[�n���h��
	HRESULT hr{};

	//�o�b�t�@�쐬
	D3D11_BUFFER_DESC ibd{};
	ibd.ByteWidth = static_cast<UINT>(sizeof(unsigned short) * m_Count);
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.StructureByteStride = static_cast<UINT>(sizeof(unsigned short));
	D3D11_SUBRESOURCE_DATA isd{};
	isd.pSysMem = Indices.data();
	hr = GetDevice(Gfx)->CreateBuffer(&ibd, &isd, &m_pIndexBuffer);
	ERROR_DX(hr);
}

INDEX_BUFFER::~INDEX_BUFFER() noexcept
{
}

//�o�C���h����
void INDEX_BUFFER::Bind(GRAPHIC& Gfx) noexcept
{
	GetContext(Gfx)->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}
