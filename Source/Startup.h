/**
 * @file Startup.h
 * @brief �X�^�[�g�A�b�v����
 * @author ���J�C�A��
 * @date 2022/04/30
 * @���� 2022/04/30�F�E�B���h�E�쐬�AWndProc�쐬
 *		 2022/06/09�F�E�B���h�E�������C�u������
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �萔�E�}�N����` =====

#ifdef _DEBUG

#define IMGUI						 //ImGui�g�p
constexpr bool gc_IS_DEBUG = true;   //�f�o�b�O���[�h
constexpr bool gc_RUN_EDITOR = true; //�G�f�B�^���[�h

#else

constexpr bool gc_IS_DEBUG = false;
constexpr bool gc_RUN_EDITOR = false;

#endif // _DEBUG
