//===== インクルード部 =====
#include <WinApp/ErrorOutput.h>

//===== クラス実装 =====
CT_ERROR_OUTPUT::CT_ERROR_OUTPUT(const int& nLine, const char* chFile) noexcept
	: m_Line(nLine)
	, m_File(chFile) {}

CT_ERROR_OUTPUT::~CT_ERROR_OUTPUT() noexcept = default;

/**
 * エラー情報取得
 *
 * \param
 * \return std::string
 */
std::string CT_ERROR_OUTPUT::GetErrorInfo() const noexcept
{
	std::ostringstream l_Oss;
	l_Oss << "[File] " << m_File << '\n'
		<< "[Line] " << m_Line;
	return l_Oss.str();
}

/**
 * エラー情報出力
 *
 * \param
 * \return const char*
 */
const char* CT_ERROR_OUTPUT::what() const noexcept
{
	std::ostringstream l_Oss;
	l_Oss << GetType() << '\n'
		<< GetErrorInfo();
	m_InfoBuffer = l_Oss.str();
	return m_InfoBuffer.c_str();
}
