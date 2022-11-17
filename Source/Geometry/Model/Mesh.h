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
#include <GraphicApp/Drawer/DrawerRef.h>
#include <Geometry/Model/Model.h>

//===== 前方宣言 =====
struct CB_MTX_LOCAL;

//===== クラス定義 =====

//***** メッシュ描画 *****
class MESH : public DRAWER_EX<MESH>
{
public:

	//プロトタイプ宣言
	MESH(MODEL& ModelRef, int MeshIdx);
	~MESH() noexcept override;
	void Update() noexcept override;															//更新処理
	void Draw(GRAPHIC& Gfx, bool bDrawInstance = false) const noexcept(!IS_DEBUG) override;		//書込み処理
	int AddInstance() override;																	//インスタンス追加

	DirectX::XMFLOAT4X4 GetTransformMtx(int InstanceIndex = 0) const noexcept override			//変形行列取得
	{
		return m_aInstanceData[InstanceIndex].MtxWorld;
	}

	UINT GetPolygonNum() const noexcept override												//ポリゴン数取得
	{
		return GetIndexNum() / 3 * m_InstanceNum;
	}

private:

	//変数宣言
	ModelRef::MODEL_PACK& m_FileData;				//ファイル情報
	int m_MeshIdx;									//メッシュ番号
	GFX_PACK& m_Gfx;								//描画データ参照
	int& m_InstanceNum;								//インスタンス数参照
	std::vector<INSTANCE_DATA>& m_aInstanceData;	//インスタンス情報参照
	MATERIAL_DATA m_Material;						//マテリアル情報

	bool& m_bStatic;								//静的メッシュかどうか
	std::unique_ptr<CB_MTX_LOCAL> m_pLocalData;		//ローカル情報
	int& m_AnimID;									//アニメーション番号
	int& m_AnimID_Backup;							//アニメーション番号（バックアップ）
	int& m_AnimFrame;								//アニメーション再生フレーム
	int& m_AnimFrame_Backup;						//アニメーション再生フレーム（バックアップ）
	bool& m_bBlendAnim;								//ブレンドモード
	int& m_BlendTimer;								//ブレンド用タイマ
};
