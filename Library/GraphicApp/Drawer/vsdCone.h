/**
 * @file vsdCone.h
 * @brief コーン形状テンプレート
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

//***** コーン形状 *****
class VSD_CONE
{
public:

	//プロトタイプ宣言
	template<class V>
	static VS_DATA<V> MakeTessellation(int DivNum)	//テッセレーション作成
	{
		//前処理
		namespace dx = DirectX;
		if (DivNum < 3)
			throw ERROR_EX2("分割数は3以上にしてください。");

		//変数宣言
		const dx::XMVECTOR vBase = dx::XMVectorSet(0.5f, -0.5f, 0.0f, 0.0f);	//基準用頂点ベクトル
		const float fDivAngle = 2.0f * fPI / DivNum;							//分割用回転角(ラジアン)
		std::vector<V> aData;

		//底部円形上の頂点作成
		for (int i = 0; i < DivNum; i++) {
			aData.emplace_back();
			dx::XMVECTOR vVertex = dx::XMVector3Transform(vBase, dx::XMMatrixRotationY(fDivAngle * i));		//Y軸回転
			dx::XMStoreFloat3(&aData.back().m_Pos, vVertex);
		}

		//底部円形の中心頂点作成
		aData.emplace_back();
		aData.back().m_Pos = { 0.0f, -0.5f, 0.0f };
		const unsigned short usCtrIdx = static_cast<unsigned short>(aData.size() - 1);	//インデックス参照

		//コーンの先端頂点作成
		aData.emplace_back();
		aData.back().m_Pos = { 0.0f, 0.5f, 0.0f };
		const unsigned short usTopIdx = static_cast<unsigned short>(aData.size() - 1);

		//底部円形上のインデックスリスト作成
		std::vector<unsigned short> Indices;
		const unsigned short Mod = static_cast<unsigned short>(DivNum);
		for (unsigned short i = 0; i < DivNum; i++) {
			Indices.push_back(usCtrIdx);
			Indices.push_back((i + 1u) % Mod);
			Indices.push_back(i);
		}

		//その他のインデックスリスト作成
		for (unsigned short i = 0; i < DivNum; i++) {
			Indices.push_back(usTopIdx);
			Indices.push_back(i);
			Indices.push_back((i + 1u) % Mod);
		}

		return VS_DATA<V>(std::move(aData), std::move(Indices));
	}

	template<class V>
	static VS_DATA<V> MakeData(int DivNum = 24)		//データ作成
	{
		return MakeTessellation<V>(DivNum);
	}

	template<class V>
	static VS_DATA<V> MakeData_Model(int DivNum = 24)	//データ作成（モデル用）
	{
		VS_DATA<V> vsd = MakeTessellation<V>(DivNum);
		vsd.ResetDataForModel();
		vsd.SetVertexNormal();
		return vsd;
	}

private:

	//プロトタイプ宣言
	VSD_CONE() noexcept {}
	~VSD_CONE() noexcept {}
};
