/**
 * @file Camera_FP.h
 * @brief FP�J����
 * @author ���J�C�A��
 * @date 2022/09/26
 * @���� 2022/09/26�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <Camera/Camera.h>

//===== �N���X��` =====

//***** FP�J���� *****
class CAMERA_FP : public CAMERA
{
public:

	//�v���g�^�C�v�錾
	explicit CAMERA_FP(APP& App) noexcept;
	~CAMERA_FP() noexcept override;
	void Update() noexcept override;		//�X�V����

private:

	//�ϐ��錾
	INPUT_MGR& m_Input;						//���͎Q��

	//�v���g�^�C�v�錾
	void UpdateMatrix() noexcept;			//�s��X�V
	void ResetCamera() noexcept override;	//�J�������Z�b�g
};
