/**
 * @file cbTransform.h
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
class CB_MTX_T : public BINDER
{
public:

	//プロトタイプ宣言
	CB_MTX_T(GRAPHIC& Gfx, const DRAWER& Parent);
	~CB_MTX_T() noexcept override;
	void Bind(const GRAPHIC& Gfx) const noexcept override;					//バインド処理

private:

	//変数宣言
	static std::unique_ptr<VERTEX_CBUFFER<DirectX::XMFLOAT4X4>> m_pVcBuff;	//定数バッファのポインタ
	static int m_RefCount;													//定数バッファの利用数
	const DRAWER& m_Parent;													//所属するドロワー（has-a）
};
