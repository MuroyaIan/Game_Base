/**
 * @file cbBone.h
 * @brief 骨情報バッファ
 * @author 室谷イアン
 * @date 2022/09/28
 * @履歴 2022/09/28：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Binder/ConstantBuffer.h>

//===== クラス定義 =====

//***** 骨情報バッファ *****
class CB_BONE : public BINDER
{
public:

	//プロトタイプ宣言
	CB_BONE(GRAPHIC& Gfx, const CB_MTX_BONE& aMtxBone, bool Transpose = false);
	~CB_BONE() noexcept override;
	void Bind(GRAPHIC& Gfx) noexcept override;						//バインド処理

private:

	//変数宣言
	static std::unique_ptr<VERTEX_CBUFFER<CB_MTX_BONE>> m_pVcBuff;	//定数バッファのポインタ
	const CB_MTX_BONE& m_aMtxBone;									//骨情報の提供先
	bool m_bTransposeMatrix;										//転置処理実行フラグ
};
