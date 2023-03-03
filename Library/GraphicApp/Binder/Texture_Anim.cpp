
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/Texture_Anim.h>

//===== �N���X���� =====
TEXTURE_ANIM::TEXTURE_ANIM(const GRAPHIC& Gfx, const TEX_DATA_ANIM& Data, UINT StartSlot) :
	BINDER(), m_pTextureView(), m_StartSlot(StartSlot)
{
	//�G���[�n���h��
	HRESULT hr{};

	//�o�b�t�@�쐬
	if (Data.pAnimData != nullptr) {

		//�e�N�X�`�����\�[�X�쐬
		D3D11_TEXTURE2D_DESC td{};
		td.Width = static_cast<UINT>(Data.nWidth) * 4u;		//�A�j���[�V������ * �X�̃t���[���� * float4�i1�e�N�Z���j
		td.Height = static_cast<UINT>(Data.nHeight);
		td.MipLevels = 1u;
		td.ArraySize = 1u;
		td.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		td.SampleDesc.Count = 1u;
		td.SampleDesc.Quality = 0u;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0u;
		td.MiscFlags = 0u;
		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = Data.pAnimData;
		sd.SysMemPitch = Data.nWidth * sizeof(float) * 16u;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
		hr = GetDevice(Gfx)->CreateTexture2D(&td, &sd, &pTexture);
		ERROR_DX(hr);

		//���\�[�X�r���[�쐬
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd{};
		srvd.Format = td.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MostDetailedMip = 0u;
		srvd.Texture2D.MipLevels = 1u;
		hr = GetDevice(Gfx)->CreateShaderResourceView(pTexture.Get(), &srvd, &m_pTextureView);
		ERROR_DX(hr);
	}
	else
		throw ERROR_EX2("�e�N�X�`���̃|�C���^��NULL�ł��B");
}

TEXTURE_ANIM::~TEXTURE_ANIM() noexcept
{
}

//�o�C���h����
void TEXTURE_ANIM::Bind(const GRAPHIC& Gfx) const
{
	GetContext(Gfx)->PSSetShaderResources(m_StartSlot, 1u, m_pTextureView.GetAddressOf());
}
