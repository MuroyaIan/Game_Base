/**
 * @file Keyboard.h
 * @brief �L�[�{�[�h����
 * @author ���J�C�A��
 * @date 2022/05/02
 * @���� 2022/05/02�F�N���X�쐬
 *		 2024/04/04�F�������P
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <bitset>				//�r�b�g�W���N���X
#include <queue>				//FIFO�R���e�i

//===== �N���X��` =====

//***** �L�[��� *****
enum class ET_KEY_STATUS
{
	me_Press,
	me_Release,
	me_Invalid
};

//***** �L�[�C�x���g *****
class CT_KEY_EVENTS
{
public:

	//�R�s�[�����[�u
	CT_KEY_EVENTS(const CT_KEY_EVENTS&) = default;
	CT_KEY_EVENTS& operator =(const CT_KEY_EVENTS&) = default;
	CT_KEY_EVENTS(CT_KEY_EVENTS&&) noexcept = default;
	CT_KEY_EVENTS& operator=(CT_KEY_EVENTS&&) noexcept = default;

	//�v���g�^�C�v�錾
	explicit CT_KEY_EVENTS() noexcept;
	CT_KEY_EVENTS(const ET_KEY_STATUS& type, const unsigned char& code) noexcept;
	~CT_KEY_EVENTS() noexcept;

	[[nodiscard]] bool IsPress() const noexcept //�L�[�����m�F
	{
		return m_Type == ET_KEY_STATUS::me_Press;
	}

	[[nodiscard]] bool IsRelease() const noexcept //�L�[�����m�F
	{
		return m_Type == ET_KEY_STATUS::me_Release;
	}

	[[nodiscard]] bool IsValid() const noexcept //�L�[�C�x���g�L���m�F
	{
		return m_Type == ET_KEY_STATUS::me_Invalid;
	}

	[[nodiscard]] unsigned char GetCode() const noexcept //�L�[�R�[�h�擾
	{
		return m_Code;
	}

private:

	//�ϐ��錾
	ET_KEY_STATUS m_Type; //�L�[�̏��
	unsigned char m_Code; //���z�L�[�R�[�h�̒l
};

//***** �L�[�{�[�h���� *****
class CT_KEYBOARD
{
public:

	//�R�s�[�����[�u
	CT_KEYBOARD(const CT_KEYBOARD&) = default;
	CT_KEYBOARD& operator =(const CT_KEYBOARD&) = default;
	CT_KEYBOARD(CT_KEYBOARD&&) noexcept = default;
	CT_KEYBOARD& operator=(CT_KEYBOARD&&) noexcept = default;

	//�v���g�^�C�v�錾
	explicit CT_KEYBOARD() noexcept;
	~CT_KEYBOARD() noexcept;
	[[nodiscard]] bool KeyIsPressed(const unsigned char& keyCode) const noexcept; //�L�[�����m�F
	CT_KEY_EVENTS ReadKey() noexcept;                                             //�L�[�Ǎ���
	unsigned char ReadChar() noexcept;                                            //�e�L�X�g�Ǎ���
	[[nodiscard]] bool KeyIsEmpty() const noexcept;                               //�L�[�o�b�t�@�̋󂫊m�F
	[[nodiscard]] bool CharIsEmpty() const noexcept;                              //�e�L�X�g�o�b�t�@�̋󂫊m�F
	void ClearKeyBuffer() noexcept;                                               //�L�[�o�b�t�@�N���A
	void ClearCharBuffer() noexcept;                                              //�e�L�X�g�o�b�t�@�N���A
	void ClearBuffer() noexcept;                                                  //�S�o�b�t�@�N���A

private:

	//�ϐ��錾
	static constexpr unsigned int c_Keys = 256u;      //�L�[��
	static constexpr unsigned int c_BufferSize = 16u; //�L���[�̃T�C�Y�i�t���[�����Ƃ̍ő��t���j
	std::bitset<c_Keys> m_KeyStates;                  //256�̃L�[�ɑΉ�����r�b�g�W��
	std::queue<CT_KEY_EVENTS> m_KeyBuffer;            //�L�[�C�x���g�p�L���[
	std::queue<unsigned char> m_CharBuffer;           //�e�L�X�g���͗p�L���[

	//�v���g�^�C�v�錾
	template<typename t_Buffer>
	static void TruncateBuffer(std::queue<t_Buffer>& buffer) noexcept	//�o�b�t�@�؂�̂�
	{
		while (buffer.size() > c_BufferSize)							//����T�C�Y�Ɏ��܂�܂�
			buffer.pop();												//�L���[�|�b�v
	}

	void KeyPressed(const unsigned char& keyCode) noexcept;  //�L�[����
	void KeyReleased(const unsigned char& keyCode) noexcept; //�L�[����
	void CharInput(const unsigned char& character) noexcept; //�e�L�X�g����
	void ClearState() noexcept;                              //�L�[��ԃ��Z�b�g

	//�����w��
	friend class CT_IW_WIN;
};
