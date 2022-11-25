/**
 * @file cbTransform.h
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
class CB_MTX_T : public BINDER
{
public:

	//プロトタイプ宣言
	explicit CB_MTX_T(const GRAPHIC& Gfx, const DRAWER& Parent);
	~CB_MTX_T() noexcept override;
	void Bind(const GRAPHIC& Gfx) const noexcept override;					//バインド処理

private:

	//変数宣言
	static std::unique_ptr<VS_CBUFFER<DirectX::XMFLOAT4X4>> m_pCBuffVS;		//定数バッファのポインタ
	static int m_RefCount;													//定数バッファの利用数
	const DRAWER& m_Parent;													//所属するドロワー（has-a）
};
