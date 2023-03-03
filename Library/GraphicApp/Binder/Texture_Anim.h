/**
 * @file Texture_Anim.h
 * @brief テクスチャ（アニメーション情報）
 * @author 室谷イアン
 * @date 2023/02/20
 * @履歴 2023/02/20：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Binder/Binder.h>
#include <GraphicApp/Drawer/VertexShaderData.h>

//===== 構造体宣言 =====
struct TEX_DATA_ANIM
{
	float* pAnimData;	//データポインタ
	int nWidth;			//テクスチャ幅（全アニメーションのフレーム数の合計）
	int nHeight;		//テクスチャ高さ（最大骨数）

	explicit TEX_DATA_ANIM() noexcept : pAnimData(nullptr), nWidth(0), nHeight(MAX_BONE)
	{}
	~TEX_DATA_ANIM() noexcept {}
};

//===== クラス定義 =====
class TEXTURE_ANIM : public BINDER
{
public:

	//プロトタイプ宣言
	explicit TEXTURE_ANIM(const GRAPHIC& Gfx, const TEX_DATA_ANIM& Data, UINT StartSlot = 0u);
	~TEXTURE_ANIM() noexcept override;
	void Bind(const GRAPHIC& Gfx) const noexcept override;				//バインド処理

	ID3D11ShaderResourceView* GetSrvPtr() const							//ポインタ参照
	{
		//例外処理
		if (m_pTextureView == nullptr)
			throw ERROR_EX2("ポインタはNULLです。");

		return m_pTextureView.Get();
	}

private:

	//変数宣言
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTextureView;	//ポインタ
	UINT m_StartSlot;													//レジスタ番号
};
