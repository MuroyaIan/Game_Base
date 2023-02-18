/**
 * @file Mesh.h
 * @brief メッシュ描画
 * @author 室谷イアン
 * @date 2022/09/08
 * @履歴 2022/09/08：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Drawer/Drawer.h>
#include <Geometry/Model/Model.h>

//===== 前方宣言 =====
struct CBD_MTX_LOCAL;

//===== クラス定義 =====

//***** メッシュ描画 *****
class MESH : public DRAWER
{
public:

	//プロトタイプ宣言
	explicit MESH(MODEL& ModelRef, int MeshIdx);
	~MESH() noexcept override;
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
	int& m_InstanceNum;								//インスタンス数参照
	std::vector<VSD_INSTANCE>& m_aInstanceData;		//インスタンス情報参照
	CBD_MATERIAL m_Material;						//マテリアル情報

	ModelRef::MODEL_PACK& m_FileData;				//ファイル情報
	int m_MeshIdx;									//メッシュ番号

	bool& m_bStatic;								//静的メッシュかどうか
	std::unique_ptr<CBD_MTX_LOCAL> m_pLocalData;	//ローカル情報
	int& m_AnimID;									//アニメーション番号
	int& m_AnimFrame;								//アニメーション再生フレーム
};
