//===== �C���N���[�h�� =====
#include <WinApp/Keyboard.h>

//===== �N���X���� =====

//***** �L�[�C�x���g *****
CT_KEY_EVENTS::CT_KEY_EVENTS() noexcept
	: m_Type(ET_KEY_STATUS::me_Invalid)
	, m_Code(0u) {}

CT_KEY_EVENTS::CT_KEY_EVENTS(const ET_KEY_STATUS& type, const unsigned char& code) noexcept
	: m_Type(type)
	, m_Code(code) {}

CT_KEY_EVENTS::~CT_KEY_EVENTS() noexcept = default;

//***** �L�[�{�[�h���� *****
CT_KEYBOARD::CT_KEYBOARD() noexcept
	: m_KeyStates(0ull) {}

CT_KEYBOARD::~CT_KEYBOARD() noexcept = default;

/**
 * �L�[�����m�F
 *
 * \param keyCode
 * \return bool
 */
bool CT_KEYBOARD::KeyIsPressed(const unsigned char& keyCode) const noexcept
{
	return m_KeyStates[keyCode];
}

/**
 * �L�[�Ǎ���
 *
 * \param
 * \return CT_KEY_EVENTS
 */
CT_KEY_EVENTS CT_KEYBOARD::ReadKey() noexcept
{
	if (!m_KeyBuffer.empty())
	{
		CT_KEY_EVENTS l_Event = m_KeyBuffer.front(); //���̃C�x���g�ɃA�N�Z�X
		m_KeyBuffer.pop();                           //�C�x���g���폜����
		return l_Event;
	}

	return CT_KEY_EVENTS{};
}

/**
 * �e�L�X�g�Ǎ���
 *
 * \param
 * \return unsigned char
 */
unsigned char CT_KEYBOARD::ReadChar() noexcept
{
	if (!m_CharBuffer.empty())
	{
		const unsigned char l_CharCode = m_CharBuffer.front();
		m_CharBuffer.pop();
		return l_CharCode;
	}

	return 255u;
}

/**
 * �L�[�o�b�t�@�̋󂫊m�F
 *
 * \param
 * \return bool
 */
bool CT_KEYBOARD::KeyIsEmpty() const noexcept
{
	return m_KeyBuffer.empty();
}

/**
 * �e�L�X�g�o�b�t�@�̋󂫊m�F
 *
 * \param
 * \return bool
 */
bool CT_KEYBOARD::CharIsEmpty() const noexcept
{
	return m_CharBuffer.empty();
}

/**
 * �L�[�o�b�t�@�N���A
 *
 * \param
 * \return void
 */
void CT_KEYBOARD::ClearKeyBuffer() noexcept
{
	m_KeyBuffer = std::queue<CT_KEY_EVENTS>{};
}

/**
 * �e�L�X�g�o�b�t�@�N���A
 *
 * \param
 * \return void
 */
void CT_KEYBOARD::ClearCharBuffer() noexcept
{
	m_CharBuffer = std::queue<unsigned char>{};
}

/**
 * �S�o�b�t�@�N���A
 *
 * \param
 * \return void
 */
void CT_KEYBOARD::ClearBuffer() noexcept
{
	ClearKeyBuffer();
	ClearCharBuffer();
}

/**
 * �L�[����
 *
 * \param keyCode
 * \return void
 */
void CT_KEYBOARD::KeyPressed(const unsigned char& keyCode) noexcept
{
	m_KeyStates.set(keyCode);									//��Ԋi�[
	m_KeyBuffer.emplace(ET_KEY_STATUS::me_Press, keyCode); //�L���[�v�b�V��
	TruncateBuffer(m_KeyBuffer);								//�o�b�t�@����Ǘ�
}

/**
 * �L�[����
 *
 * \param keyCode
 * \return void
 */
void CT_KEYBOARD::KeyReleased(const unsigned char& keyCode) noexcept
{
	m_KeyStates.reset(keyCode);
	m_KeyBuffer.emplace(ET_KEY_STATUS::me_Release, keyCode);
	TruncateBuffer(m_KeyBuffer);
}

/**
 * �e�L�X�g����
 *
 * \param character
 * \return void
 */
void CT_KEYBOARD::CharInput(const unsigned char& character) noexcept
{
	m_CharBuffer.push(character);		//�L���[�v�b�V��
	TruncateBuffer(m_CharBuffer);	//�o�b�t�@����Ǘ�
}

/**
 * �L�[��ԃ��Z�b�g
 *
 * \param
 * \return void
 */
void CT_KEYBOARD::ClearState() noexcept
{
	m_KeyStates.reset();
}
