/**
 * @file TextureLoader.h
 * @brief テクスチャ読込
 * @author 室谷イアン
 * @date 2022/07/15
 * @履歴 2022/07/15：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <WinApp/WinWindow.h>

//===== クラス宣言 =====

//***** テクスチャ読込 *****
class TEX_LOADER
{
public:

	//テクスチャ情報
	struct TEX_DATA
	{
		unsigned char* pImageData;	//データポインタ
		int nWidth;					//画像幅
		int nHeight;				//画像高さ

		explicit TEX_DATA() noexcept : pImageData(nullptr), nWidth(0), nHeight(0)
		{}
		~TEX_DATA() noexcept {}
	};

	//プロトタイプ宣言
	static TEX_DATA LoadTexture(const char* FilePath);
	static TEX_DATA LoadTexture(const wchar_t* FilePath);			//テクスチャ読込
	static void ReleaseTexture(unsigned char* pImageData) noexcept;	//テクスチャ解放

private:

	//プロトタイプ宣言
	explicit TEX_LOADER() noexcept {}
	~TEX_LOADER() noexcept {}
};
