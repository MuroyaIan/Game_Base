/**
 * @file cbMatrix_LWVP.h
 * @brief 変換行列の処理
 * @author 室谷イアン
 * @date 2022/06/25
 * @履歴 2022/06/25：ファイル作成
 *		 2022/11/25：処理改善
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Binder/ConstantBuffer.h>

//===== 前方宣言 =====
class DRAWER;

//===== クラス定義 =====

//***** 変換行列バッファ *****
class CB_MTX_LWVP : public BINDER
{
public:

	//プロトタイプ宣言
	explicit CB_MTX_LWVP(const CT_GRAPHIC& Gfx, CB_PTR* cbPtr, const DRAWER& Parent, const DirectX::XMFLOAT4X4& mtxL);
	~CB_MTX_LWVP() noexcept override;
	void Bind(const CT_GRAPHIC& Gfx) const override;	//バインド処理(データ更新)

	void SetBuffPtr(CB_PTR* cbPtr) const noexcept	//バッファポインタ設定
	{
		m_pCBuff->SetBuffPtr(cbPtr);
	}

private:

	//変数宣言
	static std::unique_ptr<CONSTANT_BUFFER<CBD_MTX_LWVP>> m_pCBuff;		//定数バッファのポインタ
	static int m_RefCount;												//定数バッファの利用数
	const DRAWER& m_Parent;												//所属するドロワー（has-a）
	const DirectX::XMFLOAT4X4& m_mtxL;									//ローカル行列の提供先
};
