/**
 * @file WinConfig.h
 * @brief OS���ݒ�p�w�b�_(Windows)
 * @author ���J�C�A��
 * @date 2022/05/04
 * @���� 2022/05/04�F�w�b�_�쐬
 *		 2022/11/14�F�f�o�b�O�ݒ�ǉ�
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#define WINVER 0x0A00			//�o�[�W�����w��(Win10)
#include <sdkddkver.h>

//�g�p���Ȃ��@�\�̐���
//#define WIN32_LEAN_AND_MEAN	//FBX�Ǎ��΍�
#define NOGDICAPMASKS
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
//#define NOCTLMGR				//IMGUI�΍�
#define NODRAWTEXT
#define NOKERNEL
//#define NONLS					//�e�N�X�`���Ǎ��΍�
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE
#include <Windows.h>

//�f�o�b�O�@�\
#ifdef _DEBUG

#include <debugapi.h>	//�f�o�b�O�\��
#include <crtdbg.h>		//���������[�N���o

#define PRINT_D(wStr) OutputDebugString(wStr)	//�f�o�b�O�o�͗p

#endif // _DEBUG
