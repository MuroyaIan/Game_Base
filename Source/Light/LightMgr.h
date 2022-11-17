/**
 * @file LightMgr.h
 * @brief ライトマネージャ
 * @author 室谷イアン
 * @date 2022/08/20
 * @履歴 2022/08/20：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <Light/Light.h>

//===== 定数・マクロ定義 =====
constexpr int LIGHT_NUM = 16;		//点光源の最大描画数

//===== 前方宣言 =====
template<typename C>
class PIXEL_CBUFFER;

//===== クラス定義 =====

//***** ライトマネージャ *****
class LIGHT_MGR
{
public:

	//バッファパック
	struct LIGHT_PACK
	{
		LIGHT::LIGHT_DIRECTIONAL DirectionalLight;	//平行光源
		LIGHT::LIGHT_POINT PointLight[LIGHT_NUM];	//点光源
		DirectX::XMFLOAT4 AmbientLight;				//環境光

		LIGHT_PACK() noexcept : DirectionalLight(), PointLight(), AmbientLight(1.0f, 1.0f, 1.0f, 0.2f)
		{}
		~LIGHT_PACK() noexcept
		{}
	};

	//プロトタイプ宣言
	LIGHT_MGR(APP_64& App) noexcept;
	~LIGHT_MGR() noexcept;
	void Draw() noexcept;									//描画処理
	void AddPointLight(LIGHT::LIGHT_POINT Data) noexcept;	//ポイントライト登録

	LIGHT_PACK& GetData() noexcept							//ライト情報参照
	{
		return m_LightData;
	}

	int GetLightNum() noexcept								//ポイントライト使用数取得
	{
		return m_UsedData_backup;
	}

private:

	//変数宣言
	GFX_PACK& m_Gfx;										//描画データ参照
	LIGHT_PACK m_LightData;									//ライト情報
	int m_UsedData;											//ポイントライト使用数
	int m_UsedData_backup;									//外部取得用
	std::unique_ptr<PIXEL_CBUFFER<LIGHT_PACK>> m_pcBuff;	//定数バッファ
};
