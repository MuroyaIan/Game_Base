/**
 * @file IM_Mouse.h
 * @brief �}�E�X����
 * @author ���J�C�A��
 * @date 2022/07/28
 * @���� 2022/07/28�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <WinApp/WinWindow.h>
#include <GraphicApp/Graphic.h>

//===== �萔�E�}�N����` =====
constexpr int MOUSE_L = static_cast<int>(VK_LBUTTON);		//�}�E�X�̍��{�^��
constexpr int MOUSE_R = static_cast<int>(VK_RBUTTON);		//�}�E�X�̉E�{�^��
constexpr int MOUSE_M = static_cast<int>(VK_MBUTTON);		//�}�E�X�̒����{�^��
constexpr int MOUSE_X1 = static_cast<int>(VK_XBUTTON1);		//�}�E�X�̑�1�g���{�^��
constexpr int MOUSE_X2 = static_cast<int>(VK_XBUTTON2);		//�}�E�X�̑�2�g���{�^��

//===== �O���錾 =====
class APP;
class MOUSE;
class INPUT_KB;

//===== �N���X��` =====

//***** �}�E�X���� *****
class INPUT_CURSOR
{
public:

	//* �v���g�^�C�v�錾 *
	INPUT_CURSOR(APP& App, INPUT_KB& KB) noexcept;
	~INPUT_CURSOR() noexcept;
	bool GetPress(int nBtn) const noexcept;				//�v���X����
	bool GetTrigger(int nBtn) const noexcept;			//�g���K�[����
	bool GetRelease(int nBtn) const noexcept;			//�����[�X����
	bool GetRepeat(int nBtn, float fInterval = 0.5f);	//���s�[�g����i�f�t�H���g0.5�b�j

	DirectX::XMFLOAT2 GetPos() const noexcept;			//���W�擾
	int GetWheelVal() const noexcept;					//�z�C�[���l�擾
	DirectX::XMINT2 GetMoveVal() const noexcept;		//�ړ��ʎ擾

private:

	//* �ϐ��錾 *
	MOUSE& m_Mouse;				//�}�E�X�Q��
	INPUT_KB& m_KB;				//�L�[�{�[�h�Q��
	DirectX::XMINT2 MoveVal;	//�ړ��ʁi���t���[���j

	//* �v���g�^�C�v�錾 *
	void Update() noexcept;		//�X�V����

	//�����w��
	friend class INPUT_MGR;
};
