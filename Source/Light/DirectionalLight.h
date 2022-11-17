/**
 * @file DirectionalLight.h
 * @brief 平行光源
 * @author 室谷イアン
 * @date 2022/08/20
 * @履歴 2022/08/20：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <Light/LightMgr.h>

//===== クラス定義 =====

//***** 平行光源 *****
class DIRECTIONAL_LIGHT : public LIGHT
{
public:

	//プロトタイプ宣言
	DIRECTIONAL_LIGHT(APP_64& App) noexcept;
	~DIRECTIONAL_LIGHT() noexcept override;
	void Update() const noexcept override;		//更新処理

	LIGHT_DIRECTIONAL& GetData() noexcept		//ライト情報参照
	{
		return m_LightData;
	}

private:

	//変数宣言
	LIGHT_MGR& m_LightMgr;						//ライトマネージャ参照
	LIGHT_DIRECTIONAL m_LightData;				//ライト情報
};
