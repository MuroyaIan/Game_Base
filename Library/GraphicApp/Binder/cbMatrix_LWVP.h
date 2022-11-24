/**
 * @file cbMatrix_LWVP.h
 * @brief 変換行列の処理
 * @author 室谷イアン
 * @date 2022/06/25
 * @履歴 2022/06/25：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Binder/ConstantBuffer.h>
#include <GraphicApp/Drawer/Drawer.h>

//===== クラス定義 =====

//***** 変換行列バッファ *****
class CB_MTX_LWVP : public BINDER
{
public:

	//プロトタイプ宣言
	CB_MTX_LWVP(GRAPHIC& Gfx, const DRAWER& Parent, const DirectX::XMFLOAT4X4& mtxL);
	~CB_MTX_LWVP() noexcept override;
	void Bind(const GRAPHIC& Gfx) noexcept override;							//バインド処理

private:

	//変数宣言
	static std::unique_ptr<VERTEX_CBUFFER<CBD_MTX_LWVP>> m_pVcBuff;	//定数バッファのポインタ
	static int m_RefCount;												//定数バッファの利用数
	const DRAWER& m_Parent;												//所属するドロワー（has-a）
	const DirectX::XMFLOAT4X4& m_mtxL;									//ローカル行列の提供先
};
