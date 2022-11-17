
//===== インクルード部 =====
#include <WinApp/WinEO.h>

//===== クラス実装 =====
WIN_EO::WIN_EO(int nLine, const char* chFile, HRESULT hr, const char* str) noexcept :
	ERROR_OUTPUT(nLine, chFile), m_hr(hr), m_strError()
{
	if (str != nullptr)
		m_strError = str;
	else {
		m_strError.resize(0);
		m_strError.reserve(0);
	}
}

WIN_EO::~WIN_EO() noexcept
{
}

//エラータイプ取得
std::string WIN_EO::GetType() const noexcept
{
	return std::string{ "Windowエラー" };
}

//エラーコード情報取得
std::string WIN_EO::GetErrorCodeInfo(HRESULT hr) const noexcept
{
	//カスタム例外
	if (hr == S_OK && (!m_strError.empty()))
		return m_strError;

	//情報用メモリを確保
	LPSTR pMsgBuffer = nullptr;
	const DWORD dwMsgLen = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuffer), 0, nullptr);

	//例外処理
	if (dwMsgLen == 0)
		return std::string{ "未定義のエラー" };

	//情報取得
	std::string strError{ pMsgBuffer };
	LocalFree(pMsgBuffer);					//メモリ解放
	return std::move(strError);
}

//エラー情報出力
const char* WIN_EO::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << m_hr << std::endl
		<< "[Description] " << GetErrorCodeInfo(m_hr) << std::endl
		<< GetErrorInfo();
	m_InfoBuffer = std::move(oss.str());
	return m_InfoBuffer.c_str();
}
