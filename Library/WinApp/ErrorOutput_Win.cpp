//===== インクルード部 =====
#include <WinApp/ErrorOutput_Win.h>

//===== クラス実装 =====
CT_EO_WIN::CT_EO_WIN(const int nLine, const char* chFile, const HRESULT hr, const char* str) noexcept
	: CT_ERROR_OUTPUT(nLine, chFile)
	, m_Hr(hr)
{
	if (str != nullptr)
		m_StrError = str;
	else
	{
		m_StrError.resize(0);
		m_StrError.reserve(0);
	}
}

CT_EO_WIN::~CT_EO_WIN() noexcept = default;

/**
 * エラータイプ取得
 *
 * \param
 * \return std::string
 */
std::string CT_EO_WIN::GetType() const noexcept
{
	return std::string{"Windows Error"};
}

/**
 * エラーコード情報取得
 *
 * \param hr
 * \return std::string
 */
std::string CT_EO_WIN::GetErrorCodeInfo(const HRESULT hr) const noexcept
{
	//カスタム例外
	if (hr == S_OK && (!m_StrError.empty()))
		return m_StrError;

	//情報用メモリを確保
	LPSTR l_PtrMsgBuffer = nullptr;
	const DWORD l_MsgLen = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&l_PtrMsgBuffer), 0, nullptr);

	//例外処理
	if (l_MsgLen == 0)
		return std::string{"Unknown Error"};

	//情報取得
	std::string l_StrError{l_PtrMsgBuffer};
	LocalFree(l_PtrMsgBuffer); //メモリ解放
	return l_StrError;
}

/**
 * エラー情報出力
 *
 * \param
 * \return const char*
 */
const char* CT_EO_WIN::what() const noexcept
{
	std::ostringstream l_Oss;
	l_Oss << GetType() << '\n'
		<< "[Error Code] " << m_Hr << '\n'
		<< "[Description] " << GetErrorCodeInfo(m_Hr) << '\n'
		<< GetErrorInfo();
	m_InfoBuffer = l_Oss.str();
	return m_InfoBuffer.c_str();
}
