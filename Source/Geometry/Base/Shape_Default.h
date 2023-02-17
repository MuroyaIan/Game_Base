/**
 * @file Shape_Default.h
 * @brief ジオメトリ描画（ポリゴン表示）
 * @author 室谷イアン
 * @date 2022/06/25
 * @履歴 2022/06/25：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Drawer/Drawer.h>
#include <GraphicApp/Drawer/vsdRef.h>
#include <App.h>

//===== クラス定義 =====

//***** ジオメトリ描画（ポリゴン表示） *****
class SHAPE_DEFAULT : public DRAWER
{
public:

	//プロトタイプ宣言
	explicit SHAPE_DEFAULT(GFX_PACK& Gfx, VSD_MAKER::SHAPE Type);
	~SHAPE_DEFAULT() noexcept override;
	void Update() noexcept override;														//更新処理
	void Draw(int InstanceNum = 0) const noexcept override;									//書込み処理
	int AddInstance() override;																//インスタンス追加

	DirectX::XMFLOAT4X4 GetWorldMatrix(int InstanceIndex = 0) const noexcept override		//変形行列取得
	{
		return m_aMtxWorld[InstanceIndex];
	}

	void SetWorldMatrix(DirectX::XMFLOAT4X4 mtxW, int InstanceIndex = 0) noexcept override	//変形行列設定
	{
		m_aMtxWorld[InstanceIndex] = mtxW;
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
	std::vector<DirectX::XMFLOAT4X4> m_aMtxWorld;	//ワールド行列
};
