/**
 * @file Sampler.h
 * @brief サンプラーの処理
 * @author 室谷イアン
 * @date 2022/07/16
 * @履歴 2022/07/16：ファイル作成
 *		 2022/11/24：処理改善
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
	explicit SAMPLER(const CT_GRAPHIC& Gfx, UINT StartSlot = 0u, bool IsPixel = false);
	~SAMPLER() noexcept override;
	void Bind(const CT_GRAPHIC& Gfx) const override;	//バインド処理

private:

	//変数宣言
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSampler;	//ポインタ
	UINT m_StartSlot;										//レジスタ番号
};
