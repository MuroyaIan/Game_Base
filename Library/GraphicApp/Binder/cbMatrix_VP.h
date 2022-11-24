/**
 * @file cbMatrix_VP.h
 * @brief 変換行列の処理
 * @author 室谷イアン
 * @date 2022/08/11
 * @履歴 2022/08/11：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Binder/ConstantBuffer.h>

//===== クラス定義 =====

//***** 変換行列バッファ（ビュー行列、投影行列） *****
class CB_MTX_VP : public BINDER
{
public:

	//プロトタイプ宣言
	CB_MTX_VP(GRAPHIC& Gfx);
	~CB_MTX_VP() noexcept override;
	void Bind(const GRAPHIC& Gfx) noexcept override;		//バインド処理

private:

	//変数宣言
	static std::unique_ptr<VERTEX_CBUFFER<CBD_MTX_VP>> m_pVcBuff;	//定数バッファのポインタ
	static int m_RefCount;											//定数バッファの利用数
};
