/**
 * @file vsdSphere.h
 * @brief 球体形状テンプレート
 * @author 室谷イアン
 * @date 2022/07/10
 * @履歴 2022/07/10：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Drawer/VertexShaderData.h>
#include <Tool/gMath.h>

//===== クラス定義 =====

//***** 球体形状 *****
class VSD_SPHERE
{
public:

	//プロトタイプ宣言
	template<class V>
	static VS_DATA<V> MakeTessellation(int LatDiv, int LongDiv)		//テッセレーション作成
	{
		//前処理
		namespace dx = DirectX;
		if (LatDiv < 3)
			throw ERROR_EX2("緯度分割数は3以上にしてください。");
		if (LongDiv < 3)
			throw ERROR_EX2("経度分割数は3以上にしてください。");

		//変数宣言
		constexpr float fRadius = 0.5f;											//半径
		const dx::XMVECTOR vBase = dx::XMVectorSet(0.0f, fRadius, 0.0f, 0.0f);	//基準用頂点ベクトル
		const float fLatAngle = fPI / LatDiv;									//緯度角(180で分割)
		const float fLongAngle = 2.0f * fPI / LongDiv;							//経度角(360で分割)

		//頂点作成
		std::vector<V> aData;
		for (int iLat = 1; iLat < LatDiv; iLat++) {
			const dx::XMVECTOR vLatBase = dx::XMVector3Transform(vBase, dx::XMMatrixRotationX(fLatAngle * iLat));		//緯度の分割分ごとに
			for (int iLong = 0; iLong < LongDiv; iLong++) {
				aData.emplace_back();
				dx::XMVECTOR vVertex = dx::XMVector3Transform(vLatBase, dx::XMMatrixRotationY(fLongAngle * iLong));		//経度の分割数分の頂点を作成
				dx::XMStoreFloat3(&aData.back().m_Pos, vVertex);
			}
		}

		//北極点頂点作成
		const UINT usIdxNP = (UINT)aData.size();
		aData.emplace_back();
		dx::XMStoreFloat3(&aData.back().m_Pos, vBase);

		//南極点頂点作成
		const UINT usIdxSP = (UINT)aData.size();
		aData.emplace_back();
		dx::XMStoreFloat3(&aData.back().m_Pos, dx::XMVectorNegate(vBase));

		//インデックス算出用ラムダ式
		const auto CalcIdx = [LongDiv](UINT iLat, UINT iLong)
		{ return static_cast<UINT>(iLat * LongDiv + iLong); };

		//インデックスリスト作成
		std::vector<UINT> Indices;
		const UINT Mod = static_cast<UINT>(LongDiv);
		for (UINT iLat = 0; iLat < static_cast<UINT>(LatDiv - 2); iLat++) {
			for (UINT iLong = 0; iLong < static_cast<UINT>(LongDiv); iLong++) {
				Indices.emplace_back(CalcIdx(iLat,		iLong));
				Indices.emplace_back(CalcIdx(iLat + 1u, iLong));
				Indices.emplace_back(CalcIdx(iLat,		(iLong + 1u) % Mod));
				Indices.emplace_back(CalcIdx(iLat,		(iLong + 1u) % Mod));
				Indices.emplace_back(CalcIdx(iLat + 1u, iLong));
				Indices.emplace_back(CalcIdx(iLat + 1u, (iLong + 1u) % Mod));
			}
		}

		//極点周りのインデックスリスト作成
		const UINT usLatDiv = static_cast<UINT>(LatDiv);
		for (UINT iLong = 0; iLong < static_cast<UINT>(LongDiv); iLong++) {

			//北極点
			Indices.push_back(usIdxNP);
			Indices.push_back(CalcIdx(0u, iLong));
			Indices.push_back(CalcIdx(0u, (iLong + 1u) % Mod));

			//南極点
			Indices.push_back(CalcIdx(usLatDiv - 2u, (iLong + 1u) % Mod));
			Indices.push_back(CalcIdx(usLatDiv - 2u, iLong));
			Indices.push_back(usIdxSP);
		}

		return VS_DATA<V>(std::move(aData), std::move(Indices));
	}

	template<class V>
	static VS_DATA<V> MakeData(int LatDiv = 12, int LongDiv = 12)	//データ作成
	{
		return MakeTessellation<V>(LatDiv, LongDiv);
	}

	template<class V>
	static VS_DATA<V> MakeData_Model(int LatDiv = 12, int LongDiv = 12)		//データ作成（モデル用）
	{
		VS_DATA<V> vsd = MakeTessellation<V>(LatDiv, LongDiv);
		vsd.ResetDataForModel();
		vsd.SetVertexNormal();
		return vsd;
	}

private:

	//プロトタイプ宣言
	VSD_SPHERE() noexcept {}
	~VSD_SPHERE() noexcept {}
};
