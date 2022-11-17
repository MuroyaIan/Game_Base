/**
 * @file WinEO.h
 * @brief �G���[�o�̓N���X(Window)
 * @author ���J�C�A��
 * @date 2022/11/15
 * @���� 2022/11/15�F�N���X�쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <WinApp/ErrorOutput.h>
#include <WinApp/WinConfig.h>

//===== �N���X��` =====
class WIN_EO : public ERROR_OUTPUT
{
public:

	//�v���g�^�C�v�錾
	explicit WIN_EO(int nLine, const char* chFile, HRESULT hr, const char* str = nullptr) noexcept;
	~WIN_EO() noexcept override;
	std::string GetType() const noexcept override;				//�G���[�^�C�v�擾
	std::string GetErrorCodeInfo(HRESULT hr) const noexcept;	//�G���[�R�[�h���擾
	const char* what() const noexcept override;					//�G���[���o��

private:

	//�ϐ��錾
	HRESULT m_hr;				//���b�Z�[�W���ʎq
	std::string m_strError;		//�J�X�^���G���[���
};

//===== �}�N����` =====
#define ERROR_DEFAULT() WIN_EO{__LINE__, __FILE__, HRESULT_FROM_WIN32(GetLastError())}
#define ERROR_EX(hr) WIN_EO{__LINE__, __FILE__, (hr)}
#define ERROR_EX2(str) WIN_EO{__LINE__, __FILE__, S_OK, (str)}
