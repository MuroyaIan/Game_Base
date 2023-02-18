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
#include <GraphicApp/Drawer/Drawer.h>
#include <App.h>
#include <GraphicApp/Drawer/vsdRef.h>
#include <GraphicApp/Binder/cbdRef.h>

//===== 前方宣言 =====
class VIEWER;
class FBX_LOADER;
struct CBD_BONE;

template<typename C>
class CONSTANT_BUFFER;

//===== クラス定義 =====

//***** モデル描画 *****
class VIEWER_MODEL : public DRAWER
{
public:

	//プロトタイプ宣言
	explicit VIEWER_MODEL(GFX_PACK& Gfx, VIEWER& Viewer, FBX_LOADER& Loader, int MeshIndex);
	~VIEWER_MODEL() noexcept override;
	void Update() noexcept override;													//更新処理
	void Draw(int InstanceNum = 0) const noexcept override;								//書込み処理

	DirectX::XMFLOAT4X4 GetWorldMatrix(int InstanceIndex = 0) const noexcept override	//変形行列取得
	{
		(void)InstanceIndex;
		return m_mtxWorld;
	}

	void SetWorldMatrix(DirectX::XMFLOAT4X4 mtxW, int InstanceIndex = 0) noexcept override	//変形行列設定
	{
		(void)InstanceIndex;
		m_mtxWorld = mtxW;
	}

	UINT GetPolygonNum() const noexcept override										//ポリゴン数取得
	{
		return GetIndexNum() / 3;
	}

private:

	//変数宣言
	GRAPHIC& m_Gfx;							//グラフィックAPI参照
	SHADER_MGR& m_ShaderMgr;				//シェーダMgr参照
	VIEWER& m_Viewer;						//ビューワ参照
	FBX_LOADER& m_Loader;					//ローダ参照
	int m_MeshIndex;						//メッシュ番号
	DirectX::XMFLOAT4X4 m_mtxLocal;			//ローカル行列
	DirectX::XMFLOAT4X4 m_mtxWorld;			//ワールド行列
	CBD_MATERIAL m_Material;				//マテリアル情報
	bool m_bNoBone;							//骨なしメッシュ

	std::unique_ptr<CBD_BONE> m_pMtxBone;	//骨情報
	bool& m_bDrawAnimation;					//アニメーション再生
	int& m_AnimationID;						//アニメーション番号
	int m_AnimFrame;						//再生するフレーム数
	int m_FrameCnt;							//再生フレーム計算用
	bool& m_AnimPause;						//アニメーション一時停止

	float& m_Scale;							//モデルスケール
	float& m_RotY;							//回転(Y軸)

	DirectX::XMFLOAT3& m_LightPos;			//ライト座標

	bool bUseNormalMap;						//ノーマルマップ使用
	bool bUseDispMap;						//視差マップ使用

	//プロトタイプ宣言
	VS_DATA<VERTEX_MNB> MakeData_VS() const noexcept;	//頂点情報作成
	void UpdateBoneData(int AnimID = 0) noexcept;		//骨情報更新
};
