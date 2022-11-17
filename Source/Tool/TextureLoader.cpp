
//===== インクルード部 =====
#include <Tool/TextureLoader.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_WINDOWS_UTF8
#include <3rd_Party/stb_image.h>	//support : jpg, png, tga, bmp, psd, gif, hdr, pic, ppm-binary

//===== クラス実装 =====

//テクスチャ読込
TEX_LOADER::TEX_DATA TEX_LOADER::LoadTexture(const char* FilePath)
{
	//ファイル読込
	TEX_DATA Data;
	int nNumComponents{};
	Data.pImageData = stbi_load(FilePath, &Data.nWidth, &Data.nHeight, &nNumComponents, STBI_rgb_alpha);

	//読込エラー
	if (Data.pImageData == nullptr) {
		std::ostringstream oss;
		oss << "画像ファイル読込失敗 : " << FilePath;
		throw ERROR_EX2(oss.str().c_str());
	}

	return Data;
}

TEX_LOADER::TEX_DATA TEX_LOADER::LoadTexture(const wchar_t* FilePath)
{
	//マルチバイト文字変換
	char strFilePath[MAX_PATH]{};
	stbi_convert_wchar_to_utf8(strFilePath, _countof(strFilePath), FilePath);

	return LoadTexture(strFilePath);
}

//テクスチャ解放
void TEX_LOADER::ReleaseTexture(unsigned char* pImageData) noexcept
{
	//メモリ解放
	stbi_image_free(pImageData);
}
