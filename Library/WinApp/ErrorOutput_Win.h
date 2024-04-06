/**
 * @file ErrorOutput_Win.h
 * @brief �G���[�o�̓N���X(Window)
 * @author ���J�C�A��
 * @date 2022/11/15
 * @���� 2022/11/15�F�N���X�쐬
 *		 2024/04/02�F�������P
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <WinApp/ErrorOutput.h>
#include <WinApp/WinConfig.h>

//===== �N���X��` =====

//***** �G���[�o�́iWindows�p�j *****
class CT_EO_WIN final : public CT_ERROR_OUTPUT
{
public:

	//�R�s�[�����[�u
	CT_EO_WIN(const CT_EO_WIN&) = default;
	CT_EO_WIN& operator =(const CT_EO_WIN&) = default;
	CT_EO_WIN(CT_EO_WIN&&) noexcept = default;
	CT_EO_WIN& operator=(CT_EO_WIN&&) noexcept = default;

	//�v���g�^�C�v�錾
	explicit CT_EO_WIN(const int& nLine, const char* chFile, const HRESULT& hr, const char* str = nullptr) noexcept;
	~CT_EO_WIN() noexcept override;
	std::string GetType() const noexcept override;                  //�G���[�^�C�v�擾
	std::string GetErrorCodeInfo(const HRESULT& hr) const noexcept; //�G���[�R�[�h���擾
	const char* what() const noexcept override;                     //�G���[���o��

private:

	//�ϐ��錾
	HRESULT m_Hr;           //���b�Z�[�W���ʎq
	std::string m_StrError; //�J�X�^���G���[���
};

//===== �}�N����` =====
#define ERROR_DEFAULT() CT_EO_WIN{__LINE__, __FILE__, HRESULT_FROM_WIN32(GetLastError())}
#define ERROR_EX(hr) CT_EO_WIN{__LINE__, __FILE__, (hr)}
#define ERROR_EX2(str) CT_EO_WIN{__LINE__, __FILE__, S_OK, (str)}
