/**
 * @file ErrorOutput.h
 * @brief �G���[�o�̓N���X
 * @author ���J�C�A��
 * @date 2022/05/02
 * @���� 2022/05/02�F�N���X�쐬
 *		 2022/11/14�F�R���X�g���N�^���P
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <exception>			//��O����
#include <string>				//������N���X
#include <sstream>				//������X�g���[���N���X

//===== �N���X��` =====

//***** �G���[�o�� *****
class ERROR_OUTPUT : public std::exception
{
public:

	//�v���g�^�C�v�錾
	explicit ERROR_OUTPUT(int nLine, const char* chFile) noexcept;
	~ERROR_OUTPUT() noexcept override;
	virtual std::string GetType() const noexcept = 0;	//�G���[�^�C�v�擾
	std::string GetErrorInfo() const noexcept;			//�G���[���擾
	const char* what() const noexcept override;			//�G���[���o��

protected:

	//�ϐ��錾
	mutable std::string m_InfoBuffer;	//���i�[�p�o�b�t�@

private:

	//�ϐ��錾
	int m_Line;							//�����ʒu�i���s�ځj
	std::string m_File;					//�����t�@�C����
};
