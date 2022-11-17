/**
 * @file TextIO.h
 * @brief �e�L�X�g���o�̓w�b�_�t�@�C���Q
 * @author ���J�C�A��
 * @date 2022/06/09
 * @���� 2022/06/09�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <iostream>					//�W�����o��
#include <string>					//������N���X
#include <sstream>					//������X�g���[���N���X
#include <Tool/gMemory.h>

//===== �v���g�^�C�v�錾 =====
namespace gText
{
	//�����񕪊�
	std::vector<std::string> Split(const std::string& Text, char Sep);
}
