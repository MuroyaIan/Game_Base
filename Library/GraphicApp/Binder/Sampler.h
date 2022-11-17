/**
 * @file Sampler.h
 * @brief サンプラーの処理
 * @author 室谷イアン
 * @date 2022/07/16
 * @履歴 2022/07/16：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Binder/Binder.h>

//===== クラス定義 =====

//***** サンプラー *****
class SAMPLER : public BINDER
{
public:

	//プロトタイプ宣言
	SAMPLER(GRAPHIC& Gfx);
	~SAMPLER() noexcept override;
	void Bind(GRAPHIC& Gfx) noexcept override;			//バインド処理

protected:

	//変数宣言
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSampler;	//ポインタ
};
