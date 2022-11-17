/**
 * @file gMemory.h
 * @brief �������Ǘ��w�b�_�t�@�C���Q
 * @author ���J�C�A��
 * @date 2022/06/10
 * @���� 2022/06/10�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <memory>					//�X�}�[�g�|�C���^
#include <array>					//�Œ�z��
#include <vector>					//���I�z��
#include <list>						//���X�g
#include <map>						//�A�z�z��
#include <algorithm>

//===== �萔�E�}�N����` =====
#ifdef _DEBUG
#
#	define NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)	//���������[�N���o
#
#else
#
#	define NEW new
#
#endif // _DEBUG

#ifndef SAFE_RELEASE
#
#	define SAFE_RELEASE(x)	do { if(x) { (x)->Release(); (x)=nullptr; } } while(0)
#
#endif // !SAFE_RELEASE

#ifndef SAFE_DELETE
#
#	define SAFE_DELETE(x)	do { if(x) { delete(x); (x)=nullptr; } } while(0)
#
#endif // !SAFE_DELETE

#ifndef SAFE_DELETE_ARRAY
#
#	define SAFE_DELETE_ARRAY(x)	do { if(x) { delete[](x); (x)=nullptr; } } while(0)
#
#endif // !SAFE_DELETE_ARRAY
