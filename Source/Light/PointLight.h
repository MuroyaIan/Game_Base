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
#include <Light/Light.h>

//===== �O���錾 =====
class LIGHT_MGR;

//===== �N���X��` =====

//***** �_���� *****
class POINT_LIGHT : public LIGHT
{
public:

	//�v���g�^�C�v�錾
	explicit POINT_LIGHT(APP& App, float Range = 100.0f) noexcept;
	~POINT_LIGHT() noexcept override;
	void Update() const noexcept override;			//�X�V����
	void ClacAttenuation(float Range) noexcept;		//�����v�Z

	LIGHT_POINT& GetData() noexcept					//���C�g���Q��
	{
		return m_LightData;
	}

private:

	//�ϐ��錾
	LIGHT_POINT m_LightData;	//���C�g���
	float m_Range;				//���C�g������͈�
	LIGHT_MGR& m_LightMgr;		//���C�g�}�l�[�W���Q��
};
