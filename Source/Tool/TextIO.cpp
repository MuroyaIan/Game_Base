
//===== �C���N���[�h�� =====
#include <Tool/TextIO.h>

//===== �v���g�^�C�v�錾 =====

//�����񕪊�
std::vector<std::string> gText::Split(const std::string& Text, char Sep)
{
	//�ϐ��錾
	std::vector<std::string> aStr;  //�����㕶����z��
	std::stringstream ss(Text);     //�����O������
	std::string strBuff;            //�����p������

	//��������
	while (std::getline(ss, strBuff, Sep))
		aStr.push_back(strBuff);
	return aStr;
}
