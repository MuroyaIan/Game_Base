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
	VTX_COLOR() noexcept : r(0), g(0), b(0), a(0)
	{}

	VTX_COLOR(unsigned char rIn, unsigned char gIn, unsigned char bIn, unsigned char aIn) noexcept :
		r(rIn), g(gIn), b(bIn), a(aIn)
	{}

	~VTX_COLOR() noexcept
	{}
};

//===== クラス定義 =====
class VERTEX					//頂点情報(ベースクラス)
{
public:

	//変数宣言
	DirectX::XMFLOAT3 m_Pos;	//位置

	//プロトタイプ宣言
	explicit VERTEX() noexcept : m_Pos()
	{}

	explicit VERTEX(DirectX::XMFLOAT3 Pos) noexcept : m_Pos(Pos)
	{}

	~VERTEX() noexcept
	{}
};

class VERTEX_C					//色付き
{
public:

	//変数宣言
	DirectX::XMFLOAT3 m_Pos;	//位置
	VTX_COLOR m_Color;			//色

	//プロトタイプ宣言
	explicit VERTEX_C() noexcept : m_Pos(), m_Color()
	{}

	explicit VERTEX_C(DirectX::XMFLOAT3 Pos, VTX_COLOR Color) noexcept :
		m_Pos(Pos), m_Color(Color)
	{}

	~VERTEX_C() noexcept
	{}
};

class VERTEX_T					//UV情報付き
{
public:

	//変数宣言
	DirectX::XMFLOAT3 m_Pos;	//位置
	DirectX::XMFLOAT2 m_UV;		//UV座標

	//プロトタイプ宣言
	explicit VERTEX_T() noexcept : m_Pos(), m_UV()
	{}

	explicit VERTEX_T(DirectX::XMFLOAT3 Pos, DirectX::XMFLOAT2 uv) noexcept :
		m_Pos(Pos), m_UV(uv)
	{}

	~VERTEX_T() noexcept
	{}
};

class VERTEX_M						//モデル用
{
public:

	//変数宣言
	DirectX::XMFLOAT3 m_Pos;		//位置
	DirectX::XMFLOAT2 m_UV;			//UV座標
	DirectX::XMFLOAT3 m_Normal;		//法線

	//プロトタイプ宣言
	explicit VERTEX_M() noexcept : m_Pos(), m_UV(), m_Normal()
	{}

	~VERTEX_M() noexcept
	{}
};

class VERTEX_MB						//モデル用（アニメーション付き）
{
public:

	//変数宣言
	DirectX::XMFLOAT3 m_Pos;		//位置
	DirectX::XMFLOAT2 m_UV;			//UV座標
	DirectX::XMFLOAT3 m_Normal;		//法線
	int m_BoneID[4];				//骨番号(最大の骨番号は使用しない)
	float m_BoneWeight[4];			//骨比重

	//プロトタイプ宣言
	explicit VERTEX_MB() noexcept :
		m_Pos(), m_UV(), m_Normal(), m_BoneID(), m_BoneWeight()
	{
		//骨情報初期化
		for (auto& b : m_BoneID)
			b = MAX_BONE - 1;
		for (auto& w : m_BoneWeight)
			w = 0.0f;
	}

	~VERTEX_MB() noexcept
	{}
};

class VERTEX_MN						//モデル用（法線マッピング）
{
public:

	//変数宣言
	DirectX::XMFLOAT3 m_Pos;		//位置
	DirectX::XMFLOAT2 m_UV;			//UV座標
	DirectX::XMFLOAT3 m_Normal;		//法線
	DirectX::XMFLOAT3 m_Binormal;	//従法線
	DirectX::XMFLOAT3 m_Tangent;	//接線

	//プロトタイプ宣言
	explicit VERTEX_MN() noexcept : m_Pos(), m_UV(), m_Normal(), m_Binormal(), m_Tangent()
	{
		//UV情報初期化
		m_Tangent  = { 1.0f, 0.0f, 0.0f };
		m_Binormal = { 0.0f, 1.0f, 0.0f };
		m_Normal   = { 0.0f, 0.0f, 1.0f };
	}

	~VERTEX_MN() noexcept
	{}
};

class VERTEX_MNB					//モデル用（法線マッピング+アニメーション付き）
{
public:

	//変数宣言
	DirectX::XMFLOAT3 m_Pos;		//位置
	DirectX::XMFLOAT2 m_UV;			//UV座標
	DirectX::XMFLOAT3 m_Normal;		//法線
	DirectX::XMFLOAT3 m_Binormal;	//従法線
	DirectX::XMFLOAT3 m_Tangent;	//接線
	int m_BoneID[4];				//骨番号(最大の骨番号は使用しない)
	float m_BoneWeight[4];			//骨比重

	//プロトタイプ宣言
	explicit VERTEX_MNB() noexcept :
		m_Pos(), m_UV(), m_Normal(), m_Binormal(), m_Tangent(),
		m_BoneID(), m_BoneWeight()
	{
		//UV情報初期化
		m_Tangent  = { 1.0f, 0.0f, 0.0f };
		m_Binormal = { 0.0f, 1.0f, 0.0f };
		m_Normal   = { 0.0f, 0.0f, 1.0f };

		//骨情報初期化
		for (auto& b : m_BoneID)
			b = MAX_BONE - 1;
		for (auto& w : m_BoneWeight)
			w = 0.0f;
	}

	~VERTEX_MNB() noexcept
	{}
};

//***** 頂点シェーダ用データ *****
template<class T>
class VS_DATA
{
public:

	//変数宣言
	std::vector<T> m_Vertices;		//頂点情報
	std::vector<UINT> m_Indices;	//インデックス情報

	//プロトタイプ宣言
	explicit VS_DATA() noexcept : m_Vertices(0), m_Indices(0) {}

	explicit VS_DATA(std::vector<T> Vertices, std::vector<UINT> Indices, bool bTriangle = true) :
		m_Vertices(std::move(Vertices)), m_Indices(std::move(Indices))
	{
		//三角ポリゴンの場合条件確認
		if (!bTriangle)
			return;
		if (m_Vertices.size() <= 2)
			throw ERROR_EX2("【警告】ポリゴンの頂点数が2以下！");
		if (m_Indices.size() <= 0)
			throw ERROR_EX2("【警告】ポリゴンのインデックス情報は空です！");
		if (m_Indices.size() % 3 != 0)
			throw ERROR_EX2("【警告】ポリゴンのインデックス数が3で割り切れない！");
	}

	~VS_DATA() noexcept {}

	void SetVertexPos(DirectX::XMFLOAT4X4 mtxTransform) noexcept	//頂点位置設定（初期変形）
	{
		namespace dx = DirectX;

		//行列作成
		const dx::XMFLOAT4X4 transform = mtxTransform;
		const dx::XMMATRIX mtx = dx::XMLoadFloat4x4(&transform);

		//初期化処理
		for (auto& v : m_Vertices) {
			const dx::XMVECTOR Pos = dx::XMLoadFloat3(&v.m_Pos);
			dx::XMStoreFloat3(&v.m_Pos, dx::XMVector3Transform(Pos, mtx));
		}
	}

	void SetVertexNormal()	//頂点法線設定（三角ポリゴン限定）
	{
		namespace dx = DirectX;

		//例外処理
		if (m_Vertices.size() <= 2 || m_Indices.size() % 3 != 0)
			throw ERROR_EX2("【警告】頂点情報が三角ポリゴンの要件を満たさない！");
		if (m_Indices.size() <= 0)
			throw ERROR_EX2("【警告】インデックス情報は空です！");

		//法線計算
		for (size_t i = 0, Cnt = m_Indices.size(); i < Cnt; i += 3)
		{
			//ポリゴンの3頂点取得
			auto& vtx0 = m_Vertices[m_Indices[i]];
			auto& vtx1 = m_Vertices[m_Indices[i + 1]];
			auto& vtx2 = m_Vertices[m_Indices[i + 2]];

			//外積で法線算出
			const dx::XMFLOAT3 v1 = { vtx1.m_Pos.x - vtx0.m_Pos.x, vtx1.m_Pos.y - vtx0.m_Pos.y, vtx1.m_Pos.z - vtx0.m_Pos.z };
			const dx::XMFLOAT3 v2 = { vtx2.m_Pos.x - vtx0.m_Pos.x, vtx2.m_Pos.y - vtx0.m_Pos.y, vtx2.m_Pos.z - vtx0.m_Pos.z };
			const dx::XMVECTOR n = dx::XMVector3Normalize(dx::XMVector3Cross(dx::XMLoadFloat3(&v1), dx::XMLoadFloat3(&v2)));

			//データ格納
			dx::XMStoreFloat3(&vtx0.m_Normal, n);
			dx::XMStoreFloat3(&vtx1.m_Normal, n);
			dx::XMStoreFloat3(&vtx2.m_Normal, n);
		}
	}

	void ResetDataForModel()	//頂点データ再設定（モデル用）
	{
		//例外処理
		if (m_Vertices.size() <= 2 || m_Indices.size() % 3 != 0)
			throw ERROR_EX2("【警告】頂点情報がポリゴンの要件を満たさない！");
		if (m_Indices.size() <= 0)
			throw ERROR_EX2("【警告】インデックス情報は空です！");

		//再設定処理(頂点配列をインデックス配列と一致させる)
		std::vector<T> Vertices(0);
		for (auto& i : m_Indices) {
			T vtx{};
			vtx.m_Pos = m_Vertices[i].m_Pos;
			Vertices.emplace_back(std::move(vtx));
		}
		m_Vertices = std::move(Vertices);
		for (size_t i = 0, Cnt = m_Indices.size(); i < Cnt; i++)
			m_Indices[i] = static_cast<UINT>(i);

		//UV設定（デバッグ用）
		for (size_t i = 0, Cnt = m_Indices.size() / 3; i < Cnt; i++) {
			m_Vertices[i * 3 + 0].m_UV = { 0.0f, 0.0f };
			m_Vertices[i * 3 + 1].m_UV = { 1.0f, 0.0f };
			m_Vertices[i * 3 + 2].m_UV = { 0.0f, 1.0f };
		}
	}
};
