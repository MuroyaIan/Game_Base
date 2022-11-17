/**
 * @file Camera_Test.h
 * @brief �e�X�g�J����
 * @author ���J�C�A��
 * @date 2022/08/12
 * @���� 2022/08/12�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <Camera/Camera.h>

//===== �N���X��` =====

//***** �e�X�g�J���� *****
class CAMERA_TEST : public CAMERA
{
public:

	//�v���g�^�C�v�錾
	CAMERA_TEST(APP_64& App) noexcept;
	~CAMERA_TEST() noexcept override;
	void Update() noexcept override;	//�X�V����

private:

	//�ϐ��錾
	INPUT_MGR& m_Input;					//���͎Q��

	//�v���g�^�C�v�錾
	void ResetCamera() noexcept override;	//�J�������Z�b�g
	void UpdateMatrix() noexcept;			//�s��X�V
};
