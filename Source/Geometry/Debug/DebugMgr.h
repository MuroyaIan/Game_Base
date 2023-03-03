/**
 * @file DebugMgr.h
 * @brief デバッグマネージャ
 * @author 室谷イアン
 * @date 2022/08/12
 * @履歴 2022/08/12：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <Geometry/Debug/Mesh_Light.h>

//===== クラス定義 =====

//***** デバッグマネージャ *****
class DEBUG_MGR
{
public:

	//プロトタイプ宣言
	explicit DEBUG_MGR(APP& App) noexcept;
	~DEBUG_MGR() noexcept;
	void Update();							//更新処理
	void Draw() noexcept;					//描画処理
	UINT GetPolygonNum() const noexcept;	//ポリゴン数取得

	MESH_LIGHT& GetLightMesh() noexcept		//ライトメッシュ取得
	{
		return m_Light;
	}

private:

	//変数宣言
	MESH_LIGHT m_Light;		//ライトアイコン用メッシュ
};
