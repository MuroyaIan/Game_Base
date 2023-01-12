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
#include <GraphicApp/Binder/cbdRef.h>

//===== �񋓌^�錾 =====
enum class CB_SLOT_VS
{
	Default,
	Camera,		//�J����
	Bone,		//��
	Local		//���[�J���s��
};

enum class CB_SLOT_PS
{
	Default,
	Light,		//��
	Material	//�}�e���A��
};

//===== �񋓌^�錾 =====
struct CB_PTR									//�萔�o�b�t�@�̃|�C���^���
{
	//�ϐ��錾
	std::vector<ID3D11Buffer*> m_aBuffPtrVS;
	std::vector<ID3D11Buffer*> m_aBuffPtrPS;	//�|�C���^�z��

	CB_PTR() noexcept : m_aBuffPtrVS(0), m_aBuffPtrPS(0)
	{}

	~CB_PTR() noexcept
	{}
};

//===== �N���X��` =====

//***** �萔�o�b�t�@ *****
template<typename C>
class CONSTANT_BUFFER : public BINDER
{
public:

	//�v���g�^�C�v�錾
	explicit CONSTANT_BUFFER(const GRAPHIC& Gfx, const C& Consts, CB_PTR* cbPtr, bool SlotVS = false, bool SlotPS = false) :
		BINDER(), m_pConstantBuffer(), m_bSlotVS(SlotVS), m_bSlotPS(SlotPS)
	{
		//�G���[�n���h��
		HRESULT hr{};

		//�o�b�t�@�쐬
		D3D11_BUFFER_DESC bd = GetBufferDesc(static_cast<UINT>(sizeof(C)));
		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = &Consts;
		hr = GetDevice(Gfx)->CreateBuffer(&bd, &sd, &m_pConstantBuffer);
		ERROR_DX(hr);

		//�|�C���^�Q�Ƃ�ݒ�
		SetBuffPtr(cbPtr);
	}

	explicit CONSTANT_BUFFER(const GRAPHIC& Gfx, CB_PTR* cbPtr, bool SlotVS = false, bool SlotPS = false) :
		BINDER(), m_pConstantBuffer(), m_bSlotVS(SlotVS), m_bSlotPS(SlotPS)				//�o�b�t�@�������Ȃ�
	{
		//�G���[�n���h��
		HRESULT hr{};

		//�o�b�t�@�쐬
		D3D11_BUFFER_DESC bd = GetBufferDesc(static_cast<UINT>(sizeof(C)));
		hr = GetDevice(Gfx)->CreateBuffer(&bd, nullptr, &m_pConstantBuffer);
		ERROR_DX(hr);

		//�|�C���^�Q�Ƃ�ݒ�
		if (cbPtr != nullptr) {
			if (m_bSlotVS)
				cbPtr->m_aBuffPtrVS.push_back(m_pConstantBuffer.Get());
			if (m_bSlotPS)
				cbPtr->m_aBuffPtrPS.push_back(m_pConstantBuffer.Get());
		}
	}

	~CONSTANT_BUFFER() noexcept override {}

	void Update(const GRAPHIC& Gfx, const C& Consts) const	//�o�b�t�@�X�V
	{
		MapBuffer(Gfx, Consts, m_pConstantBuffer.Get());
	}

	void SetBuffPtr(CB_PTR* cbPtr) const noexcept			//�o�b�t�@�|�C���^�ݒ�
	{
		if (cbPtr != nullptr) {
			if (m_bSlotVS)
				cbPtr->m_aBuffPtrVS.push_back(m_pConstantBuffer.Get());
			if (m_bSlotPS)
				cbPtr->m_aBuffPtrPS.push_back(m_pConstantBuffer.Get());
		}
	}

	void Bind(const GRAPHIC& Gfx) const noexcept override	//�o�C���h����
	{
		(void)Gfx;
	}

private:

	//�ϐ��錾
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;		//�|�C���^
	bool m_bSlotVS;												//�X���b�g�L��(VS)
	bool m_bSlotPS;												//�X���b�g�L��(PS)

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

//***** �萔�o�b�t�@�}�l�[�W�� *****
class CBUFF_MGR : public BINDER
{
public:

	//�v���g�^�C�v�錾
	explicit CBUFF_MGR(CB_PTR& cbPtrRef) noexcept :
		m_aBuffPtrVS(std::move(cbPtrRef.m_aBuffPtrVS)), m_aBuffPtrPS(std::move(cbPtrRef.m_aBuffPtrPS)),
		m_BuffSizeVS(static_cast<UINT>(m_aBuffPtrVS.size())), m_BuffSizePS(static_cast<UINT>(m_aBuffPtrPS.size()))
	{}

	~CBUFF_MGR() noexcept override {}

	void Bind(const GRAPHIC& Gfx) const noexcept override	//�o�C���h����
	{
		//���ꂼ��̃V�F�[�_�փo�C���h����
		if (m_BuffSizeVS > 0u)
			BINDER::GetContext(Gfx)->VSSetConstantBuffers(0u, m_BuffSizeVS, m_aBuffPtrVS.data());
		if (m_BuffSizePS > 0u)
			BINDER::GetContext(Gfx)->PSSetConstantBuffers(0u, m_BuffSizePS, m_aBuffPtrPS.data());
	}

private:

	//�ϐ��錾
	std::vector<ID3D11Buffer*> m_aBuffPtrVS;
	std::vector<ID3D11Buffer*> m_aBuffPtrPS;	//�萔�o�b�t�@�̃|�C���^�z��
	UINT m_BuffSizeVS;
	UINT m_BuffSizePS;							//�萔�o�b�t�@�̃T�C�Y
};
