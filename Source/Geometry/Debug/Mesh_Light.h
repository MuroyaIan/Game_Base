/**
 * @file Mesh_Light.h
 * @brief ライト用メッシュ
 * @author 室谷イアン
 * @date 2022/08/12
 * @履歴 2022/08/12：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Drawer/Drawer.h>
#include <App.h>

//===== クラス定義 =====

//***** ライト用メッシュ *****
class MESH_LIGHT : public DRAWER
{
public:

	//プロトタイプ宣言
	explicit MESH_LIGHT(GFX_PACK& Gfx, CAMERA_MGR& CameraMgr);
	~MESH_LIGHT() noexcept override;
	void Update() noexcept override;													//更新処理
	void Draw(int InstanceNum = 0) const noexcept override;								//書込み処理
	int AddInstance() override;															//インスタンス追加

	UINT GetPolygonNum() const noexcept override										//ポリゴン数取得
	{
		return GetIndexNum() / 3 * m_InstanceNum;
	}

	DirectX::XMFLOAT4X4 GetWorldMatrix(int InstanceIndex = 0) const noexcept override	//変形行列取得
	{
		return m_aMtxWorld[InstanceIndex];
	}

	void SetPos(DirectX::XMFLOAT3 Pos, int InstanceIndex = 0) noexcept					//座標設定
	{
		m_aMtxData[InstanceIndex].m_Pos = Pos;
	}

	void SetScale(DirectX::XMFLOAT3 Scale, int InstanceIndex = 0) noexcept				//サイズ設定
	{
		m_aMtxData[InstanceIndex].m_Scale = Scale;
	}

private:

	//変数宣言
	GFX_PACK& m_Gfx;								//描画データ参照
	CAMERA_MGR& m_CameraMgr;						//カメラマネージャ参照
	int m_InstanceNum;								//インスタンス数
	std::vector<DirectX::XMFLOAT4X4> m_aMtxWorld;	//ワールド行列

	struct MTX_DATA
	{
		DirectX::XMFLOAT3 m_Pos;	//ワールド座標
		DirectX::XMFLOAT3 m_Scale;	//サイズ

		explicit MTX_DATA() :
			m_Pos(0.0f, 0.0f, 0.0f), m_Scale(1.0f, 1.0f, 1.0f)
		{}
	};
	std::vector<MTX_DATA> m_aMtxData;				//行列計算用情報
};
