/**
 * @file PointLight.h
 * @brief 点光源
 * @author 室谷イアン
 * @date 2022/08/13
 * @履歴 2022/08/13：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <Light/Light.h>

//===== 前方宣言 =====
class LIGHT_MGR;

//===== クラス定義 =====

//***** 点光源 *****
class POINT_LIGHT : public LIGHT
{
public:

	//プロトタイプ宣言
	explicit POINT_LIGHT(APP& App, float Range = 100.0f) noexcept;
	~POINT_LIGHT() noexcept override;
	void Update() const noexcept override;			//更新処理
	void ClacAttenuation(float Range) noexcept;		//減衰計算

	LIGHT_POINT& GetData() noexcept					//ライト情報参照
	{
		return m_LightData;
	}

private:

	//変数宣言
	LIGHT_POINT m_LightData;	//ライト情報
	float m_Range;				//ライトが光る範囲
	LIGHT_MGR& m_LightMgr;		//ライトマネージャ参照
};
