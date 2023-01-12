/**
 * @file vsdCone.h
 * @brief 角柱形状テンプレート
 * @author 室谷イアン
 * @date 2022/07/09
 * @履歴 2022/07/09：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Drawer/VertexShaderData.h>
#include <Tool/gMath.h>

//===== クラス定義 =====

//***** 角柱形状 *****
class VSD_PRISM
{
public:

	//プロトタイプ宣言
	template<class V>
	static VS_DATA<V> MakeData(int DivNum = 24)		//データ作成
	{
		//前処理
		namespace dx = DirectX;
		if (DivNum < 3)
			throw ERROR_EX2("分割数は3以上にしてください。");

		//変数宣言
		const dx::XMVECTOR vBase = dx::XMVectorSet(0.0f, -0.5f, 0.5f, 0.0f);	//基準用頂点ベクトル
		const dx::XMVECTOR vOffset = dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);	//角柱の高さ
		const float fDivAngle = 2.0f * fPI / DivNum;							//分割用回転角(ラジアン)
		std::vector<V> aData;

		//底面（下）の中心頂点作成
		aData.emplace_back();
		aData.back().m_Pos = { 0.0f, -0.5f, 0.0f };
		const UINT usDownCtrIdx = static_cast<UINT>(aData.size() - 1);	//インデックス参照

		//底面（上）の中心頂点作成
		aData.emplace_back();
		aData.back().m_Pos = { 0.0f, 0.5f, 0.0f };
		const UINT usUpCtrIdx = static_cast<UINT>(aData.size() - 1);

		//底面頂点作成
		for (int i = 0; i < DivNum; i++) {

			//底面（下）
			{
				aData.emplace_back();
				dx::XMVECTOR vVertex = dx::XMVector3Transform(vBase, dx::XMMatrixRotationY(fDivAngle * i));		//Y軸回転
				dx::XMStoreFloat3(&aData.back().m_Pos, vVertex);
			}

			//底面（上）
			{
				aData.emplace_back();
				dx::XMVECTOR vVertex = dx::XMVector3Transform(vBase, dx::XMMatrixRotationY(fDivAngle * i));		//Y軸回転
				vVertex = dx::XMVectorAdd(vVertex, vOffset);													//オフセット追加
				dx::XMStoreFloat3(&aData.back().m_Pos, vVertex);
			}
		}

		//側面のインデックスリスト作成
		std::vector<UINT> Indices;
		const UINT Mod = static_cast<UINT>(DivNum * 2);
		for (int i = 0; i < DivNum; i++) {
			const UINT Idx = i * 2;
			Indices.emplace_back(Idx + 2u);
			Indices.emplace_back((Idx + 2u) % Mod + 2u);
			Indices.emplace_back(Idx + 3u);
			Indices.emplace_back((Idx + 2u) % Mod + 2u);
			Indices.emplace_back((Idx + 3u) % Mod + 2u);
			Indices.emplace_back(Idx + 3u);
		}

		//底面のインデックスリスト作成
		for (int i = 0; i < DivNum; i++) {
			const UINT Idx = i * 2;
			Indices.emplace_back(Idx + 2u);
			Indices.emplace_back(usDownCtrIdx);
			Indices.emplace_back((Idx + 2u) % Mod + 2u);	//底面（下）
			Indices.emplace_back(usUpCtrIdx);
			Indices.emplace_back(Idx + 3u);
			Indices.emplace_back((Idx + 3u) % Mod + 2u);	//底面（上）
		}

		return VS_DATA<V>(std::move(aData), std::move(Indices));
	}

	template<class V>
	static VS_DATA<V> MakeData_Model(int DivNum = 24)	//データ作成（モデル用）
	{
		VS_DATA<V> vsd = MakeData<V>(DivNum);
		vsd.ResetDataForModel();
		vsd.SetVertexNormal();
		return vsd;
	}

private:

	//プロトタイプ宣言
	explicit VSD_PRISM() noexcept {}

	~VSD_PRISM() noexcept {}
};
