/**
 * @file DirectionalLight.h
 * @brief ���s����
 * @author ���J�C�A��
 * @date 2022/08/20
 * @���� 2022/08/20�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <Light/LightMgr.h>

//===== �N���X��` =====

//***** ���s���� *****
class DIRECTIONAL_LIGHT : public LIGHT
{
public:

	//�v���g�^�C�v�錾
	DIRECTIONAL_LIGHT(APP_64& App) noexcept;
	~DIRECTIONAL_LIGHT() noexcept override;
	void Update() const noexcept override;		//�X�V����

	LIGHT_DIRECTIONAL& GetData() noexcept		//���C�g���Q��
	{
		return m_LightData;
	}

private:

	//�ϐ��錾
	LIGHT_MGR& m_LightMgr;						//���C�g�}�l�[�W���Q��
	LIGHT_DIRECTIONAL m_LightData;				//���C�g���
};
