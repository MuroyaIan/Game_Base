
//===== �C���N���[�h�� =====
#include <Tool/TextureLoader.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_WINDOWS_UTF8
#include <3rd_Party/stb_image.h>	//support : jpg, png, tga, bmp, psd, gif, hdr, pic, ppm-binary

//===== �N���X���� =====

//�e�N�X�`���Ǎ�
TEX_LOADER::TEX_DATA TEX_LOADER::LoadTexture(const char* FilePath)
{
	//�t�@�C���Ǎ�
	TEX_DATA Data;
	int nNumComponents{};
	Data.pImageData = stbi_load(FilePath, &Data.nWidth, &Data.nHeight, &nNumComponents, STBI_rgb_alpha);

	//�Ǎ��G���[
	if (Data.pImageData == nullptr) {
		std::ostringstream oss;
		oss << "�摜�t�@�C���Ǎ����s : " << FilePath;
		throw ERROR_EX2(oss.str().c_str());
	}

	return Data;
}

TEX_LOADER::TEX_DATA TEX_LOADER::LoadTexture(const wchar_t* FilePath)
{
	//�}���`�o�C�g�����ϊ�
	char strFilePath[MAX_PATH]{};
	stbi_convert_wchar_to_utf8(strFilePath, _countof(strFilePath), FilePath);

	return LoadTexture(strFilePath);
}

//�e�N�X�`�����
void TEX_LOADER::ReleaseTexture(unsigned char* pImageData) noexcept
{
	//���������
	stbi_image_free(pImageData);
}
