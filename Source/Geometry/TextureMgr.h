/**
 * @file TextureMgr.h
 * @brief �e�N�X�`���Ǘ�
 * @author ���J�C�A��
 * @date 2022/08/12
 * @���� 2022/08/12�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <Tool/TextureLoader.h>
#include <GraphicApp/Graphic.h>

//===== �O���錾 =====
class BINDER;

//===== �N���X��` =====

//***** �e�N�X�`���Ǘ� *****
class TEXTURE_MGR
{
public:

	//�e�N�X�`�����ʔԍ�
	enum class TEX_ID
	{
		TEX_Null,
		TEX_TestPlane,
		TEX_TestBox,

		ID_Max
	};

	//�e�N�X�`���p�b�N
	struct TEX_PACK
	{
		TEX_LOADER::TEX_DATA TexData;		//�e�N�X�`�����
		std::unique_ptr<BINDER> pBinder;	//�o�C���_�̃|�C���^
		int nUsedCount;						//�g�p��

		TEX_PACK() noexcept;
		~TEX_PACK() noexcept;
	};

	//�v���g�^�C�v�錾
	explicit TEXTURE_MGR(GRAPHIC& Gfx);
	~TEXTURE_MGR() noexcept;
	void SetTextureOn(TEX_ID id);
	void SetTextureOff(TEX_ID id) noexcept;		//�o�b�t�@���p�J�n�E�I��
	void Bind(TEX_ID id) const;					//�o�C���h����

	TEX_PACK& GetTexPack(TEX_ID id) noexcept	//�e�N�X�`���p�b�N�Q��
	{
		return m_aTexPack[static_cast<int>(id)];
	}

private:

	//�ϐ��錾
	std::vector<TEX_PACK> m_aTexPack;			//�e�N�X�`���p�b�N�z��
	GRAPHIC& m_DX;								//DX�Q��

	static std::string aFilePath[static_cast<int>(TEX_ID::ID_Max)];
};
