/**
 * @file InputLayout.h
 * @brief 入力レイアウトの処理
 * @author 室谷イアン
 * @date 2022/06/25
 * @履歴 2022/06/25：ファイル作成
 *		 2022/11/21：処理改善
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Binder/Binder.h>

//===== クラス定義 =====

//***** 入力レイアウト *****
class INPUT_LAYOUT : public BINDER
{
public:

	//プロトタイプ宣言
	explicit INPUT_LAYOUT(GRAPHIC& Gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& aLayout, ID3DBlob* pCodeVS);
	~INPUT_LAYOUT() noexcept override;
	void Bind(GRAPHIC& Gfx) noexcept override;					//バインド処理

protected:

	//変数宣言
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;	//ポインタ
};
