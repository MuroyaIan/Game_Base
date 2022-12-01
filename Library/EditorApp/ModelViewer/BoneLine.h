/**
 * @file BoneLine.h
 * @brief モデル描画
 * @author 室谷イアン
 * @date 2022/08/10
 * @履歴 2022/08/10：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Drawer/Drawer.h>
#include <Geometry/ShaderMgr.h>
#include <GraphicApp/Drawer/VertexShaderData.h>

//===== 前方宣言 =====
class VIEWER;
class FBX_LOADER;

//===== クラス定義 =====

//***** モデル描画 *****
class BONE_LINE : public DRAWER
{
public:

	//プロトタイプ宣言
	BONE_LINE(GRAPHIC& Gfx, SHADER_MGR& ShaderMgr, VIEWER& Viewer, FBX_LOADER& Loader, DRAWER& BoneIn);
	~BONE_LINE() noexcept override;
	void Update() noexcept override;															//更新処理
	void Draw(int InstanceNum = -1) const noexcept override;		//書込み処理

	DirectX::XMFLOAT4X4 GetWorldMatrix(int InstanceIndex = 0) const noexcept override			//変形行列取得
	{
		(void)InstanceIndex;
		return m_mtxWorld;
	}

	UINT GetPolygonNum() const noexcept override												//ポリゴン数取得
	{
		return GetIndexNum() / 3;
	}

private:

	//変数宣言
	GRAPHIC& m_Gfx;							//グラフィック参照
	SHADER_MGR& m_ShaderMgr;				//シェーダMgr参照
	FBX_LOADER& m_Loader;					//ローダ参照
	DirectX::XMFLOAT4X4 m_mtxWorld;			//ワールド行列

	DRAWER& m_Bone;							//骨参照
	bool& m_bDrawAnimation;					//アニメーション再生
	int& m_AnimationID;						//アニメーション番号
	int& m_AnimFrame;						//再生するフレーム数

	//プロトタイプ宣言
	VS_DATA<VERTEX> MakeData_VS() const;	//頂点情報作成
};
