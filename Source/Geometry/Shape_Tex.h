/**
 * @file Shape_Tex.h
 * @brief ジオメトリ描画（ポリゴン表示）
 * @author 室谷イアン
 * @date 2022/08/13
 * @履歴 2022/08/13：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Drawer/DrawerRef.h>
#include <GraphicApp/Drawer/vsdRef.h>
#include <App.h>

//===== クラス定義 =====

//***** ジオメトリ描画（テクスチャあり） *****
class SHAPE_TEX : public DRAWER_EX<SHAPE_TEX>
{
public:

	//プロトタイプ宣言
	SHAPE_TEX(GFX_PACK& Gfx, VSD_MAKER::SHAPE Type, TEXTURE_MGR::TEX_ID Tex);
	~SHAPE_TEX() noexcept override;
	void Update() noexcept override;															//更新処理
	void Draw(GRAPHIC& Gfx, bool bDrawInstance = false) const noexcept(!IS_DEBUG) override;		//書込み処理
	int AddInstance() override;																	//インスタンス追加

	DirectX::XMFLOAT4X4 GetTransformMtx(int InstanceIndex = 0) const noexcept override			//変形行列取得
	{
		return m_aMtxWorld[InstanceIndex];
	}

	UINT GetPolygonNum() const noexcept override												//ポリゴン数取得
	{
		return GetIndexNum() / 3 * m_InstanceNum;
	}

private:

	//変数宣言
	GFX_PACK& m_Gfx;								//描画データ参照
	VSD_MAKER::SHAPE m_Type;						//形状タイプ
	int m_InstanceNum;								//インスタンス数
	std::vector<DirectX::XMFLOAT4X4> m_aMtxWorld;	//ワールド行列
	TEXTURE_MGR::TEX_ID m_Tex;						//テクスチャ情報

	struct MTX_DATA
	{
		float m_r, dt;
		float m_roll, m_pitch, m_yaw;
		float m_theta, m_phi, m_chi;				//位置情報

		float m_droll, m_dpitch, m_dyaw;
		float m_dtheta, m_dphi, m_dchi;				//回転速度(デルタ/sec)
	};
	std::vector<MTX_DATA> m_aMtxData;				//行列計算用情報
};
