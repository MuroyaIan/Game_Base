/**
 * @file Texture_Model.h
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
class TEXTURE_MODEL : public BINDER
{
public:

	//テクスチャタイプ
	enum class TEX_TYPE
	{
		Diffuse,		//拡散色
		Specular,		//反射色
		Normal,			//ノーマルマップ
		Displacement,	//視差マップ

		MaxType
	};

	//プロトタイプ宣言
	explicit TEXTURE_MODEL(const CT_GRAPHIC& Gfx, const std::vector<TEX_LOADER::TEX_DATA>& aData, UINT StartSlot = 0u);
	~TEXTURE_MODEL() noexcept override;
	void Bind(const CT_GRAPHIC& Gfx) const override;	//バインド処理

private:

	using com_pSRV = Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>;

	//変数宣言
	std::vector<com_pSRV> m_pTextureViews;	//ポインタ配列

	//プロトタイプ宣言
	void MakeBuffer(const CT_GRAPHIC& Gfx, const TEX_LOADER::TEX_DATA& Data, TEX_TYPE Type);	//バッファ作成
	UINT m_StartSlot;																		//レジスタ番号
};
