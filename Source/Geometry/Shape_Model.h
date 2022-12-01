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
#include <GraphicApp/Drawer/DrawerRef.h>
#include <GraphicApp/Drawer/vsdRef.h>
#include <App.h>

//===== クラス定義 =====

//***** ジオメトリ描画（モデル表示） *****
class SHAPE_MODEL : public DRAWER_EX<SHAPE_MODEL>
{
public:

	//プロトタイプ宣言
	SHAPE_MODEL(GFX_PACK& Gfx, VSD_MAKER::SHAPE Type);
	~SHAPE_MODEL() noexcept override;
	void Update() noexcept override;															//更新処理
	void Draw(int InstanceNum = -1) const noexcept override;		//書込み処理
	int AddInstance() override;																	//インスタンス追加

	DirectX::XMFLOAT4X4 GetWorldMatrix(int InstanceIndex = 0) const noexcept override			//変形行列取得
	{
		return m_aInstanceData[InstanceIndex].MtxWorld;
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
	std::vector<INSTANCE_DATA> m_aInstanceData;		//インスタンス情報
	MATERIAL_DATA m_Material;						//マテリアル情報

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
