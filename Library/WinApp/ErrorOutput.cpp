
//===== �C���N���[�h�� =====
#include <WinApp/ErrorOutput.h>

//===== �N���X���� =====

//***** �G���[�o�� *****
ERROR_OUTPUT::ERROR_OUTPUT(int nLine, const char* chFile) noexcept :
	std::exception(), m_Line(nLine), m_File(chFile), m_InfoBuffer()
{
}

ERROR_OUTPUT::~ERROR_OUTPUT() noexcept
{
}

//�G���[���擾
std::string ERROR_OUTPUT::GetErrorInfo() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << m_File << std::endl
		<< "[Line] " << m_Line;
	return std::move(oss.str());
}

//�G���[���o��
const char* ERROR_OUTPUT::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetErrorInfo();
	m_InfoBuffer = std::move(oss.str());
	return m_InfoBuffer.c_str();
}
