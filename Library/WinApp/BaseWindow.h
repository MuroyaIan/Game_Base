/**
 * @file BaseWindow.h
 * @brief �E�B���h�E�p�C���^�[�t�F�[�X
 * @author ���J�C�A��
 * @date 2022/11/15
 * @���� 2022/11/15�F�t�@�C���쐬
 *		 2024/04/07�F�������P
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <string>				//������N���X
#include <WinApp/Keyboard.h>	//�L�[�{�[�h����
#include <WinApp/Mouse.h>		//�}�E�X����

//===== �N���X��` =====
class CT_IF_WINDOW
{
public:

	//�R�s�[�����[�u
	CT_IF_WINDOW(const CT_IF_WINDOW&) = default;
	CT_IF_WINDOW& operator =(const CT_IF_WINDOW&) = default;
	CT_IF_WINDOW(CT_IF_WINDOW&&) noexcept = default;
	CT_IF_WINDOW& operator=(CT_IF_WINDOW&&) noexcept = default;

	//�ϐ��錾
	CT_KEYBOARD m_Keyboard; //�L�[�{�[�h����
	CT_MOUSE m_Mouse;       //�}�E�X����

	//�R���X�g���N�^
	explicit CT_IF_WINDOW() noexcept = default;
	virtual ~CT_IF_WINDOW() noexcept(false) = default;

	//�v���g�^�C�v�錾
	virtual void Transform(const int& nWndPosX, const int& nWndPosY, const int& nWndWidth = 0, const int& nWndHeight = 0) = 0; //�g�����X�t�H�[��
	virtual void TitlePrint(const std::string& text) const = 0;                                                                //�^�C�g���o��
	virtual void TitlePrint_MousePos() const = 0;                                                                              //�^�C�g���o�́i�}�E�X���W�j
	virtual void TitlePrint_WheelVal() = 0;                                                                                    //�^�C�g���o�́i�z�C�[���l�j
	virtual void EnableCursor() noexcept = 0;                                                                                  //�}�E�X�g�p�E�s�g�p
	virtual void DisableCursor() noexcept = 0;
	[[nodiscard]] virtual bool IsUsingCursor() const noexcept = 0;			//�}�E�X�g�p��Ԋm�F

	[[nodiscard]] virtual std::pair<int, int> GetPos() const noexcept = 0;  //�E�B���h�E������W�擾
	[[nodiscard]] virtual std::pair<int, int> GetSize() const noexcept = 0; //�E�B���h�E�T�C�Y�擾
	[[nodiscard]] virtual bool IsUsingImgui() const noexcept = 0;           //Imgui�g�p�m�F
};
