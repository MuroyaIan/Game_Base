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
#define APP_WIN64					//�v���b�g�t�H�[���w��
#define DIRECTX_11					//dx11
//#define DIRECTX_12				//dx12

//�f�o�b�O����
#ifdef _DEBUG

constexpr bool IS_DEBUG = true;		//�f�o�b�O���[�h
#define IMGUI						//ImGui�g�p
constexpr bool RUN_EDITOR = true;	//�G�f�B�^���[�h

#else

constexpr bool IS_DEBUG = false;
constexpr bool RUN_EDITOR = false;

#endif // _DEBUG
