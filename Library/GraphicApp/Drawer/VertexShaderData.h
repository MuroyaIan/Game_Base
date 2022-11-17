/**
 * @file VertexShaderData.h
 * @brief 頂点シェーダ用データ
 * @author 室谷イアン
 * @date 2022/07/06
 * @履歴 2022/07/06：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Graphic.h>

//===== グローバル定数宣言 =====
constexpr int MAX_BONE = 250;		//骨の最大本数

//===== 構造体宣言 =====
struct VTX_COLOR
{
	//変数宣言
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	//プロトタイプ宣言
	VTX_COLOR() noexcept : r(0), g(0), b(0), a(0) {}
	VTX_COLOR(unsigned char rIn, unsigned char gIn, unsigned char bIn, unsigned char aIn) noexcept :
		r(rIn), g(gIn), b(bIn), a(aIn) {}
	~VTX_COLOR() noexcept {}
};

//===== クラス定義 =====

//***** 頂点情報 *****
class VERTEX					//ベースクラス
{
public:

	//変数宣言
	DirectX::XMFLOAT3 m_Pos;	//位置

	//プロトタイプ宣言
	VERTEX() noexcept : m_Pos() {}
	VERTEX(DirectX::XMFLOAT3 Pos) noexcept : m_Pos(Pos) {}
	~VERTEX() noexcept {}
};

class VERTEX_C					//色付き
{
public:

	//変数宣言
	DirectX::XMFLOAT3 m_Pos;	//位置
	VTX_COLOR m_Color;			//色

	//プロトタイプ宣言
	VERTEX_C() noexcept : m_Pos(), m_Color() {}
	VERTEX_C(DirectX::XMFLOAT3 Pos, VTX_COLOR Color) noexcept : m_Pos(Pos), m_Color(Color) {}
	~VERTEX_C() noexcept {}
};

class VERTEX_T					//UV情報付き
{
public:

	//変数宣言
	DirectX::XMFLOAT3 m_Pos;	//位置
	DirectX::XMFLOAT2 m_UV;		//UV座標

	//プロトタイプ宣言
	VERTEX_T() noexcept : m_Pos(), m_UV() {}
	VERTEX_T(DirectX::XMFLOAT3 Pos, DirectX::XMFLOAT2 uv) noexcept : m_Pos(Pos), m_UV(uv) {}
	~VERTEX_T() noexcept {}
};

class VERTEX_M						//モデル用
{
public:

	//変数宣言
	DirectX::XMFLOAT3 m_Pos;		//位置
	DirectX::XMFLOAT2 m_UV;			//UV座標
	DirectX::XMFLOAT3 m_Normal;		//法線

	//プロトタイプ宣言
	VERTEX_M() noexcept : m_Pos(), m_UV(), m_Normal() {}
	VERTEX_M(DirectX::XMFLOAT3 Pos, DirectX::XMFLOAT2 uv, DirectX::XMFLOAT3 Normal) noexcept :
		m_Pos(Pos), m_UV(uv), m_Normal(Normal) {}
	~VERTEX_M() noexcept {}
};

class VERTEX_MB							//モデル用（骨付き）
{
public:

	//変数宣言
	DirectX::XMFLOAT3 m_Pos;			//位置
	DirectX::XMFLOAT2 m_UV;				//UV座標
	DirectX::XMFLOAT3 m_Normal;			//法線
	int m_BoneID[4];					//骨番号
	float m_BoneWeight[4];				//骨比重

	//プロトタイプ宣言
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

//***** 頂点シェーダ用データ *****
template<class T>
class VS_DATA
{
public:

	//変数宣言
	std::vector<T> m_Vertices;				//頂点情報
	std::vector<unsigned short> m_Indices;	//インデックス情報

	//プロトタイプ宣言
	VS_DATA() noexcept : m_Vertices(0), m_Indices(0) {}
	VS_DATA(std::vector<T> Vertices, std::vector<unsigned short> Indices, bool bTriangle = true) :
		m_Vertices(std::move(Vertices)), m_Indices(std::move(Indices))
	{
		if (m_Vertices.size() <= 2 && bTriangle)
			throw ERROR_EX2("【警告】頂点数が2以下！");
		if (m_Indices.size() % 3 != 0 && bTriangle)
			throw ERROR_EX2("【警告】インデックス数が3で割り切れない！");
	}
	~VS_DATA() noexcept {}

	void InitSize(DirectX::XMFLOAT4X4 mtxScale) noexcept	//サイズ初期化
	{
		//行列作成
		const DirectX::XMFLOAT4X4 scale = mtxScale;
		const DirectX::XMMATRIX mtx = DirectX::XMLoadFloat4x4(&scale);

		//初期化処理
		for (auto& v : m_Vertices) {
			const DirectX::XMVECTOR Pos = DirectX::XMLoadFloat3(&v.m_Pos);
			DirectX::XMStoreFloat3(&v.m_Pos, DirectX::XMVector3Transform(Pos, mtx));
		}
	}

	void SetVertexNormal() noexcept(!IS_DEBUG)				//法線計算
	{

#ifdef _DEBUG

		//例外処理
		if (m_Vertices.size() <= 2 || m_Indices.size() % 3 != 0)
			throw ERROR_EX2("【警告】頂点情報がポリゴンの要件を満たさない！");
		if (m_Indices.size() <= 0)
			throw ERROR_EX2("【警告】インデックス情報は空です！");

#endif // _DEBUG

		//計算処理
		for (size_t i = 0, Cnt = m_Indices.size(); i < Cnt; i += 3)
		{
			//ポリゴンの3頂点取得
			auto& vtx0 = m_Vertices[m_Indices[i]];
			auto& vtx1 = m_Vertices[m_Indices[i + 1]];
			auto& vtx2 = m_Vertices[m_Indices[i + 2]];

			//外積で法線算出
			DirectX::XMFLOAT3 v1 = { vtx1.m_Pos.x - vtx0.m_Pos.x, vtx1.m_Pos.y - vtx0.m_Pos.y, vtx1.m_Pos.z - vtx0.m_Pos.z };
			DirectX::XMFLOAT3 v2 = { vtx2.m_Pos.x - vtx0.m_Pos.x, vtx2.m_Pos.y - vtx0.m_Pos.y, vtx2.m_Pos.z - vtx0.m_Pos.z };
			const auto n = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&v1), DirectX::XMLoadFloat3(&v2)));

			//データ格納
			DirectX::XMStoreFloat3(&vtx0.m_Normal, n);
			DirectX::XMStoreFloat3(&vtx1.m_Normal, n);
			DirectX::XMStoreFloat3(&vtx2.m_Normal, n);
		}
	}

	void ResetDataForModel() noexcept(!IS_DEBUG)			//頂点データ再計算（モデル用）
	{

#ifdef _DEBUG

		//例外処理
		if (m_Vertices.size() <= 2 || m_Indices.size() % 3 != 0)
			throw ERROR_EX2("【警告】頂点情報がポリゴンの要件を満たさない！");
		if (m_Indices.size() <= 0)
			throw ERROR_EX2("【警告】インデックス情報は空です！");

#endif // _DEBUG

		//再計算処理
		std::vector<T> Vertices(0);
		for (auto& i : m_Indices) {
			T vtx{};
			vtx.m_Pos = m_Vertices[i].m_Pos;
			Vertices.emplace_back(std::move(vtx));
		}
		m_Vertices = std::move(Vertices);
		for (size_t i = 0, Cnt = m_Indices.size(); i < Cnt; i++)
			m_Indices[i] = static_cast<unsigned short>(i);

		//UV作成（デバッグ用、テクスチャなし）
		for (size_t i = 0, Cnt = m_Indices.size() / 3; i < Cnt; i++) {
			m_Vertices[i * 3 + 0].m_UV = { 0.0f, 0.0f };
			m_Vertices[i * 3 + 1].m_UV = { 1.0f, 0.0f };
			m_Vertices[i * 3 + 2].m_UV = { 0.0f, 1.0f };
		}
	}
};
