/**
 * @file vsdPlane.h
 * @brief 板形状テンプレート
 * @author 室谷イアン
 * @date 2022/07/13
 * @履歴 2022/07/13：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Drawer/VertexShaderData.h>
#include <Tool/gMath.h>
#include <array>

//===== クラス定義 =====

//***** 板形状 *****
class VSD_PLANE
{
public:

	template<class V>
	static VS_DATA<V> MakeTessellation(int DivX, int DivY)	//テッセレーション作成
	{
		//前処理
		namespace dx = DirectX;
		if (DivX < 1)
			throw ERROR_EX2("板の横分割数は1以上にしてください。");
		if (DivY < 1)
			throw ERROR_EX2("板の縦分割数は1以上にしてください。");

		//変数宣言
		constexpr float fWidth = 1.0f;
		constexpr float fHeight = 1.0f;		//横幅・縦幅
		const int nVtxNumX = DivX + 1;
		const int nVtxNumY = DivY + 1;		//横・縦頂点数

		//頂点作成
		std::vector<V> aData(nVtxNumX * nVtxNumY);
		const dx::XMVECTOR vBottomLeft = dx::XMVectorSet(-0.5f, -0.5f, 0.0f, 0.0f);		//板の左下頂点
		const float fUnitSizeX = fWidth / DivX;
		const float fUnitSizeY = fHeight / DivY;										//板の分割単位ごとの横幅・縦幅
		for (int y = 0, nIdxCnt = 0; y < nVtxNumY; y++) {
			const float PosY = y * fUnitSizeY;											//行ごとの縦幅
			for (int x = 0; x < nVtxNumX; x++) {
				const dx::XMVECTOR vVertex = dx::XMVectorAdd(vBottomLeft, dx::XMVectorSet(x * fUnitSizeX, PosY, 0.0f, 0.0f));
				dx::XMStoreFloat3(&aData[nIdxCnt].m_Pos, vVertex);
				nIdxCnt++;																//インデックスカウンタ
			}
		}

		//インデックス算出用ラムダ式
		const auto CalcIdx = [nVtxNumX](int x, int y)
		{ return static_cast<unsigned short>(y * nVtxNumX + x); };

		//インデックス作成
		std::vector<unsigned short> Indices;
		Indices.reserve(static_cast<unsigned short>(DivX * DivY * 6));	//サイズ指定
		for (int y = 0; y < DivY; y++) {
			for (int x = 0; x < DivX; x++) {

				//ユニットごとの頂点インデックスを算出
				const std::array<unsigned short, 4> aIndex = {
					CalcIdx(x, y),
					CalcIdx(x + 1, y),
					CalcIdx(x, y + 1),
					CalcIdx(x + 1, y + 1)
				};

				//インデックス格納
				Indices.emplace_back(aIndex[0]);
				Indices.emplace_back(aIndex[2]);
				Indices.emplace_back(aIndex[1]);
				Indices.emplace_back(aIndex[1]);
				Indices.emplace_back(aIndex[2]);
				Indices.emplace_back(aIndex[3]);
			}
		}

		return VS_DATA<V>(std::move(aData), std::move(Indices));
	}

	template<class V>
	static VS_DATA<V> MakeTessellation_Tex(int DivX, int DivY)	//テッセレーション作成（テクスチャあり）
	{
		//前処理
		namespace dx = DirectX;
		if (DivX < 1)
			throw ERROR_EX2("板の横分割数は1以上にしてください。");
		if (DivY < 1)
			throw ERROR_EX2("板の縦分割数は1以上にしてください。");

		//変数宣言
		constexpr float fWidth = 1.0f;
		constexpr float fHeight = 1.0f;		//横幅・縦幅
		const int nVtxNumX = DivX + 1;
		const int nVtxNumY = DivY + 1;		//横・縦頂点数

		//頂点作成
		std::vector<V> aData(nVtxNumX * nVtxNumY);
		const dx::XMVECTOR vBottomLeft = dx::XMVectorSet(-0.5f, -0.5f, 0.0f, 0.0f);		//板の左下頂点
		const float fUnitSizeX = fWidth / DivX;
		const float fUnitSizeY = fHeight / DivY;										//板の分割単位ごとの横幅・縦幅
		for (int y = 0, nIdxCnt = 0; y < nVtxNumY; y++) {
			const float PosY = y * fUnitSizeY;											//行ごとの縦幅
			for (int x = 0; x < nVtxNumX; x++) {
				const dx::XMVECTOR vVertex = dx::XMVectorAdd(vBottomLeft, dx::XMVectorSet(x * fUnitSizeX, PosY, 0.0f, 0.0f));
				dx::XMStoreFloat3(&aData[nIdxCnt].m_Pos, vVertex);
				aData[nIdxCnt].m_UV = { x * fUnitSizeX, 1.0f - y * fUnitSizeY };		//UV情報
				nIdxCnt++;																//インデックスカウンタ
			}
		}

		//インデックス算出用ラムダ式
		const auto CalcIdx = [nVtxNumX](int x, int y)
		{ return static_cast<unsigned short>(y * nVtxNumX + x); };

		//インデックス作成
		std::vector<unsigned short> Indices;
		Indices.reserve(static_cast<unsigned short>(DivX * DivY * 6));	//サイズ指定
		for (int y = 0; y < DivY; y++) {
			for (int x = 0; x < DivX; x++) {

				//ユニットごとの頂点インデックスを算出
				const std::array<unsigned short, 4> aIndex = {
					CalcIdx(x, y),
					CalcIdx(x + 1, y),
					CalcIdx(x, y + 1),
					CalcIdx(x + 1, y + 1)
				};

				//インデックス格納
				Indices.emplace_back(aIndex[0]);
				Indices.emplace_back(aIndex[2]);
				Indices.emplace_back(aIndex[1]);
				Indices.emplace_back(aIndex[1]);
				Indices.emplace_back(aIndex[2]);
				Indices.emplace_back(aIndex[3]);
			}
		}

		return VS_DATA<V>(std::move(aData), std::move(Indices));
	}

	template<class V>
	static VS_DATA<V> MakeData(int DivX = 1, int DivY = 1)		//データ作成
	{
		return MakeTessellation<V>(DivX, DivY);
	}

	template<class V>
	static VS_DATA<V> MakeData_Tex(int DivX = 1, int DivY = 1)	//データ作成（テクスチャあり）
	{
		return MakeTessellation_Tex<V>(DivX, DivY);
	}

	template<class V>
	static VS_DATA<V> MakeData_Model(int DivX = 1, int DivY = 1)	//データ作成（モデル用）
	{
		VS_DATA<V> vsd = MakeTessellation_Tex<V>(DivX, DivY);

		//頂点情報再作成
		std::vector<V> Vertices(0);
		for (auto& i : vsd.m_Indices) {
			V vtx{};
			vtx.m_Pos = vsd.m_Vertices[i].m_Pos;
			vtx.m_UV = vsd.m_Vertices[i].m_UV;
			Vertices.emplace_back(std::move(vtx));
		}
		vsd.m_Vertices = std::move(Vertices);
		for (size_t i = 0, Cnt = vsd.m_Indices.size(); i < Cnt; i++)
			vsd.m_Indices[i] = static_cast<unsigned short>(i);

		//法線計算
		vsd.SetVertexNormal();
		return vsd;
	}

private:

	//プロトタイプ宣言
	VSD_PLANE() noexcept {}
	~VSD_PLANE() noexcept {}
};
