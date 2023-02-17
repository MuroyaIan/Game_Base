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

//===== �O���錾 =====
namespace ModelRef {
	struct MODEL_PACK;
	struct TEX_PACK;
}

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

		ID_Max
	};

	//�v���g�^�C�v�錾
	MODEL_MGR();
	~MODEL_MGR() noexcept;

	ModelRef::MODEL_PACK& GetModelPack(MODEL_ID id) const	//���f���p�b�N�Q��
	{
		return *m_aModelPackPtr[static_cast<int>(id)];
	}

private:

	//�ϐ��錾
	std::vector<std::unique_ptr<ModelRef::MODEL_PACK>> m_aModelPackPtr;				//���f���p�b�N�z��

	static std::string aModelName[static_cast<int>(MODEL_ID::ID_Max)];				//���f�����z��
	static std::vector<std::string> aAnimName[static_cast<int>(MODEL_ID::ID_Max)];	//�A�j���[�V�������z��
	static std::vector<bool> aAnimFPS_30[static_cast<int>(MODEL_ID::ID_Max)];		//�A�j���[�V����FPS�t���O

	//�v���g�^�C�v�錾
	void LoadModel(MODEL_ID id);															//���f���Ǎ�
	void LoadTextureName(std::string TexName, std::vector<ModelRef::TEX_PACK>& DataRef);	//�e�N�X�`�����Ǎ�
};
