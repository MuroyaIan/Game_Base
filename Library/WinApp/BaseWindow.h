/**
 * @file BaseWindow.h
 * @brief �E�B���h�E�p�C���^�[�t�F�[�X
 * @author ���J�C�A��
 * @date 2022/11/15
 * @���� 2022/11/15�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <string>				//������N���X
#include <WinApp/Keyboard.h>	//�L�[�{�[�h����
#include <WinApp/Mouse.h>		//�}�E�X����

//===== �N���X��` =====
class IF_WINDOW
{
public:

	//�ϐ��錾
	KEYBOARD m_Keyboard;	//�L�[�{�[�h����
	MOUSE m_Mouse;			//�}�E�X����

	//�v���g�^�C�v�錾
	explicit IF_WINDOW() noexcept : m_Keyboard(), m_Mouse() {}
	virtual ~IF_WINDOW() noexcept(false) {}
	virtual void Transform(int nWndPosX, int nWndPosY, int nWndWidth = 0, int nWndHeight = 0) = 0;	//�g�����X�t�H�[��
	virtual void TitlePrint(const std::string& Text) const = 0;										//�^�C�g���o��
	virtual void TitlePrint_MousePos() const = 0;													//�^�C�g���o�́i�}�E�X���W�j
	virtual void TitlePrint_WheelVal() = 0;															//�^�C�g���o�́i�z�C�[���l�j
	virtual void EnableCursor() noexcept = 0;														//�}�E�X�g�p�E�s�g�p
	virtual void DisableCursor() noexcept = 0;
	virtual bool IsUsingCursor() const noexcept = 0;												//�}�E�X�g�p��Ԋm�F

	virtual std::pair<int, int> GetPos() const noexcept = 0;	//�E�B���h�E������W�擾
	virtual std::pair<int, int> GetSize() const noexcept = 0;	//�E�B���h�E�T�C�Y�擾
	virtual const bool IsUsingImgui() const noexcept = 0;		//Imgui�g�p�m�F
};
