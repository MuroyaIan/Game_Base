
//===== �C���N���[�h�� =====
#include <Tool/Rand.h>
#include <WinApp/WinEO.h>		//�G���[�o��

//===== �ÓI�����o�ϐ� =====
RAND RAND_MAKER::m_Rand{};		//����������

//===== �N���X���� =====

//***** ���������N���X *****
RAND_MAKER::RAND_MAKER() noexcept
{
}
RAND_MAKER::~RAND_MAKER() noexcept
{
}

//�����평����
void RAND_MAKER::InitMaker() noexcept
{
	std::random_device Seed;	//�V�[�h����
	m_Rand.seed(Seed());		//�����̃V�[�h�w��
}

//��������
int RAND_MAKER::MakeRand_Int(int Min, int Max)
{
	//***** ��O���� *****
	if (Min > Max)
		throw ERROR_EX2("�y�x���z�����̎w��͈͂��s�K�؁I");

	//***** �������� *****
	RandRange_Int RandDist(Min, Max);	//��l���z���w��
	return RandDist(m_Rand);			//�����������ꂽ������Ԃ�
}

//��������
float RAND_MAKER::MakeRand_Float(float Min, float Max)
{
	//***** ��O���� *****
	if (Min > Max)
		throw ERROR_EX2("�y�x���z�����̎w��͈͂��s�K�؁I");

	//***** �������� *****
	RandRange_Float RandDist(Min, Max);		//��l���z���w��
	return RandDist(m_Rand);				//�����������ꂽ������Ԃ�
}
