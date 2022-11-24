/**
 * @file Viewer_Model.h
 * @brief モデル描画
 * @author 室谷イアン
 * @date 2022/07/19
 * @履歴 2022/07/19：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Drawer/DrawerRef.h>
#include <Geometry/ShaderMgr.h>
#include <GraphicApp/Drawer/vsdRef.h>

//===== 前方宣言 =====
class VIEWER;
class FBX_LOADER;
struct CBD_MTX_BONE;
class INPUT_MGR;

//===== クラス定義 =====

//***** モデル描画 *****
class VIEWER_MODEL : public DRAWER_EX<VIEWER_MODEL>
{
public:

	//プロトタイプ宣言
	VIEWER_MODEL(GRAPHIC& Gfx, SHADER_MGR& ShaderMgr, VIEWER& Viewer, FBX_LOADER& Loader, int MeshIndex, INPUT_MGR& Input);
	~VIEWER_MODEL() noexcept override;
	void Update() noexcept override;															//更新処理
	void Draw(GRAPHIC& Gfx, bool bDrawInstance = false) const noexcept(!IS_DEBUG) override;		//書込み処理

	DirectX::XMFLOAT4X4 GetTransformMtx(int InstanceIndex = 0) const noexcept override			//変形行列取得
	{
		(void)InstanceIndex;
		return m_MtxWorld;
	}

	UINT GetPolygonNum() const noexcept override												//ポリゴン数取得
	{
		return GetIndexNum() / 3;
	}

private:

	//変数宣言
	SHADER_MGR& m_ShaderMgr;					//シェーダMgr参照
	VIEWER& m_Viewer;							//ビューワ参照
	FBX_LOADER& m_Loader;						//ローダ参照
	int m_MeshIndex;							//メッシュ番号
	DirectX::XMFLOAT4X4 m_MtxLocal;				//ローカル行列
	DirectX::XMFLOAT4X4 m_MtxWorld;				//ワールド行列
	MATERIAL_DATA m_Material;					//マテリアル情報
	bool m_bNoBone;								//骨なしメッシュ

	std::unique_ptr<CBD_MTX_BONE> m_pMtxBone;	//骨情報
	bool& m_bDrawAnimation;						//アニメーション再生
	int& m_AnimationID;							//アニメーション番号
	int m_AnimFrame;							//再生するフレーム数
	int m_FrameCnt;								//再生フレーム計算用
	bool& m_AnimPause;							//アニメーション一時停止

	INPUT_MGR& m_Input;							//入力参照
	float& m_Scale;								//モデルスケール
	DirectX::XMFLOAT3 m_Rot;					//回転

	//プロトタイプ宣言
	VS_DATA<VERTEX_MB> MakeData_VS() const noexcept;	//頂点情報作成
	void UpdateBoneData(int AnimID = 0) noexcept;		//骨情報更新
};
