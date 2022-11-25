
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/IndexBuffer.h>

//===== �N���X���� =====
INDEX_BUFFER::INDEX_BUFFER(const GRAPHIC& Gfx, const std::vector<UINT>& aIndex) :
	BINDER(), m_pIndexBuffer(), m_IndexNum(static_cast<UINT>(aIndex.size()))
{
	//�G���[�n���h��
	HRESULT hr{};

	//�o�b�t�@�쐬
	D3D11_BUFFER_DESC bd{};
	bd.ByteWidth = static_cast<UINT>(sizeof(UINT) * m_IndexNum);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.StructureByteStride = static_cast<UINT>(sizeof(UINT));
	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = aIndex.data();
	hr = GetDevice(Gfx)->CreateBuffer(&bd, &sd, &m_pIndexBuffer);
	ERROR_DX(hr);
}

INDEX_BUFFER::~INDEX_BUFFER() noexcept
{
}

//�o�C���h����
void INDEX_BUFFER::Bind(const GRAPHIC& Gfx) const noexcept
{
	GetContext(Gfx)->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
}
