/**
 * @file ErrorOutput.h
 * @brief �G���[�o�̓N���X
 * @author ���J�C�A��
 * @date 2022/05/02
 * @���� 2022/05/02�F�N���X�쐬
 *		 2022/11/14�F�R���X�g���N�^���P
 *		 2024/04/02�F�������P
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <exception>			//��O����
#include <string>				//������N���X
#include <sstream>				//������X�g���[���N���X

//===== �N���X��` =====

//***** �G���[�o�� *****
class CT_ERROR_OUTPUT : public std::exception
{
public:

	//�R�s�[�����[�u
	CT_ERROR_OUTPUT(const CT_ERROR_OUTPUT&) = default;
	CT_ERROR_OUTPUT& operator =(const CT_ERROR_OUTPUT&) = default;
	CT_ERROR_OUTPUT(CT_ERROR_OUTPUT&&) noexcept = default;
	CT_ERROR_OUTPUT& operator=(CT_ERROR_OUTPUT&&) noexcept = default;

	//�v���g�^�C�v�錾
	explicit CT_ERROR_OUTPUT(const int& nLine, const char* chFile) noexcept;
	~CT_ERROR_OUTPUT() noexcept override;
	std::string GetErrorInfo() const noexcept;  //�G���[���擾
	const char* what() const noexcept override; //�G���[���o��

	//���z�֐�
	virtual std::string GetType() const noexcept = 0; //�G���[�^�C�v�擾

protected:

	//�ϐ��錾
	mutable std::string m_InfoBuffer; //���i�[�p�o�b�t�@

private:

	//�ϐ��錾
	int m_Line;         //�����ʒu�i���s�ځj
	std::string m_File; //�����t�@�C����
};
