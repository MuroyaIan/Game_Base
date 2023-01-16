/**
 * @file vsdGrid.h
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
class VSD_GRID
{
public:

	template<class V>
	static VS_DATA<V> MakeData(int DivX = 1, int DivY = 1)	//データ作成
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
		std::vector<V> aData(static_cast<size_t>(nVtxNumX) * nVtxNumY);
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
		{ return static_cast<UINT>(y * nVtxNumX + x); };

		//インデックス作成
		std::vector<UINT> Indices;
		Indices.reserve(static_cast<size_t>(DivX) * DivY * 4 + (static_cast<size_t>(DivX) + DivY) * 2);	//サイズ指定
		for (int y = 0; y < DivY; y++) {
			for (int x = 0; x < DivX; x++) {

				//ユニットごとの頂点インデックスを算出
				const std::array<UINT, 3> aIndex = {
					CalcIdx(x, y),
					CalcIdx(x + 1, y),
					CalcIdx(x, y + 1),
				};

				//インデックス格納
				Indices.emplace_back(aIndex[0]);
				Indices.emplace_back(aIndex[1]);
				Indices.emplace_back(aIndex[0]);
				Indices.emplace_back(aIndex[2]);
			}
		}
		for (int x = 0, y = DivY; x < DivX; x++) {

			//インデックス格納（一番上の行）
			Indices.emplace_back(CalcIdx(x, y));
			Indices.emplace_back(CalcIdx(x + 1, y));
		}
		for (int x = DivX, y = 0; y < DivY; y++) {

			//インデックス格納（一番右の列）
			Indices.emplace_back(CalcIdx(x, y));
			Indices.emplace_back(CalcIdx(x, y + 1));
		}

		return VS_DATA<V>(std::move(aData), std::move(Indices), false);
	}

private:

	//プロトタイプ宣言
	explicit VSD_GRID() noexcept {}

	~VSD_GRID() noexcept {}
};
