
//===== �C���N���[�h�� =====
#include <WinApp/WinEO.h>

//===== �N���X���� =====
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

//�G���[�^�C�v�擾
std::string WIN_EO::GetType() const noexcept
{
	return std::string{ "Window�G���[" };
}

//�G���[�R�[�h���擾
std::string WIN_EO::GetErrorCodeInfo(HRESULT hr) const noexcept
{
	//�J�X�^����O
	if (hr == S_OK && (!m_strError.empty()))
		return m_strError;

	//���p���������m��
	LPSTR pMsgBuffer = nullptr;
	const DWORD dwMsgLen = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuffer), 0, nullptr);

	//��O����
	if (dwMsgLen == 0)
		return std::string{ "����`�̃G���[" };

	//���擾
	std::string strError{ pMsgBuffer };
	LocalFree(pMsgBuffer);					//���������
	return std::move(strError);
}

//�G���[���o��
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
