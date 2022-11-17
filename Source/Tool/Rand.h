/**
 * @file Rand.h
 * @brief ��������
 * @author ���J�C�A��
 * @date 2022/07/04
 * @���� 2022/07/04�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <random>					//��������

//===== �萔�E�}�N����` =====
using RAND = std::mt19937_64;
using RandRange_Int = std::uniform_int_distribution<int>;
using RandRange_Float = std::uniform_real_distribution<float>;	//��l���z��

//===== �N���X��` =====

//***** ���������N���X *****
class RAND_MAKER
{
public:

	//�v���g�^�C�v�錾
	static void InitMaker() noexcept;									//�����평����
	static int MakeRand_Int(int Min = 0, int Max = 9);					//��������
	static float MakeRand_Float(float Min = 0.0f, float Max = 9.9f);	//��������

private:

	//�ϐ��錾
	static RAND m_Rand;		//����������

	//�v���g�^�C�v�錾
	RAND_MAKER() noexcept;
	~RAND_MAKER() noexcept;
};
