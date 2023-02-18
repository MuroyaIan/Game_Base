/**
 * @file Shape_Model.h
 * @brief ジオメトリ描画（モデル表示）
 * @author 室谷イアン
 * @date 2022/06/25
 * @履歴 2022/06/25：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Drawer/Drawer.h>
#include <GraphicApp/Drawer/vsdRef.h>
#include <GraphicApp/Binder/cbdRef.h>
#include <App.h>

//===== クラス定義 =====

//***** ジオメトリ描画（モデル表示） *****
class SHAPE_MODEL : public DRAWER
{
public:

	//プロトタイプ宣言
	explicit SHAPE_MODEL(GFX_PACK& Gfx, VSD_MAKER::SHAPE Type);
	~SHAPE_MODEL() noexcept override;
	void Update() noexcept override;														//更新処理
	void Draw(int InstanceNum = 0) const noexcept override;									//書込み処理
	int AddInstance() override;																//インスタンス追加

	DirectX::XMFLOAT4X4 GetWorldMatrix(int InstanceIndex = 0) const noexcept override		//ワールド行列取得
	{
		return m_aInstanceData[InstanceIndex].MtxWorld;
	}

	void SetWorldMatrix(DirectX::XMFLOAT4X4 mtxW, int InstanceIndex = 0) noexcept override	//ワールド行列設定
	{
		m_aInstanceData[InstanceIndex].MtxWorld = mtxW;
	}

	UINT GetPolygonNum() const noexcept override											//ポリゴン数取得
	{
		return GetIndexNum() / 3 * m_InstanceNum;
	}

private:

	//変数宣言
	GFX_PACK& m_Gfx;								//描画データ参照
	VSD_MAKER::SHAPE m_Type;						//形状タイプ
	int m_InstanceNum;								//インスタンス数
	std::vector<VSD_INSTANCE> m_aInstanceData;		//インスタンス情報
	CBD_MATERIAL m_Material;						//マテリアル情報
};
