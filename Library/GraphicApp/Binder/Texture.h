/**
 * @file Texture.h
 * @brief テクスチャの処理
 * @author 室谷イアン
 * @date 2022/07/16
 * @履歴 2022/07/16：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Binder/Binder.h>
#include <Tool/TextureLoader.h>

//===== クラス定義 =====

//***** テクスチャ *****
class TEXTURE : public BINDER
{
public:

	//プロトタイプ宣言
	TEXTURE(GRAPHIC& Gfx, TEX_LOADER::TEX_DATA& Data);
	~TEXTURE() noexcept override;
	void Bind(GRAPHIC& Gfx) noexcept override;	//バインド処理

protected:

	//変数宣言
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTextureView;	//ポインタ
};
