/**
 * @file Normal.h
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
class FBX_LOADER;

//===== クラス定義 =====

//***** モデル描画 *****
class NORMAL : public DRAWER
{
public:

	//プロトタイプ宣言
	explicit NORMAL(GRAPHIC& Gfx, SHADER_MGR& ShaderMgr, FBX_LOADER& Loader, int MeshIndex, DRAWER& ModelIn);
	~NORMAL() noexcept override;
	void Update() noexcept override;													//更新処理
	void Draw(int InstanceNum = 0) const noexcept override;								//書込み処理

	DirectX::XMFLOAT4X4 GetWorldMatrix(int InstanceIndex = 0) const noexcept override	//変形行列取得
	{
		(void)InstanceIndex;
		return m_mtxWorld;
	}

	UINT GetPolygonNum() const noexcept override										//ポリゴン数取得
	{
		return GetIndexNum() / 3;
	}

private:

	//変数宣言
	SHADER_MGR& m_ShaderMgr;				//シェーダMgr参照
	FBX_LOADER& m_Loader;					//ローダ参照
	int m_MeshIndex;						//メッシュ番号
	DirectX::XMFLOAT4X4 m_mtxWorld;			//ワールド行列

	DRAWER& m_Model;						//モデル参照

	//プロトタイプ宣言
	VS_DATA<VERTEX_C> MakeData_VS() const;	//頂点情報作成
};
