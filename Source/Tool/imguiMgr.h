/**
 * @file imguiMgr.h
 * @brief ImGui�}�l�[�W��
 * @author ���J�C�A��
 * @date 2022/07/18
 * @���� 2022/07/18�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <3rd_Party/ImGui/imgui.h>

//===== �萔�E�}�N����` =====
#define U8(str) reinterpret_cast<const char*>(str)

//===== �N���X�錾 =====

//***** ImGui�}�l�[�W�� *****
class IMGUI_MGR
{
public:

	//�v���g�^�C�v�錾
	IMGUI_MGR() noexcept;
	~IMGUI_MGR() noexcept;
	void DrawDemo() const noexcept;		//DEMO�E�B���h�E�`��
};
