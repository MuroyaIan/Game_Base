/**
 * @file VertexShaderData.h
 * @brief ���_�V�F�[�_�p�f�[�^
 * @author ���J�C�A��
 * @date 2022/07/06
 * @���� 2022/07/06�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Graphic.h>

//===== �O���[�o���萔�錾 =====
constexpr int MAX_BONE = 250;		//���̍ő�{��

//===== �\���̐錾 =====
struct VTX_COLOR
{
	//�ϐ��錾
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	//�v���g�^�C�v�錾
	VTX_COLOR() noexcept : r(0), g(0), b(0), a(0)
	{}

	VTX_COLOR(unsigned char rIn, unsigned char gIn, unsigned char bIn, unsigned char aIn) noexcept :
		r(rIn), g(gIn), b(bIn), a(aIn)
	{}

	~VTX_COLOR() noexcept
	{}
};

//===== �N���X��` =====
class VERTEX					//���_���(�x�[�X�N���X)
{
public:

	//�ϐ��錾
	DirectX::XMFLOAT3 m_Pos;	//�ʒu

	//�v���g�^�C�v�錾
	explicit VERTEX() noexcept : m_Pos()
	{}

	explicit VERTEX(DirectX::XMFLOAT3 Pos) noexcept : m_Pos(Pos)
	{}

	~VERTEX() noexcept
	{}
};

class VERTEX_C					//�F�t��
{
public:

	//�ϐ��錾
	DirectX::XMFLOAT3 m_Pos;	//�ʒu
	VTX_COLOR m_Color;			//�F

	//�v���g�^�C�v�錾
	explicit VERTEX_C() noexcept : m_Pos(), m_Color()
	{}

	explicit VERTEX_C(DirectX::XMFLOAT3 Pos, VTX_COLOR Color) noexcept :
		m_Pos(Pos), m_Color(Color)
	{}

	~VERTEX_C() noexcept
	{}
};

class VERTEX_T					//UV���t��
{
public:

	//�ϐ��錾
	DirectX::XMFLOAT3 m_Pos;	//�ʒu
	DirectX::XMFLOAT2 m_UV;		//UV���W

	//�v���g�^�C�v�錾
	explicit VERTEX_T() noexcept : m_Pos(), m_UV()
	{}

	explicit VERTEX_T(DirectX::XMFLOAT3 Pos, DirectX::XMFLOAT2 uv) noexcept :
		m_Pos(Pos), m_UV(uv)
	{}

	~VERTEX_T() noexcept
	{}
};

class VERTEX_M						//���f���p
{
public:

	//�ϐ��錾
	DirectX::XMFLOAT3 m_Pos;		//�ʒu
	DirectX::XMFLOAT2 m_UV;			//UV���W
	DirectX::XMFLOAT3 m_Normal;		//�@��

	//�v���g�^�C�v�錾
	explicit VERTEX_M() noexcept : m_Pos(), m_UV(), m_Normal()
	{}

	~VERTEX_M() noexcept
	{}
};

class VERTEX_MB						//���f���p�i�A�j���[�V�����t���j
{
public:

	//�ϐ��錾
	DirectX::XMFLOAT3 m_Pos;		//�ʒu
	DirectX::XMFLOAT2 m_UV;			//UV���W
	DirectX::XMFLOAT3 m_Normal;		//�@��
	int m_BoneID[4];				//���ԍ�(�ő�̍��ԍ��͎g�p���Ȃ�)
	float m_BoneWeight[4];			//����d

	//�v���g�^�C�v�錾
	explicit VERTEX_MB() noexcept :
		m_Pos(), m_UV(), m_Normal(), m_BoneID(), m_BoneWeight()
	{
		//����񏉊���
		for (auto& b : m_BoneID)
			b = MAX_BONE - 1;
		for (auto& w : m_BoneWeight)
			w = 0.0f;
	}

	~VERTEX_MB() noexcept
	{}
};

class VERTEX_MN						//���f���p�i�@���}�b�s���O�j
{
public:

	//�ϐ��錾
	DirectX::XMFLOAT3 m_Pos;		//�ʒu
	DirectX::XMFLOAT2 m_UV;			//UV���W
	DirectX::XMFLOAT3 m_Normal;		//�@��
	DirectX::XMFLOAT3 m_Binormal;	//�]�@��
	DirectX::XMFLOAT3 m_Tangent;	//�ڐ�

	//�v���g�^�C�v�錾
	explicit VERTEX_MN() noexcept : m_Pos(), m_UV(), m_Normal(), m_Binormal(), m_Tangent()
	{
		//UV��񏉊���
		m_Tangent  = { 1.0f, 0.0f, 0.0f };
		m_Binormal = { 0.0f, 1.0f, 0.0f };
		m_Normal   = { 0.0f, 0.0f, 1.0f };
	}

	~VERTEX_MN() noexcept
	{}
};

class VERTEX_MNB					//���f���p�i�@���}�b�s���O+�A�j���[�V�����t���j
{
public:

	//�ϐ��錾
	DirectX::XMFLOAT3 m_Pos;		//�ʒu
	DirectX::XMFLOAT2 m_UV;			//UV���W
	DirectX::XMFLOAT3 m_Normal;		//�@��
	DirectX::XMFLOAT3 m_Binormal;	//�]�@��
	DirectX::XMFLOAT3 m_Tangent;	//�ڐ�
	int m_BoneID[4];				//���ԍ�(�ő�̍��ԍ��͎g�p���Ȃ�)
	float m_BoneWeight[4];			//����d

	//�v���g�^�C�v�錾
	explicit VERTEX_MNB() noexcept :
		m_Pos(), m_UV(), m_Normal(), m_Binormal(), m_Tangent(),
		m_BoneID(), m_BoneWeight()
	{
		//UV��񏉊���
		m_Tangent  = { 1.0f, 0.0f, 0.0f };
		m_Binormal = { 0.0f, 1.0f, 0.0f };
		m_Normal   = { 0.0f, 0.0f, 1.0f };

		//����񏉊���
		for (auto& b : m_BoneID)
			b = MAX_BONE - 1;
		for (auto& w : m_BoneWeight)
			w = 0.0f;
	}

	~VERTEX_MNB() noexcept
	{}
};

//***** ���_�V�F�[�_�p�f�[�^ *****
template<class T>
class VS_DATA
{
public:

	//�ϐ��錾
	std::vector<T> m_Vertices;		//���_���
	std::vector<UINT> m_Indices;	//�C���f�b�N�X���

	//�v���g�^�C�v�錾
	explicit VS_DATA() noexcept : m_Vertices(0), m_Indices(0) {}

	explicit VS_DATA(std::vector<T> Vertices, std::vector<UINT> Indices, bool bTriangle = true) :
		m_Vertices(std::move(Vertices)), m_Indices(std::move(Indices))
	{
		//�O�p�|���S���̏ꍇ�����m�F
		if (!bTriangle)
			return;
		if (m_Vertices.size() <= 2)
			throw ERROR_EX2("�y�x���z�|���S���̒��_����2�ȉ��I");
		if (m_Indices.size() <= 0)
			throw ERROR_EX2("�y�x���z�|���S���̃C���f�b�N�X���͋�ł��I");
		if (m_Indices.size() % 3 != 0)
			throw ERROR_EX2("�y�x���z�|���S���̃C���f�b�N�X����3�Ŋ���؂�Ȃ��I");
	}

	~VS_DATA() noexcept {}

	void SetVertexPos(DirectX::XMFLOAT4X4 mtxTransform) noexcept	//���_�ʒu�ݒ�i�����ό`�j
	{
		namespace dx = DirectX;

		//�s��쐬
		const dx::XMFLOAT4X4 transform = mtxTransform;
		const dx::XMMATRIX mtx = dx::XMLoadFloat4x4(&transform);

		//����������
		for (auto& v : m_Vertices) {
			const dx::XMVECTOR Pos = dx::XMLoadFloat3(&v.m_Pos);
			dx::XMStoreFloat3(&v.m_Pos, dx::XMVector3Transform(Pos, mtx));
		}
	}

	void SetVertexNormal()	//���_�@���ݒ�i�O�p�|���S������j
	{
		namespace dx = DirectX;

		//��O����
		if (m_Vertices.size() <= 2 || m_Indices.size() % 3 != 0)
			throw ERROR_EX2("�y�x���z���_��񂪎O�p�|���S���̗v���𖞂����Ȃ��I");
		if (m_Indices.size() <= 0)
			throw ERROR_EX2("�y�x���z�C���f�b�N�X���͋�ł��I");

		//�@���v�Z
		for (size_t i = 0, Cnt = m_Indices.size(); i < Cnt; i += 3)
		{
			//�|���S����3���_�擾
			auto& vtx0 = m_Vertices[m_Indices[i]];
			auto& vtx1 = m_Vertices[m_Indices[i + 1]];
			auto& vtx2 = m_Vertices[m_Indices[i + 2]];

			//�O�ςŖ@���Z�o
			const dx::XMFLOAT3 v1 = { vtx1.m_Pos.x - vtx0.m_Pos.x, vtx1.m_Pos.y - vtx0.m_Pos.y, vtx1.m_Pos.z - vtx0.m_Pos.z };
			const dx::XMFLOAT3 v2 = { vtx2.m_Pos.x - vtx0.m_Pos.x, vtx2.m_Pos.y - vtx0.m_Pos.y, vtx2.m_Pos.z - vtx0.m_Pos.z };
			const dx::XMVECTOR n = dx::XMVector3Normalize(dx::XMVector3Cross(dx::XMLoadFloat3(&v1), dx::XMLoadFloat3(&v2)));

			//�f�[�^�i�[
			dx::XMStoreFloat3(&vtx0.m_Normal, n);
			dx::XMStoreFloat3(&vtx1.m_Normal, n);
			dx::XMStoreFloat3(&vtx2.m_Normal, n);
		}
	}

	void ResetDataForModel()	//���_�f�[�^�Đݒ�i���f���p�j
	{
		//��O����
		if (m_Vertices.size() <= 2 || m_Indices.size() % 3 != 0)
			throw ERROR_EX2("�y�x���z���_��񂪃|���S���̗v���𖞂����Ȃ��I");
		if (m_Indices.size() <= 0)
			throw ERROR_EX2("�y�x���z�C���f�b�N�X���͋�ł��I");

		//�Đݒ菈��(���_�z����C���f�b�N�X�z��ƈ�v������)
		std::vector<T> Vertices(0);
		for (auto& i : m_Indices) {
			T vtx{};
			vtx.m_Pos = m_Vertices[i].m_Pos;
			Vertices.emplace_back(std::move(vtx));
		}
		m_Vertices = std::move(Vertices);
		for (size_t i = 0, Cnt = m_Indices.size(); i < Cnt; i++)
			m_Indices[i] = static_cast<UINT>(i);

		//UV�ݒ�i�f�o�b�O�p�j
		for (size_t i = 0, Cnt = m_Indices.size() / 3; i < Cnt; i++) {
			m_Vertices[i * 3 + 0].m_UV = { 0.0f, 0.0f };
			m_Vertices[i * 3 + 1].m_UV = { 1.0f, 0.0f };
			m_Vertices[i * 3 + 2].m_UV = { 0.0f, 1.0f };
		}
	}
};
