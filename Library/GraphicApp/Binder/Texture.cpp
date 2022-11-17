
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/Texture.h>

//===== �N���X���� =====
TEXTURE::TEXTURE(GRAPHIC& Gfx, TEX_LOADER::TEX_DATA& Data) : BINDER(), m_pTextureView()
{
	//�G���[�n���h��
	HRESULT hr{};

	//�o�b�t�@�쐬
	if (Data.pImageData != nullptr) {

		//�e�N�X�`�����\�[�X�쐬
		D3D11_TEXTURE2D_DESC TextureDesc{};
		TextureDesc.Width = static_cast<UINT>(Data.nWidth);
		TextureDesc.Height = static_cast<UINT>(Data.nHeight);
		TextureDesc.MipLevels = 1;
		TextureDesc.ArraySize = 1;
		TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		TextureDesc.SampleDesc.Count = 1;
		TextureDesc.SampleDesc.Quality = 0;
		TextureDesc.Usage = D3D11_USAGE_DEFAULT;
		TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		TextureDesc.CPUAccessFlags = 0;
		TextureDesc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = Data.pImageData;
		sd.SysMemPitch = Data.nWidth * sizeof(uint32_t);
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
		hr = GetDevice(Gfx)->CreateTexture2D(&TextureDesc, &sd, &pTexture);
		ERROR_DX(hr);

		//���\�[�X�r���[�쐬
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = TextureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		hr = GetDevice(Gfx)->CreateShaderResourceView(pTexture.Get(), &srvDesc, &m_pTextureView);
		ERROR_DX(hr);
	}
	else
		throw ERROR_EX2("�e�N�X�`���̃|�C���^��NULL�ł��B");
}

TEXTURE::~TEXTURE() noexcept
{
}

//�o�C���h����
void TEXTURE::Bind(GRAPHIC& Gfx) noexcept
{
	GetContext(Gfx)->PSSetShaderResources(0u, 1u, m_pTextureView.GetAddressOf());
}
