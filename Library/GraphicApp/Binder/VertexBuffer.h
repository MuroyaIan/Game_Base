/**
 * @file VertexBuffer.h
 * @brief ���_�o�b�t�@�̏���
 * @author ���J�C�A��
 * @date 2022/06/24
 * @���� 2022/06/24�F�t�@�C���쐬
 *		 2022/08/11�F�C���X�^���V���O����
 *		 2022/11/21�F�������P
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/Binder.h>

//===== �N���X��` =====

//***** ���_�o�b�t�@ *****
class VERTEX_BUFFER : public CT_BINDER
{
public:

	//�o�b�t�@�^�C�v
	enum class VB_TYPE
	{
		Vertex,		//���_���
		Instance,	//�C���X�^���X���

		MaxType
	};

	//�v���g�^�C�v�錾
	template<class V>
	explicit VERTEX_BUFFER(const CT_GRAPHIC& Gfx, const std::vector<V>& aVertice, bool bDynamic = false) :
		CT_BINDER(), m_pVertexBuffers(static_cast<int>(VB_TYPE::MaxType)), m_bDynamicBuffer(bDynamic), m_aStride(static_cast<int>(VB_TYPE::MaxType))
	{
		//�G���[�n���h��
		HRESULT hr{};

		//�o�b�t�@�쐬
		D3D11_BUFFER_DESC bd = GetBufferDesc(
			static_cast<UINT>(sizeof(V) * aVertice.size()),
			static_cast<UINT>(sizeof(V)),
			m_bDynamicBuffer
		);
		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = aVertice.data();
		hr = GetDevice(Gfx)->CreateBuffer(&bd, &sd, &m_pVertexBuffers[static_cast<int>(VB_TYPE::Vertex)]);
		ERROR_DX(hr);

		//���̑�������
		m_aStride[static_cast<int>(VB_TYPE::Vertex)] = static_cast<UINT>(sizeof(V));
	}

	template<class V, class I>
	explicit VERTEX_BUFFER(const CT_GRAPHIC& Gfx, const std::vector<V>& aVertice, const std::vector<I>& aInstance, bool bDynamic = false) :
		CT_BINDER(), m_pVertexBuffers(static_cast<int>(VB_TYPE::MaxType)), m_bDynamicBuffer(bDynamic), m_aStride(static_cast<int>(VB_TYPE::MaxType))
	{
		//�G���[�n���h��
		HRESULT hr{};

		//�o�b�t�@�쐬�i���_���j
		D3D11_BUFFER_DESC bd1 = GetBufferDesc(
			static_cast<UINT>(sizeof(V) * aVertice.size()),
			static_cast<UINT>(sizeof(V)),
			m_bDynamicBuffer
		);
		D3D11_SUBRESOURCE_DATA sd1{};
		sd1.pSysMem = aVertice.data();
		hr = GetDevice(Gfx)->CreateBuffer(&bd1, &sd1, &m_pVertexBuffers[static_cast<int>(VB_TYPE::Vertex)]);
		ERROR_DX(hr);

		//�o�b�t�@�쐬�i�C���X�^���X���j
		int size{};
		if (aInstance.size() == 0)
			size = 1;
		else
			size = static_cast<int>(aInstance.size());
		D3D11_BUFFER_DESC bd2 = GetBufferDesc(
			static_cast<UINT>(sizeof(I) * size),
			static_cast<UINT>(sizeof(I)),
			true
		);
		hr = GetDevice(Gfx)->CreateBuffer(&bd2, nullptr, &m_pVertexBuffers[static_cast<int>(VB_TYPE::Instance)]);
		ERROR_DX(hr);

		//���̑�������
		m_aStride[static_cast<int>(VB_TYPE::Vertex)] = static_cast<UINT>(sizeof(V));
		m_aStride[static_cast<int>(VB_TYPE::Instance)] = static_cast<UINT>(sizeof(I));
	}
	~VERTEX_BUFFER() noexcept override;

	template<typename D>
	void UpdateBuffer(const CT_GRAPHIC& Gfx, const std::vector<D>& aData, VB_TYPE Type) const	//�o�b�t�@�X�V
	{
		//��O����
		if (m_pVertexBuffers[static_cast<int>(Type)].Get() == nullptr)
			throw ERROR_EX2("�o�b�t�@��null�ł��B");
		if (Type == VB_TYPE::Vertex && !m_bDynamicBuffer)
			throw ERROR_EX2("���_�o�b�t�@�͓��I�ł͂Ȃ�!");

		MapBuffer(Gfx, aData, m_pVertexBuffers[static_cast<int>(Type)].Get());
	}

	template<typename I>
	void ResetInstanceBuffer(const CT_GRAPHIC& Gfx, const std::vector<I>& aInstance)	//�C���X�^���X�o�b�t�@�Đݒ�
	{
		//�G���[�n���h��
		HRESULT hr{};

		//�C���X�^���X�o�b�t�@����ːV��������
		int size{};
		if (aInstance.size() == 0)
			size = 1;
		else
			size = static_cast<int>(aInstance.size());
		D3D11_BUFFER_DESC bd2 = GetBufferDesc(
			static_cast<UINT>(sizeof(I) * size),
			static_cast<UINT>(sizeof(I)),
			true
		);
		hr = GetDevice(Gfx)->CreateBuffer(&bd2, nullptr, &m_pVertexBuffers[static_cast<int>(VB_TYPE::Instance)]);
		ERROR_DX(hr);

		//���̑�������
		m_aStride[static_cast<int>(VB_TYPE::Instance)] = static_cast<UINT>(sizeof(I));
	}

	void Bind(const CT_GRAPHIC& Gfx) const override;	//�o�C���h����

private:

	//�ϐ��錾
	std::vector<ComPtr<ID3D11Buffer>> m_pVertexBuffers;		//�|�C���^�z��
	bool m_bDynamicBuffer;									//���_�o�b�t�@�̓_�C�i�~�b�N���ǂ���
	std::vector<UINT> m_aStride;							//���_�o�b�t�@�z��̗v�f1���̃T�C�Y�̔z��

	//�v���g�^�C�v�錾
	D3D11_BUFFER_DESC GetBufferDesc(UINT ArraySize, UINT TypeSize, bool bDynamic) const noexcept;	//�o�b�t�@�ݒ�擾
};
