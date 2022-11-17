/**
 * @file ConstantBuffer.h
 * @brief �萔�o�b�t�@�̏���
 * @author ���J�C�A��
 * @date 2022/06/25
 * @���� 2022/06/25�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/Binder.h>
#include <GraphicApp/Drawer/VertexShaderData.h>

//===== �񋓌^�錾 =====
enum class CB_SLOT_VS
{
	DEFAULT,
	CAMERA,
	BONE,
	LOCAL
};

enum class CB_SLOT_PS
{
	DEFAULT,
	LIGHT,
	MATERIAL
};

//===== �\���̐錾 =====
struct CB_COLOR							//�J���[�p�萔�o�b�t�@
{
	//�ϐ��錾
	DirectX::XMFLOAT4 FaceColor[24];	//�|���S���F�i�ʏ��j

	CB_COLOR() noexcept
	{
		FaceColor[0]  = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		FaceColor[1]  = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		FaceColor[2]  = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
		FaceColor[3]  = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
		FaceColor[4]  = DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
		FaceColor[5]  = DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
		FaceColor[6]  = DirectX::XMFLOAT4(1.0f, 0.5f, 0.0f, 1.0f);
		FaceColor[7]  = DirectX::XMFLOAT4(0.5f, 1.0f, 0.0f, 1.0f);
		FaceColor[8]  = DirectX::XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f);
		FaceColor[9]  = DirectX::XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f);
		FaceColor[10] = DirectX::XMFLOAT4(0.0f, 1.0f, 0.5f, 1.0f);
		FaceColor[11] = DirectX::XMFLOAT4(0.0f, 0.5f, 1.0f, 1.0f);
		FaceColor[12] = DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f);
		FaceColor[13] = DirectX::XMFLOAT4(0.7f, 1.0f, 0.0f, 1.0f);
		FaceColor[14] = DirectX::XMFLOAT4(1.0f, 0.0f, 0.7f, 1.0f);
		FaceColor[15] = DirectX::XMFLOAT4(0.7f, 0.0f, 1.0f, 1.0f);
		FaceColor[16] = DirectX::XMFLOAT4(0.0f, 1.0f, 0.7f, 1.0f);
		FaceColor[17] = DirectX::XMFLOAT4(0.0f, 0.7f, 1.0f, 1.0f);
		FaceColor[18] = DirectX::XMFLOAT4(1.0f, 0.3f, 0.0f, 1.0f);
		FaceColor[19] = DirectX::XMFLOAT4(0.3f, 1.0f, 0.0f, 1.0f);
		FaceColor[20] = DirectX::XMFLOAT4(1.0f, 0.0f, 0.3f, 1.0f);
		FaceColor[21] = DirectX::XMFLOAT4(0.3f, 0.0f, 1.0f, 1.0f);
		FaceColor[22] = DirectX::XMFLOAT4(0.0f, 1.0f, 0.3f, 1.0f);
		FaceColor[23] = DirectX::XMFLOAT4(0.0f, 0.3f, 1.0f, 1.0f);
	}
	~CB_COLOR() noexcept
	{}
};

struct CB_MTX_V_P					//�ϊ��s��p�萔�o�b�t�@
{
	//�ϐ��錾
	DirectX::XMFLOAT4X4 mtxView;	//�r���[�s��
	DirectX::XMFLOAT4X4 mtxProj;	//���e�s��

	CB_MTX_V_P() noexcept : mtxView(), mtxProj()
	{
		DirectX::XMStoreFloat4x4(&mtxView, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&mtxProj, DirectX::XMMatrixIdentity());
	}
	CB_MTX_V_P(DirectX::XMFLOAT4X4 mtxV, DirectX::XMFLOAT4X4 mtxP) noexcept : mtxView(mtxV), mtxProj(mtxP)
	{}
	~CB_MTX_V_P() noexcept
	{}
};

struct CB_MTX_L_W_V_P					//�ϊ��s��p�萔�o�b�t�@
{
	//�ϐ��錾
	DirectX::XMFLOAT4X4 mtxLocal;	//���[�J���s��
	DirectX::XMFLOAT4X4 mtxWorld;	//���[���h�s��
	DirectX::XMFLOAT4X4 mtxView;	//�r���[�s��
	DirectX::XMFLOAT4X4 mtxProj;	//���e�s��

	CB_MTX_L_W_V_P() noexcept : mtxLocal(), mtxWorld(), mtxView(), mtxProj()
	{
		DirectX::XMStoreFloat4x4(&mtxLocal, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&mtxWorld, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&mtxView, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&mtxProj, DirectX::XMMatrixIdentity());
	}
	CB_MTX_L_W_V_P(DirectX::XMFLOAT4X4 mtxL, DirectX::XMFLOAT4X4 mtxW, DirectX::XMFLOAT4X4 mtxV, DirectX::XMFLOAT4X4 mtxP) noexcept : mtxLocal(mtxL), mtxWorld(mtxW), mtxView(mtxV), mtxProj(mtxP)
	{}
	~CB_MTX_L_W_V_P() noexcept
	{}
};

struct CB_MTX_BONE							//���s��p�萔�o�b�t�@
{
	//�ϐ��錾
	DirectX::XMFLOAT4X4 mtxBone[MAX_BONE];	//���[���h�s��

	CB_MTX_BONE() noexcept : mtxBone()
	{
		for (auto& b : mtxBone)
			DirectX::XMStoreFloat4x4(&b, DirectX::XMMatrixIdentity());
	}
	~CB_MTX_BONE() noexcept
	{}
};

struct CB_MTX_LOCAL							//���s��p�萔�o�b�t�@
{
	//�ϐ��錾
	DirectX::XMFLOAT4X4 mtxSkin;			//���Ȃ����b�V���p���[�J���s��

	CB_MTX_LOCAL() noexcept : mtxSkin()
	{
		DirectX::XMStoreFloat4x4(&mtxSkin, DirectX::XMMatrixIdentity());
	}
	~CB_MTX_LOCAL() noexcept
	{}
};

//===== �N���X��` =====

//***** �萔�o�b�t�@ *****
template<typename C>
class CONSTANT_BUFFER : public BINDER
{
public:

	//�v���g�^�C�v�錾
	CONSTANT_BUFFER(GRAPHIC& Gfx, const C& Consts, UINT Slot = 0u) :
		BINDER(), m_pConstantBuffer(), m_Slot(Slot)
	{
		//�G���[�n���h��
		HRESULT hr{};

		//�o�b�t�@�쐬
		D3D11_BUFFER_DESC cbd{};
		cbd.ByteWidth = static_cast<UINT>(sizeof(Consts));
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.StructureByteStride = 0u;
		D3D11_SUBRESOURCE_DATA csd{};
		csd.pSysMem = &Consts;
		hr = GetDevice(Gfx)->CreateBuffer(&cbd, &csd, &m_pConstantBuffer);
		ERROR_DX(hr);
	}
	CONSTANT_BUFFER(GRAPHIC& Gfx, UINT Slot = 0u) :
		BINDER(), m_pConstantBuffer(), m_Slot(Slot)		//�o�b�t�@�������Ȃ�
	{
		//�G���[�n���h��
		HRESULT hr{};

		//�o�b�t�@�쐬
		D3D11_BUFFER_DESC cbd{};
		cbd.ByteWidth = static_cast<UINT>(sizeof(C));
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.StructureByteStride = 0u;
		hr = GetDevice(Gfx)->CreateBuffer(&cbd, nullptr, &m_pConstantBuffer);
		ERROR_DX(hr);
	}
	~CONSTANT_BUFFER() noexcept override {}

	void Update(GRAPHIC& Gfx, const C& Consts) const	//�o�b�t�@�X�V
	{
		//�G���[�n���h��
		HRESULT hr{};

		//�X�V����
		D3D11_MAPPED_SUBRESOURCE msr{};
		hr = GetContext(Gfx)->Map(
			m_pConstantBuffer.Get(), 0u,
			D3D11_MAP_WRITE_DISCARD, 0u,
			&msr);												//GPU�̃A�N�Z�X�����b�N
		ERROR_DX(hr);
		memcpy(msr.pData, &Consts, sizeof(Consts));				//�f�[�^������
		GetContext(Gfx)->Unmap(m_pConstantBuffer.Get(), 0u);	//GPU�̃A�N�Z�X�����
	}

protected:

	//�ϐ��錾
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;		//�|�C���^
	UINT m_Slot;												//���̓X���b�g
};

//***** �萔�o�b�t�@�i���_�V�F�[�_�p�j *****
template<typename C>
class VERTEX_CBUFFER : public CONSTANT_BUFFER<C>
{
public:

	//�v���g�^�C�v�錾
	VERTEX_CBUFFER(GRAPHIC& Gfx, const C& Consts, UINT Slot = 0u) : C_BUFF::CONSTANT_BUFFER(Gfx, Consts, Slot) {}
	VERTEX_CBUFFER(GRAPHIC& Gfx, UINT Slot = 0u) : C_BUFF::CONSTANT_BUFFER(Gfx, Slot) {}
	~VERTEX_CBUFFER() noexcept override {}

	void Bind(GRAPHIC& Gfx) noexcept override	//�o�C���h����
	{
		BINDER::GetContext(Gfx)->VSSetConstantBuffers(C_BUFF::m_Slot, 1u, C_BUFF::m_pConstantBuffer.GetAddressOf());
	}

private:

	//�e���v���[�g�΍�
	using C_BUFF = CONSTANT_BUFFER<C>;
};

//***** �萔�o�b�t�@�i�s�N�Z���V�F�[�_�p�j *****
template<typename C>
class PIXEL_CBUFFER : public CONSTANT_BUFFER<C>
{
public:

	//�v���g�^�C�v�錾
	PIXEL_CBUFFER(GRAPHIC& Gfx, const C& Consts, UINT Slot = 0u) : C_BUFF::CONSTANT_BUFFER(Gfx, Consts, Slot) {}
	PIXEL_CBUFFER(GRAPHIC& Gfx, UINT Slot = 0u) : C_BUFF::CONSTANT_BUFFER(Gfx, Slot) {}
	~PIXEL_CBUFFER() noexcept override {}

	void Bind(GRAPHIC& Gfx) noexcept override	//�o�C���h����
	{
		BINDER::GetContext(Gfx)->PSSetConstantBuffers(C_BUFF::m_Slot, 1u, C_BUFF::m_pConstantBuffer.GetAddressOf());
	}

private:

	//�e���v���[�g�΍�
	using C_BUFF = CONSTANT_BUFFER<C>;
};
