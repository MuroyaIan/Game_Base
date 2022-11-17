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
	VTX_COLOR() noexcept : r(0), g(0), b(0), a(0) {}
	VTX_COLOR(unsigned char rIn, unsigned char gIn, unsigned char bIn, unsigned char aIn) noexcept :
		r(rIn), g(gIn), b(bIn), a(aIn) {}
	~VTX_COLOR() noexcept {}
};

//===== �N���X��` =====

//***** ���_��� *****
class VERTEX					//�x�[�X�N���X
{
public:

	//�ϐ��錾
	DirectX::XMFLOAT3 m_Pos;	//�ʒu

	//�v���g�^�C�v�錾
	VERTEX() noexcept : m_Pos() {}
	VERTEX(DirectX::XMFLOAT3 Pos) noexcept : m_Pos(Pos) {}
	~VERTEX() noexcept {}
};

class VERTEX_C					//�F�t��
{
public:

	//�ϐ��錾
	DirectX::XMFLOAT3 m_Pos;	//�ʒu
	VTX_COLOR m_Color;			//�F

	//�v���g�^�C�v�錾
	VERTEX_C() noexcept : m_Pos(), m_Color() {}
	VERTEX_C(DirectX::XMFLOAT3 Pos, VTX_COLOR Color) noexcept : m_Pos(Pos), m_Color(Color) {}
	~VERTEX_C() noexcept {}
};

class VERTEX_T					//UV���t��
{
public:

	//�ϐ��錾
	DirectX::XMFLOAT3 m_Pos;	//�ʒu
	DirectX::XMFLOAT2 m_UV;		//UV���W

	//�v���g�^�C�v�錾
	VERTEX_T() noexcept : m_Pos(), m_UV() {}
	VERTEX_T(DirectX::XMFLOAT3 Pos, DirectX::XMFLOAT2 uv) noexcept : m_Pos(Pos), m_UV(uv) {}
	~VERTEX_T() noexcept {}
};

class VERTEX_M						//���f���p
{
public:

	//�ϐ��錾
	DirectX::XMFLOAT3 m_Pos;		//�ʒu
	DirectX::XMFLOAT2 m_UV;			//UV���W
	DirectX::XMFLOAT3 m_Normal;		//�@��

	//�v���g�^�C�v�錾
	VERTEX_M() noexcept : m_Pos(), m_UV(), m_Normal() {}
	VERTEX_M(DirectX::XMFLOAT3 Pos, DirectX::XMFLOAT2 uv, DirectX::XMFLOAT3 Normal) noexcept :
		m_Pos(Pos), m_UV(uv), m_Normal(Normal) {}
	~VERTEX_M() noexcept {}
};

class VERTEX_MB							//���f���p�i���t���j
{
public:

	//�ϐ��錾
	DirectX::XMFLOAT3 m_Pos;			//�ʒu
	DirectX::XMFLOAT2 m_UV;				//UV���W
	DirectX::XMFLOAT3 m_Normal;			//�@��
	int m_BoneID[4];					//���ԍ�
	float m_BoneWeight[4];				//����d

	//�v���g�^�C�v�錾
	VERTEX_MB() noexcept : m_Pos(), m_UV(), m_Normal(), m_BoneID(), m_BoneWeight()
	{
		for (auto& b : m_BoneID)
			b = MAX_BONE - 1;
		for (auto& w : m_BoneWeight)
			w = 0.0f;
	}
	VERTEX_MB(DirectX::XMFLOAT3 Pos, DirectX::XMFLOAT2 uv, DirectX::XMFLOAT3 Normal) noexcept :
		m_Pos(Pos), m_UV(uv), m_Normal(Normal), m_BoneID(), m_BoneWeight()
	{
		for (auto& b : m_BoneID)
			b = MAX_BONE - 1;
		for (auto& w : m_BoneWeight)
			w = 0.0f;
	}
	~VERTEX_MB() noexcept {}
};

//***** ���_�V�F�[�_�p�f�[�^ *****
template<class T>
class VS_DATA
{
public:

	//�ϐ��錾
	std::vector<T> m_Vertices;				//���_���
	std::vector<unsigned short> m_Indices;	//�C���f�b�N�X���

	//�v���g�^�C�v�錾
	VS_DATA() noexcept : m_Vertices(0), m_Indices(0) {}
	VS_DATA(std::vector<T> Vertices, std::vector<unsigned short> Indices, bool bTriangle = true) :
		m_Vertices(std::move(Vertices)), m_Indices(std::move(Indices))
	{
		if (m_Vertices.size() <= 2 && bTriangle)
			throw ERROR_EX2("�y�x���z���_����2�ȉ��I");
		if (m_Indices.size() % 3 != 0 && bTriangle)
			throw ERROR_EX2("�y�x���z�C���f�b�N�X����3�Ŋ���؂�Ȃ��I");
	}
	~VS_DATA() noexcept {}

	void InitSize(DirectX::XMFLOAT4X4 mtxScale) noexcept	//�T�C�Y������
	{
		//�s��쐬
		const DirectX::XMFLOAT4X4 scale = mtxScale;
		const DirectX::XMMATRIX mtx = DirectX::XMLoadFloat4x4(&scale);

		//����������
		for (auto& v : m_Vertices) {
			const DirectX::XMVECTOR Pos = DirectX::XMLoadFloat3(&v.m_Pos);
			DirectX::XMStoreFloat3(&v.m_Pos, DirectX::XMVector3Transform(Pos, mtx));
		}
	}

	void SetVertexNormal() noexcept(!IS_DEBUG)				//�@���v�Z
	{

#ifdef _DEBUG

		//��O����
		if (m_Vertices.size() <= 2 || m_Indices.size() % 3 != 0)
			throw ERROR_EX2("�y�x���z���_��񂪃|���S���̗v���𖞂����Ȃ��I");
		if (m_Indices.size() <= 0)
			throw ERROR_EX2("�y�x���z�C���f�b�N�X���͋�ł��I");

#endif // _DEBUG

		//�v�Z����
		for (size_t i = 0, Cnt = m_Indices.size(); i < Cnt; i += 3)
		{
			//�|���S����3���_�擾
			auto& vtx0 = m_Vertices[m_Indices[i]];
			auto& vtx1 = m_Vertices[m_Indices[i + 1]];
			auto& vtx2 = m_Vertices[m_Indices[i + 2]];

			//�O�ςŖ@���Z�o
			DirectX::XMFLOAT3 v1 = { vtx1.m_Pos.x - vtx0.m_Pos.x, vtx1.m_Pos.y - vtx0.m_Pos.y, vtx1.m_Pos.z - vtx0.m_Pos.z };
			DirectX::XMFLOAT3 v2 = { vtx2.m_Pos.x - vtx0.m_Pos.x, vtx2.m_Pos.y - vtx0.m_Pos.y, vtx2.m_Pos.z - vtx0.m_Pos.z };
			const auto n = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&v1), DirectX::XMLoadFloat3(&v2)));

			//�f�[�^�i�[
			DirectX::XMStoreFloat3(&vtx0.m_Normal, n);
			DirectX::XMStoreFloat3(&vtx1.m_Normal, n);
			DirectX::XMStoreFloat3(&vtx2.m_Normal, n);
		}
	}

	void ResetDataForModel() noexcept(!IS_DEBUG)			//���_�f�[�^�Čv�Z�i���f���p�j
	{

#ifdef _DEBUG

		//��O����
		if (m_Vertices.size() <= 2 || m_Indices.size() % 3 != 0)
			throw ERROR_EX2("�y�x���z���_��񂪃|���S���̗v���𖞂����Ȃ��I");
		if (m_Indices.size() <= 0)
			throw ERROR_EX2("�y�x���z�C���f�b�N�X���͋�ł��I");

#endif // _DEBUG

		//�Čv�Z����
		std::vector<T> Vertices(0);
		for (auto& i : m_Indices) {
			T vtx{};
			vtx.m_Pos = m_Vertices[i].m_Pos;
			Vertices.emplace_back(std::move(vtx));
		}
		m_Vertices = std::move(Vertices);
		for (size_t i = 0, Cnt = m_Indices.size(); i < Cnt; i++)
			m_Indices[i] = static_cast<unsigned short>(i);

		//UV�쐬�i�f�o�b�O�p�A�e�N�X�`���Ȃ��j
		for (size_t i = 0, Cnt = m_Indices.size() / 3; i < Cnt; i++) {
			m_Vertices[i * 3 + 0].m_UV = { 0.0f, 0.0f };
			m_Vertices[i * 3 + 1].m_UV = { 1.0f, 0.0f };
			m_Vertices[i * 3 + 2].m_UV = { 0.0f, 1.0f };
		}
	}
};
