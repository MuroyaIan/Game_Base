/**
 * @file PixelShader.h
 * @brief ピクセルシェーダの処理
 * @author 室谷イアン
 * @date 2022/06/25
 * @履歴 2022/06/25：ファイル作成
 *		 2022/11/24：処理改善
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Binder/Binder.h>

//===== クラス定義 =====

//***** ピクセルシェーダ *****
class PIXEL_SHADER : public BINDER
{
public:

	//プロトタイプ宣言
	explicit PIXEL_SHADER(const CT_GRAPHIC& Gfx, const std::wstring& Path);
	~PIXEL_SHADER() noexcept override;
	void Bind(const CT_GRAPHIC& Gfx) const override;	//バインド処理

private:

	//変数宣言
	ComPtr<ID3D11PixelShader> m_pPixelShader;	//ポインタ
};
