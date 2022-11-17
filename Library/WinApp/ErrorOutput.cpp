
//===== インクルード部 =====
#include <WinApp/ErrorOutput.h>

//===== クラス実装 =====

//***** エラー出力 *****
ERROR_OUTPUT::ERROR_OUTPUT(int nLine, const char* chFile) noexcept :
	std::exception(), m_Line(nLine), m_File(chFile), m_InfoBuffer()
{
}

ERROR_OUTPUT::~ERROR_OUTPUT() noexcept
{
}

//エラー情報取得
std::string ERROR_OUTPUT::GetErrorInfo() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << m_File << std::endl
		<< "[Line] " << m_Line;
	return std::move(oss.str());
}

//エラー情報出力
const char* ERROR_OUTPUT::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetErrorInfo();
	m_InfoBuffer = std::move(oss.str());
	return m_InfoBuffer.c_str();
}
