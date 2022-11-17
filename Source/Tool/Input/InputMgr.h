/**
 * @file InputMgr.h
 * @brief ���͏����}�l�[�W��
 * @author ���J�C�A��
 * @date 2022/07/28
 * @���� 2022/07/28�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <Tool/Input/IM_Keyboard.h>		//�L�[�{�[�h����
#include <Tool/Input/IM_Mouse.h>		//�}�E�X����
#include <Tool/Input/IM_GamePad.h>		//�R���g���[������

//===== �O���錾 =====
class APP_64;

//===== �N���X��` =====

//***** ���̓}�l�[�W�� *****
class INPUT_MGR
{
public:

	//* �ϐ��錾 *
	INPUT_KB m_KB;						//�L�[�{�[�h����
	INPUT_CURSOR m_Mouse;				//�}�E�X����
	INPUT_PAD m_GamePad;				//�R���g���[������

	//* �v���g�^�C�v�錾 *
	INPUT_MGR(APP_64& App) noexcept;
	~INPUT_MGR() noexcept;
	void Update() noexcept;				//�X�V����
};
