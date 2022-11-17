
//===== インクルード部 =====
#include <WinApp/Keyboard.h>

//===== クラス実装 =====

//***** キーイベント *****
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

//***** キーボード処理 *****
KEYBOARD::KEYBOARD() noexcept : m_KeyStates(0ul), m_KeyBuffer(), m_CharBuffer()
{
}

KEYBOARD::~KEYBOARD() noexcept
{
}

//キー押し確認
bool KEYBOARD::KeyIsPressed(unsigned char KeyCode) const noexcept
{
	return m_KeyStates[KeyCode];
}

//キー読込み
KEY_EVENTS KEYBOARD::ReadKey() noexcept
{
	if (m_KeyBuffer.size() > 0u) {
		KEY_EVENTS Event = m_KeyBuffer.front();		//次のイベントにアクセス
		m_KeyBuffer.pop();							//イベントを削除する
		return Event;
	}
	else
		return KEY_EVENTS{};
}

//テキスト読込み
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

//キーバッファの空き確認
bool KEYBOARD::KeyIsEmpty() const noexcept
{
	return m_KeyBuffer.empty();
}

//テキストバッファの空き確認
bool KEYBOARD::CharIsEmpty() const noexcept
{
	return m_CharBuffer.empty();
}

//キーバッファクリア
void KEYBOARD::ClearKeyBuffer() noexcept
{
	m_KeyBuffer = std::queue<KEY_EVENTS>{};
}

//テキストバッファクリア
void KEYBOARD::ClearCharBuffer() noexcept
{
	m_CharBuffer = std::queue<unsigned char>{};
}

//全バッファクリア
void KEYBOARD::ClearBuffer() noexcept
{
	ClearKeyBuffer();
	ClearCharBuffer();
}

//キー押し
void KEYBOARD::KeyPressed(unsigned char KeyCode) noexcept
{
	m_KeyStates.set(KeyCode);									//状態格納
	m_KeyBuffer.push(KEY_EVENTS(KEY_STATUS::Press, KeyCode));	//キュープッシュ
	TruncateBuffer(m_KeyBuffer);								//バッファ上限管理
}

//キー離し
void KEYBOARD::KeyReleased(unsigned char KeyCode) noexcept
{
	m_KeyStates.reset(KeyCode);
	m_KeyBuffer.push(KEY_EVENTS(KEY_STATUS::Release, KeyCode));
	TruncateBuffer(m_KeyBuffer);
}

//テキスト入力
void KEYBOARD::CharInput(unsigned char Character) noexcept
{
	m_CharBuffer.push(Character);	//キュープッシュ
	TruncateBuffer(m_CharBuffer);	//バッファ上限管理
}

//キー状態リセット
void KEYBOARD::ClearState() noexcept
{
	m_KeyStates.reset();
}
