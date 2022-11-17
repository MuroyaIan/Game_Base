/**
 * @file Grid.h
 * @brief グリッド描画
 * @author 室谷イアン
 * @date 2022/07/19
 * @履歴 2022/07/19：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Drawer/DrawerRef.h>
#include <Geometry/ShaderMgr.h>

//===== クラス定義 =====

//***** グリッド描画 *****
class GRID : public DRAWER_EX<GRID>
{
public:

	//プロトタイプ宣言
	GRID(GRAPHIC& Gfx, SHADER_MGR& ShaderMgr);
	~GRID() noexcept override;
	void Update() noexcept override;															//更新処理
	void Draw(GRAPHIC& Gfx, bool bDrawInstance = false) const noexcept(!IS_DEBUG) override;		//書込み処理

	DirectX::XMFLOAT4X4 GetTransformMtx(int InstanceIndex = 0) const noexcept override			//変形行列取得
	{
		(void)InstanceIndex;
		return m_mtxWorld;
	}

	UINT GetPolygonNum() const noexcept override												//ポリゴン数取得
	{
		return GetIndexNum() / 2;
	}

private:

	//変数宣言
	SHADER_MGR& m_ShaderMgr;			//シェーダMgr参照
	DirectX::XMFLOAT4X4 m_mtxWorld;		//ワールド行列
	DirectX::XMFLOAT2 m_Size;			//サイズ（XY）
};
