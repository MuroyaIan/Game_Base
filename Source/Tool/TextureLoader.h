/**
 * @file TextureLoader.h
 * @brief �e�N�X�`���Ǎ�
 * @author ���J�C�A��
 * @date 2022/07/15
 * @���� 2022/07/15�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <WinApp/WinWindow.h>

//===== �N���X�錾 =====

//***** �e�N�X�`���Ǎ� *****
class TEX_LOADER
{
public:

	//�e�N�X�`�����
	struct TEX_DATA
	{
		unsigned char* pImageData;	//�f�[�^�|�C���^
		int nWidth;					//�摜��
		int nHeight;				//�摜����

		explicit TEX_DATA() noexcept : pImageData(nullptr), nWidth(0), nHeight(0)
		{}
		~TEX_DATA() noexcept {}
	};

	//�v���g�^�C�v�錾
	static TEX_DATA LoadTexture(const char* FilePath);
	static TEX_DATA LoadTexture(const wchar_t* FilePath);			//�e�N�X�`���Ǎ�
	static void ReleaseTexture(unsigned char* pImageData) noexcept;	//�e�N�X�`�����

private:

	//�v���g�^�C�v�錾
	explicit TEX_LOADER() noexcept {}
	~TEX_LOADER() noexcept {}
};
