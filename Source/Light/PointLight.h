/**
 * @file PointLight.h
 * @brief �_����
 * @author ���J�C�A��
 * @date 2022/08/13
 * @���� 2022/08/13�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <Light/LightMgr.h>

//===== �N���X��` =====

//***** �_���� *****
class POINT_LIGHT : public LIGHT
{
public:

	//�v���g�^�C�v�錾
	POINT_LIGHT(APP_64& App) noexcept;
	~POINT_LIGHT() noexcept override;
	void Update() const noexcept override;	//�X�V����

	LIGHT_POINT& GetData() noexcept			//���C�g���Q��
	{
		return m_LightData;
	}

private:

	//�ϐ��錾
	LIGHT_MGR& m_LightMgr;					//���C�g�}�l�[�W���Q��
	LIGHT_POINT m_LightData;				//���C�g���
};
