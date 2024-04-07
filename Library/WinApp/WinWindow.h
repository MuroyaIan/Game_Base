/**
 * @file WinWindow.h
 * @brief �Q�[���E�B���h�E����
 * @author ���J�C�A��
 * @date 2022/05/01
 * @���� 2022/05/01�F�E�B���h�E�N���X�쐬
 *		 2022/05/02�F�G���[�o�̓N���X�쐬�A�L�[�{�[�h��t��������
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <WinApp/BaseWindow.h>			//�C���^�[�t�F�[�X
#include <WinApp/ErrorOutput_Win.h>		//�G���[�o��
#include <vector>						//���I�z��

//===== �N���X��` =====
class CT_IW_WIN final : public CT_IF_WINDOW
{
public:

	//�R�s�[�����[�u
	CT_IW_WIN(const CT_IW_WIN&) = default;
	CT_IW_WIN& operator =(const CT_IW_WIN&) = default;
	CT_IW_WIN(CT_IW_WIN&&) noexcept = default;
	CT_IW_WIN& operator=(CT_IW_WIN&&) noexcept = default;

	//�v���g�^�C�v�錾
	explicit CT_IW_WIN(const LPCWSTR& windowName, const int& nWndWidth, const int& nWndHeight, const int& nWndPosX = CW_USEDEFAULT, const int& nWndPosY = CW_USEDEFAULT);
	~CT_IW_WIN() noexcept(false) override;
	void Transform(const int& nWndPosX, const int& nWndPosY, const int& nWndWidth = 0, const int& nWndHeight = 0) override; //�g�����X�t�H�[��
	void TitlePrint(const std::string& text) const override;                                                                //�^�C�g���o��
	void TitlePrint_MousePos() const override;                                                                              //�^�C�g���o�́i�}�E�X���W�j
	void TitlePrint_WheelVal() override;                                                                                    //�^�C�g���o�́i�z�C�[���l�j
	void EnableCursor() noexcept override;                                                                                  //�}�E�X�g�p
	void DisableCursor() noexcept override;                                                                                 //�}�E�X�s�g�p
	[[nodiscard]] bool IsUsingCursor() const noexcept override;                                                             //�}�E�X�g�p��Ԋm�F

	[[nodiscard]] std::pair<int, int> GetPos() const noexcept override //�E�B���h�E������W�擾
	{
		return {m_PosX, m_PosY};
	}

	[[nodiscard]] std::pair<int, int> GetSize() const noexcept override //�E�B���h�E�T�C�Y�擾
	{
		return {m_Width, m_Height};
	}

	[[nodiscard]] bool IsUsingImgui() const noexcept override //Imgui�g�p�m�F
	{
		return m_bUseImgui;
	}

	[[nodiscard]] HWND GetHandle() const noexcept //�n���h���擾
	{
		return m_WinHandle;
	}

private:

	//�ϐ��錾
	static constexpr LPCWSTR c_Class_Name = L"WindowClass_Game"; //�N���X��
	HINSTANCE m_AppInst;                                         //Instance�n���h��
	int m_PosX;                                                  //�E�B���h�E����X���W
	int m_PosY;                                                  //�E�B���h�E����Y���W
	int m_Width;                                                 //���i�N���C�A���g�̈�j
	int m_Height;                                                //�����i�N���C�A���g�̈�j
	HWND m_WinHandle;                                            //�n���h��
	bool m_bDrawCursor;                                          //�}�E�X�`��t���O
	std::vector<BYTE> m_RawBuffer;                               //RawInput�p�o�b�t�@
	bool m_bUseImgui;                                            //Imgui�g�p���t���O

	//�v���g�^�C�v�錾
	static LRESULT CALLBACK WndProc_Init(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;        //WndProc������
	static LRESULT CALLBACK WndProc_Call(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;        //WndProc�Ăяo��
	LRESULT WndProc(const HWND& hWnd, const UINT& uMsg, const WPARAM& wParam, const LPARAM& lParam) noexcept; //WndProc�{����
	static void HideCursor() noexcept;                                                                        //�}�E�X��\��
	static void ShowCursor() noexcept;                                                                        //�}�E�X�\��
	void LockCursor() const noexcept;                                                                         //�}�E�X���E�B���h�E���Ƀ��b�N
	void UnlockCursor() const noexcept;                                                                       //�}�E�X���E�B���h�E���Ƀ��b�N����
};
