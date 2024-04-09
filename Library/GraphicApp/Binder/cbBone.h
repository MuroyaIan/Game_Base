/**
 * @file cbBone.h
 * @brief 骨情報バッファ
 * @author 室谷イアン
 * @date 2022/09/28
 * @履歴 2022/09/28：ファイル作成
 *		 2022/11/25：処理改善
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
	explicit CB_BONE(const CT_GRAPHIC& Gfx, CB_PTR* cbPtr, const CBD_BONE& aMtxBone, bool Transpose = false);
	~CB_BONE() noexcept override;
	void Bind(const CT_GRAPHIC& Gfx) const override;	//バインド処理(データ更新)

	void SetBuffPtr(CB_PTR* cbPtr) const noexcept	//バッファポインタ設定
	{
		m_pCBuff->SetBuffPtr(cbPtr);
	}

private:

	//変数宣言
	static std::unique_ptr<CONSTANT_BUFFER<CBD_BONE>> m_pCBuff;		//定数バッファのポインタ
	static int m_RefCount;											//定数バッファの利用数
	const CBD_BONE& m_aMtxBone;										//骨情報の提供先
	bool m_bTransposeMatrix;										//転置処理実行フラグ
};
