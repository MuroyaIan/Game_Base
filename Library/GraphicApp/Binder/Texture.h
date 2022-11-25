/**
 * @file Texture.h
 * @brief テクスチャの処理
 * @author 室谷イアン
 * @date 2022/07/16
 * @履歴 2022/07/16：ファイル作成
 *		 2022/11/24：処理改善
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
	explicit TEXTURE(const GRAPHIC& Gfx, const TEX_LOADER::TEX_DATA& Data, UINT StartSlot = 0u);
	~TEXTURE() noexcept override;
	void Bind(const GRAPHIC& Gfx) const noexcept override;				//バインド処理

protected:

	//変数宣言
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTextureView;	//ポインタ
	UINT m_StartSlot;													//レジスタ番号
};
