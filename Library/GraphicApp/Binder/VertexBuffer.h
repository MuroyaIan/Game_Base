/**
 * @file VertexBuffer.h
 * @brief ���_�o�b�t�@�̏���
 * @author ���J�C�A��
 * @date 2022/06/24
 * @���� 2022/06/24�F�t�@�C���쐬
 *		 2022/08/11�F�C���X�^���V���O����
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/Binder.h>

//===== �N���X��` =====

//***** ���_�o�b�t�@ *****
class VERTEX_BUFFER : public BINDER
{
public:

	//�o�b�t�@�^�C�v
	enum class VB_TYPE
	{
		VERTEX,		//���_���
		INSTANCE,	//�C���X�^���X���

		MAX_TYPE
	};

	//�v���g�^�C�v�錾
	template<class V>
	VERTEX_BUFFER(GRAPHIC& Gfx, const std::vector<V>& Vertices, bool bDynamic = false) :
		BINDER(), m_aStride(), m_pVertexBuffer(), m_bDynamicBuffer(bDynamic)
	{
		//�G���[�n���h��
		HRESULT hr{};

		//�o�b�t�@�쐬
		D3D11_BUFFER_DESC bd{};
		bd.ByteWidth = static_cast<UINT>(sizeof(V) * Vertices.size());
		if (!m_bDynamicBuffer)
			bd.Usage = D3D11_USAGE_DEFAULT;
		else
			bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		if (!m_bDynamicBuffer)
			bd.CPUAccessFlags = 0u;
		else
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0u;
		bd.StructureByteStride = static_cast<UINT>(sizeof(V));
		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = Vertices.data();
		hr = GetDevice(Gfx)->CreateBuffer(&bd, &sd, &m_pVertexBuffer[static_cast<int>(VB_TYPE::VERTEX)]);
		ERROR_DX(hr);

		//���̑�������
		m_aStride[static_cast<int>(VB_TYPE::VERTEX)] = static_cast<UINT>(sizeof(V));
	}

	template<class V, class I>
	VERTEX_BUFFER(GRAPHIC& Gfx, const std::vector<V>& Vertices, const std::vector<I>& Instances, bool bDynamic = false) :
		BINDER(), m_aStride(), m_pVertexBuffer(), m_bDynamicBuffer(bDynamic)
	{
		//�G���[�n���h��
		HRESULT hr{};

		//�o�b�t�@�쐬�i���_���j
		D3D11_BUFFER_DESC bd1{};
		bd1.ByteWidth = static_cast<UINT>(sizeof(V) * Vertices.size());
		bd1.Usage = D3D11_USAGE_DEFAULT;
		bd1.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd1.CPUAccessFlags = 0u;
		bd1.MiscFlags = 0u;
		bd1.StructureByteStride = static_cast<UINT>(sizeof(V));
		D3D11_SUBRESOURCE_DATA sd1{};
		sd1.pSysMem = Vertices.data();
		hr = GetDevice(Gfx)->CreateBuffer(&bd1, &sd1, &m_pVertexBuffer[static_cast<int>(VB_TYPE::VERTEX)]);
		ERROR_DX(hr);

		//�o�b�t�@�쐬�i�C���X�^���X���j
		int size{};
		if (Instances.size() == 0)
			size = 1;
		else
			size = static_cast<int>(Instances.size());
		D3D11_BUFFER_DESC bd2{};
		bd2.ByteWidth = static_cast<UINT>(sizeof(I) * size);
		bd2.Usage = D3D11_USAGE_DYNAMIC;
		bd2.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd2.MiscFlags = 0u;
		bd2.StructureByteStride = static_cast<UINT>(sizeof(I));
		hr = GetDevice(Gfx)->CreateBuffer(&bd2, nullptr, &m_pVertexBuffer[static_cast<int>(VB_TYPE::INSTANCE)]);
		ERROR_DX(hr);

		//���̑�������
		m_aStride[static_cast<int>(VB_TYPE::VERTEX)] = static_cast<UINT>(sizeof(V));
		m_aStride[static_cast<int>(VB_TYPE::INSTANCE)] = static_cast<UINT>(sizeof(I));
	}
	~VERTEX_BUFFER() noexcept override;

	template<typename D>
	void UpdateBuffer(GRAPHIC& Gfx, const std::vector<D>& aData, VB_TYPE type) const	//�o�b�t�@�X�V
	{
		//��O����
		if (m_pVertexBuffer[static_cast<int>(type)].Get() == nullptr)
			throw ERROR_EX2("�o�b�t�@��null�ł��B");
		if (type == VB_TYPE::VERTEX && !m_bDynamicBuffer)
			throw ERROR_EX2("���_�o�b�t�@�͓��I�ł͂Ȃ�!");

		//�G���[�n���h��
		HRESULT hr{};

		//�X�V����
		D3D11_MAPPED_SUBRESOURCE msr{};
		hr = GetContext(Gfx)->Map(
			m_pVertexBuffer[static_cast<int>(type)].Get(), 0u,
			D3D11_MAP_WRITE_DISCARD, 0u,
			&msr);																		//GPU�̃A�N�Z�X�����b�N
		ERROR_DX(hr);
		memcpy(msr.pData, aData.data(), static_cast<UINT>(sizeof(D) * aData.size()));	//�f�[�^������
		GetContext(Gfx)->Unmap(m_pVertexBuffer[static_cast<int>(type)].Get(), 0u);		//GPU�̃A�N�Z�X�����
	}

	template<typename I>
	void ResetInstanceBuffer(GRAPHIC& Gfx, const std::vector<I>& Instances)				//�C���X�^���X�o�b�t�@�Đݒ�
	{
		//�G���[�n���h��
		HRESULT hr{};

		//�C���X�^���X�o�b�t�@����ːV��������
		int size{};
		if (Instances.size() == 0)
			size = 1;
		else
			size = static_cast<int>(Instances.size());
		D3D11_BUFFER_DESC bd2{};
		bd2.ByteWidth = static_cast<UINT>(sizeof(I) * size);
		bd2.Usage = D3D11_USAGE_DYNAMIC;
		bd2.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd2.MiscFlags = 0u;
		bd2.StructureByteStride = static_cast<UINT>(sizeof(I));
		hr = GetDevice(Gfx)->CreateBuffer(&bd2, nullptr, &m_pVertexBuffer[static_cast<int>(VB_TYPE::INSTANCE)]);
		ERROR_DX(hr);

		//���̑�������
		m_aStride[static_cast<int>(VB_TYPE::INSTANCE)] = static_cast<UINT>(sizeof(I));
	}

	void Bind(GRAPHIC& Gfx) noexcept override;													//�o�C���h����

protected:

	//�ϐ��錾
	UINT m_aStride[static_cast<int>(VB_TYPE::MAX_TYPE)];										//���_�o�b�t�@�z��̗v�f1���̃T�C�Y
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer[static_cast<int>(VB_TYPE::MAX_TYPE)];	//�|�C���^
	bool m_bDynamicBuffer;																		//���_�o�b�t�@�̓_�C�i�~�b�N���ǂ���
};
