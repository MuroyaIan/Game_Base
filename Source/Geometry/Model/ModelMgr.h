/**
 * @file ModelRef.h
 * @brief ���f���}�l�[�W��
 * @author ���J�C�A��
 * @date 2022/09/08
 * @���� 2022/09/08�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <GraphicApp/Graphic.h>
#include <Tool/TextureLoader.h>

//===== �O���錾 =====
namespace ModelRef {
	struct MODEL_PACK;
}
class TEXTURE;
class TEXTURE_ANIM;

//===== �N���X��` =====

//***** ���f�����[�_ *****
class MODEL_MGR
{
public:

	//���f���ԍ�
	enum class MODEL_ID
	{
		Tarantella,
		Unity,
		Cube,

		ID_Max
	};

	//�e�N�X�`���p�b�N
	struct TEX_PACK
	{
		std::vector<std::string> aName;						//�e�N�X�`����
		std::vector<TEX_LOADER::TEX_DATA> aTexData;			//�e�N�X�`�����
		std::vector<std::unique_ptr<TEXTURE>> aTexBuffPtr;	//�e�N�X�`���o�b�t�@�̃|�C���^
		std::unique_ptr<TEXTURE_ANIM> AnimBuffPtr;			//�A�j���[�V�����o�b�t�@�̃|�C���^
		std::vector<int> aUsedCount;						//�g�p��

		TEX_PACK() noexcept;
		~TEX_PACK() noexcept;
	};

	//�v���g�^�C�v�錾
	explicit MODEL_MGR(GRAPHIC& Gfx);
	~MODEL_MGR() noexcept;
	ID3D11ShaderResourceView* SetTextureOn(MODEL_ID id, std::string TexName);
	void SetTextureOff(MODEL_ID id, std::string TexName);						//�e�N�X�`���o�b�t�@���p�J�n�E�I��
	ID3D11ShaderResourceView* SetAnimTexOn(MODEL_ID id);
	void SetAnimTexOff(MODEL_ID id) noexcept;									//�A�j���[�V�����o�b�t�@���p�J�n�E�I��

	ModelRef::MODEL_PACK& GetModelPack(MODEL_ID id) const noexcept				//���f���p�b�N�Q��
	{
		return *m_aModelPackPtr[static_cast<int>(id)];
	}

private:

	//�ϐ��錾
	std::vector<std::unique_ptr<ModelRef::MODEL_PACK>> m_aModelPackPtr;				//���f���p�b�N�z��
	std::vector<TEX_PACK> m_aTexPack;												//�e�N�X�`���p�b�N�z��
	GRAPHIC& m_DX;																	//DX�Q��

	static std::string aModelName[static_cast<int>(MODEL_ID::ID_Max)];				//���f�����z��
	static std::vector<std::string> aAnimName[static_cast<int>(MODEL_ID::ID_Max)];	//�A�j���[�V�������z��
	static std::vector<bool> aAnimFPS_30[static_cast<int>(MODEL_ID::ID_Max)];		//�A�j���[�V����FPS�t���O

	//�v���g�^�C�v�錾
	void LoadModel(MODEL_ID id);													//���f���Ǎ�
	void LoadTextureName(std::string TexName, TEX_PACK& DataRef) const noexcept;	//�e�N�X�`�����Ǎ�
};
