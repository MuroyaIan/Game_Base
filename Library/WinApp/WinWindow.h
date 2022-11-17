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
#include <WinApp/BaseWindow.h>	//�C���^�[�t�F�[�X
#include <WinApp/WinEO.h>		//�G���[�o��
#include <vector>				//���I�z��

//===== �N���X��` =====
class WIN_WINDOW : public IF_WINDOW
{
public:

	//�v���g�^�C�v�錾
	WIN_WINDOW(LPCWSTR WindowName, int nWndWidth, int nWndHeight, int nWndPosX = CW_USEDEFAULT, int nWndPosY = CW_USEDEFAULT);
	~WIN_WINDOW() noexcept(false) override;
	void Transform(int nWndPosX, int nWndPosY, int nWndWidth = 0, int nWndHeight = 0) override;		//�g�����X�t�H�[��
	void TitlePrint(const std::string& Text) const override;										//�^�C�g���o��
	void TitlePrint_MousePos() const override;														//�^�C�g���o�́i�}�E�X���W�j
	void TitlePrint_WheelVal() override;															//�^�C�g���o�́i�z�C�[���l�j
	void EnableCursor() noexcept override;
	void DisableCursor() noexcept override;															//�}�E�X�g�p�E�s�g�p
	bool IsUsingCursor() const noexcept override;													//�}�E�X�g�p��Ԋm�F

	std::pair<int, int> GetPos() const noexcept override	//�E�B���h�E������W�擾
	{
		return { m_PosX, m_PosY };
	}
	std::pair<int, int> GetSize() const noexcept override	//�E�B���h�E�T�C�Y�擾
	{
		return { m_Width, m_Height };
	}
	const HWND GetHandle() const noexcept					//�n���h���擾
	{
		return m_hWindow;
	}
	const bool IsUsingImgui() const noexcept override		//Imgui�g�p�m�F
	{
		return m_useImgui;
	}

private:

	//�ϐ��錾
	static constexpr const LPCWSTR CLASS_NAME = L"WindowClass_Game";	//�N���X��
	HINSTANCE m_hAppInst;												//Instance�n���h��
	int m_PosX;
	int m_PosY;						//�ʒu
	int m_Width;
	int m_Height;					//���E�����i�N���C�A���g�̈�j
	HWND m_hWindow;					//�n���h��
	bool m_bDrawCursor;				//�}�E�X�`��t���O
	std::vector<BYTE> m_RawBuffer;	//RawInput�p�o�b�t�@
	bool m_useImgui;				//Imgui�g�p���t���O

	//�v���g�^�C�v�錾
	static LRESULT CALLBACK WndProc_Init(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;	//WndProc������
	static LRESULT CALLBACK WndProc_Call(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;	//WndProc�Ăяo��
	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;						//WndProc�{����
	void HideCursor() noexcept;
	void ShowCursor() noexcept;																			//�}�E�X��\���E�\��
	void LockCursor() noexcept;
	void UnlockCursor() noexcept;																		//�}�E�X���E�B���h�E���Ƀ��b�N�E���b�N����
};
