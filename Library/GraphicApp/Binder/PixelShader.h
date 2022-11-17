/**
 * @file PixelShader.h
 * @brief ピクセルシェーダの処理
 * @author 室谷イアン
 * @date 2022/06/25
 * @履歴 2022/06/25：ファイル作成
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
	PIXEL_SHADER(GRAPHIC& Gfx, const std::wstring& Path);
	~PIXEL_SHADER() noexcept override;
	void Bind(GRAPHIC& Gfx) noexcept override;					//バインド処理

protected:

	//変数宣言
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;	//ポインタ
};
