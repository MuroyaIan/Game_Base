/**
 * @file Keyboard.h
 * @brief �L�[�{�[�h����
 * @author ���J�C�A��
 * @date 2022/05/02
 * @���� 2022/05/02�F�N���X�쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <bitset>				//�r�b�g�W���N���X
#include <queue>				//FIFO�R���e�i

//===== �N���X��` =====

//***** �L�[��� *****
enum class KEY_STATUS
{
	Press,
	Release,
	Invalid
};

//***** �L�[�C�x���g *****
class KEY_EVENTS
{
public:

	//�v���g�^�C�v�錾
	explicit KEY_EVENTS() noexcept;
	KEY_EVENTS(KEY_STATUS Type, unsigned char Code) noexcept;
	~KEY_EVENTS() noexcept;

	bool IsPress() const noexcept								//�L�[�����m�F
	{
		return (m_Type == KEY_STATUS::Press) ? true : false;
	}

	bool IsRelease() const noexcept								//�L�[�����m�F
	{
		return (m_Type == KEY_STATUS::Release) ? true : false;
	}

	bool IsValid() const noexcept								//�L�[�C�x���g�L���m�F
	{
		return (m_Type == KEY_STATUS::Invalid) ? true : false;
	}

	unsigned char GetCode() const noexcept						//�L�[�R�[�h�擾
	{
		return m_Code;
	}

private:

	//�ϐ��錾
	KEY_STATUS m_Type;		//�L�[�̏��
	unsigned char m_Code;	//���z�L�[�R�[�h�̒l
};

//***** �L�[�{�[�h���� *****
class KEYBOARD
{
public:

	//�v���g�^�C�v�錾
	explicit KEYBOARD() noexcept;
	~KEYBOARD() noexcept;
	bool KeyIsPressed(unsigned char KeyCode) const noexcept;	//�L�[�����m�F
	KEY_EVENTS ReadKey() noexcept;								//�L�[�Ǎ���
	unsigned char ReadChar() noexcept;							//�e�L�X�g�Ǎ���
	bool KeyIsEmpty() const noexcept;							//�L�[�o�b�t�@�̋󂫊m�F
	bool CharIsEmpty() const noexcept;							//�e�L�X�g�o�b�t�@�̋󂫊m�F
	void ClearKeyBuffer() noexcept;								//�L�[�o�b�t�@�N���A
	void ClearCharBuffer() noexcept;							//�e�L�X�g�o�b�t�@�N���A
	void ClearBuffer() noexcept;								//�S�o�b�t�@�N���A

private:

	//�ϐ��錾
	static constexpr unsigned int nKeys = 256u;			//�L�[��
	static constexpr unsigned int nBufferSize = 16u;	//�L���[�̃T�C�Y�i�t���[�����Ƃ̍ő��t���j
	std::bitset<nKeys> m_KeyStates;						//256�̃L�[�ɑΉ�����r�b�g�W��
	std::queue<KEY_EVENTS> m_KeyBuffer;					//�L�[�C�x���g�p�L���[
	std::queue<unsigned char> m_CharBuffer;				//�e�L�X�g���͗p�L���[

	//�v���g�^�C�v�錾
	template<typename T>
	static void TruncateBuffer(std::queue<T>& Buffer) noexcept		//�o�b�t�@�؂�̂�
	{
		while (Buffer.size() > nBufferSize)							//����T�C�Y�Ɏ��܂�܂�
			Buffer.pop();											//�L���[�|�b�v
	}

	void KeyPressed(unsigned char KeyCode) noexcept;				//�L�[����
	void KeyReleased(unsigned char KeyCode) noexcept;				//�L�[����
	void CharInput(unsigned char Character) noexcept;				//�e�L�X�g����
	void ClearState() noexcept;										//�L�[��ԃ��Z�b�g

	//�����w��
	friend class WIN_WINDOW;
};
