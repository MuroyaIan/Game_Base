/**
 * @file ConstantBuffer.h
 * @brief �萔�o�b�t�@�̏���
 * @author ���J�C�A��
 * @date 2022/06/25
 * @���� 2022/06/25�F�t�@�C���쐬
 *		 2022/11/24�F�������P
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/Binder.h>

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
struct CBD_COLOR						//�|���S���F�p�f�[�^
{
	//�ϐ��錾
	DirectX::XMFLOAT4 FaceColor[24];	//�|���S���F�i�ʂ��Ɓj

	CBD_COLOR() noexcept
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

	~CBD_COLOR() noexcept
	{}
};

//===== �N���X��` =====

//***** �萔�o�b�t�@ *****
template<typename C>
class CONSTANT_BUFFER : public BINDER
{
public:

	//�v���g�^�C�v�錾
	explicit CONSTANT_BUFFER(const GRAPHIC& Gfx, const C& Consts, UINT Slot = 0u) :
		BINDER(), m_pConstantBuffer(), m_StartSlot(Slot)
	{
		//�G���[�n���h��
		HRESULT hr{};

		//�o�b�t�@�쐬
		D3D11_BUFFER_DESC bd = GetBufferDesc(static_cast<UINT>(sizeof(C)));
		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = &Consts;
		hr = GetDevice(Gfx)->CreateBuffer(&bd, &sd, &m_pConstantBuffer);
		ERROR_DX(hr);
	}

	explicit CONSTANT_BUFFER(const GRAPHIC& Gfx, UINT Slot = 0u) :
		BINDER(), m_pConstantBuffer(), m_StartSlot(Slot)			//�o�b�t�@�������Ȃ�
	{
		//�G���[�n���h��
		HRESULT hr{};

		//�o�b�t�@�쐬
		D3D11_BUFFER_DESC bd = GetBufferDesc(static_cast<UINT>(sizeof(C)));
		hr = GetDevice(Gfx)->CreateBuffer(&bd, nullptr, &m_pConstantBuffer);
		ERROR_DX(hr);
	}
	~CONSTANT_BUFFER() noexcept override {}

	void Update(const GRAPHIC& Gfx, const C& Consts) const			//�o�b�t�@�X�V
	{
		MapBuffer(Gfx, Consts, m_pConstantBuffer.Get());
	}

protected:

	//�ϐ��錾
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;		//�|�C���^
	UINT m_StartSlot;											//���W�X�^�ԍ�

	//�v���g�^�C�v�錾
	D3D11_BUFFER_DESC GetBufferDesc(UINT ArraySize)				//�o�b�t�@�ݒ�擾
	{
		D3D11_BUFFER_DESC bd{};
		bd.ByteWidth = ArraySize;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0u;
		bd.StructureByteStride = 0u;

		return bd;
	}
};

//***** �萔�o�b�t�@�i���_�V�F�[�_�p�j *****
template<typename C>
class VS_CBUFFER : public CONSTANT_BUFFER<C>
{
public:

	//�v���g�^�C�v�錾
	explicit VS_CBUFFER(const GRAPHIC& Gfx, const C& Consts, UINT Slot = 0u) : C_BUFF(Gfx, Consts, Slot) {}
	explicit VS_CBUFFER(const GRAPHIC& Gfx, UINT Slot = 0u) : C_BUFF(Gfx, Slot) {}
	~VS_CBUFFER() noexcept override {}

	void Bind(const GRAPHIC& Gfx) const noexcept override	//�o�C���h����
	{
		BINDER::GetContext(Gfx)->VSSetConstantBuffers(C_BUFF::m_StartSlot, 1u, C_BUFF::m_pConstantBuffer.GetAddressOf());
	}

private:

	//�e���v���[�g�΍�
	using C_BUFF = CONSTANT_BUFFER<C>;
};

//***** �萔�o�b�t�@�i�s�N�Z���V�F�[�_�p�j *****
template<typename C>
class PS_CBUFFER : public CONSTANT_BUFFER<C>
{
public:

	//�v���g�^�C�v�錾
	explicit PS_CBUFFER(const GRAPHIC& Gfx, const C& Consts, UINT Slot = 0u) : C_BUFF(Gfx, Consts, Slot) {}
	explicit PS_CBUFFER(const GRAPHIC& Gfx, UINT Slot = 0u) : C_BUFF(Gfx, Slot) {}
	~PS_CBUFFER() noexcept override {}

	void Bind(const GRAPHIC& Gfx) const noexcept override	//�o�C���h����
	{
		BINDER::GetContext(Gfx)->PSSetConstantBuffers(C_BUFF::m_StartSlot, 1u, C_BUFF::m_pConstantBuffer.GetAddressOf());
	}

private:

	//�e���v���[�g�΍�
	using C_BUFF = CONSTANT_BUFFER<C>;
};
