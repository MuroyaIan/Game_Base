
//===== �C���N���[�h�� =====
#include <WinApp/Keyboard.h>

//===== �N���X���� =====

//***** �L�[�C�x���g *****
KEY_EVENTS::KEY_EVENTS() noexcept : m_Type(KEY_STATUS::Invalid), m_Code(0u)
{
}

KEY_EVENTS::KEY_EVENTS(KEY_STATUS Type, unsigned char Code) noexcept :
	m_Type(Type), m_Code(Code)
{
}

KEY_EVENTS::~KEY_EVENTS() noexcept
{
}

//***** �L�[�{�[�h���� *****
KEYBOARD::KEYBOARD() noexcept : m_KeyStates(0ul), m_KeyBuffer(), m_CharBuffer()
{
}

KEYBOARD::~KEYBOARD() noexcept
{
}

//�L�[�����m�F
bool KEYBOARD::KeyIsPressed(unsigned char KeyCode) const noexcept
{
	return m_KeyStates[KeyCode];
}

//�L�[�Ǎ���
KEY_EVENTS KEYBOARD::ReadKey() noexcept
{
	if (m_KeyBuffer.size() > 0u) {
		KEY_EVENTS Event = m_KeyBuffer.front();		//���̃C�x���g�ɃA�N�Z�X
		m_KeyBuffer.pop();							//�C�x���g���폜����
		return Event;
	}
	else
		return KEY_EVENTS{};
}

//�e�L�X�g�Ǎ���
unsigned char KEYBOARD::ReadChar() noexcept
{
	if (m_CharBuffer.size() > 0u) {
		unsigned char CharCode = m_CharBuffer.front();
		m_CharBuffer.pop();
		return CharCode;
	}
	else
		return 255u;
}

//�L�[�o�b�t�@�̋󂫊m�F
bool KEYBOARD::KeyIsEmpty() const noexcept
{
	return m_KeyBuffer.empty();
}

//�e�L�X�g�o�b�t�@�̋󂫊m�F
bool KEYBOARD::CharIsEmpty() const noexcept
{
	return m_CharBuffer.empty();
}

//�L�[�o�b�t�@�N���A
void KEYBOARD::ClearKeyBuffer() noexcept
{
	m_KeyBuffer = std::queue<KEY_EVENTS>{};
}

//�e�L�X�g�o�b�t�@�N���A
void KEYBOARD::ClearCharBuffer() noexcept
{
	m_CharBuffer = std::queue<unsigned char>{};
}

//�S�o�b�t�@�N���A
void KEYBOARD::ClearBuffer() noexcept
{
	ClearKeyBuffer();
	ClearCharBuffer();
}

//�L�[����
void KEYBOARD::KeyPressed(unsigned char KeyCode) noexcept
{
	m_KeyStates.set(KeyCode);									//��Ԋi�[
	m_KeyBuffer.push(KEY_EVENTS(KEY_STATUS::Press, KeyCode));	//�L���[�v�b�V��
	TruncateBuffer(m_KeyBuffer);								//�o�b�t�@����Ǘ�
}

//�L�[����
void KEYBOARD::KeyReleased(unsigned char KeyCode) noexcept
{
	m_KeyStates.reset(KeyCode);
	m_KeyBuffer.push(KEY_EVENTS(KEY_STATUS::Release, KeyCode));
	TruncateBuffer(m_KeyBuffer);
}

//�e�L�X�g����
void KEYBOARD::CharInput(unsigned char Character) noexcept
{
	m_CharBuffer.push(Character);	//�L���[�v�b�V��
	TruncateBuffer(m_CharBuffer);	//�o�b�t�@����Ǘ�
}

//�L�[��ԃ��Z�b�g
void KEYBOARD::ClearState() noexcept
{
	m_KeyStates.reset();
}
