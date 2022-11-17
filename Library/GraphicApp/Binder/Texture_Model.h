/**
 * @file Texture_Model.h
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
class TEXTURE_MODEL : public BINDER
{
public:

	//テクスチャタイプ
	enum class TEX_TYPE
	{
		DIFFUSE,
		SPECULAR,
		NORMAL,

		MAX_TYPE
	};

	//プロトタイプ宣言
	TEXTURE_MODEL(GRAPHIC& Gfx, std::vector<TEX_LOADER::TEX_DATA>& aData);
	~TEXTURE_MODEL() noexcept override;
	void Bind(GRAPHIC& Gfx) noexcept override;	//バインド処理

protected:

	using com_pSRV = Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>;

	//変数宣言
	std::vector<com_pSRV> m_aTextureViewPtr;	//ポインタ配列

	//プロトタイプ宣言
	void MakeBuffer(GRAPHIC& Gfx, TEX_LOADER::TEX_DATA& Data, TEX_TYPE Type);	//バッファ作成
};
