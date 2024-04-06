//===== インクルード部 =====
#include <WinApp/Keyboard.h>

//===== クラス実装 =====

//***** キーイベント *****
CT_KEY_EVENTS::CT_KEY_EVENTS() noexcept
	: m_Type(ET_KEY_STATUS::me_Invalid)
	, m_Code(0u) {}

CT_KEY_EVENTS::CT_KEY_EVENTS(const ET_KEY_STATUS& type, const unsigned char& code) noexcept
	: m_Type(type)
	, m_Code(code) {}

CT_KEY_EVENTS::~CT_KEY_EVENTS() noexcept = default;

//***** キーボード処理 *****
CT_KEYBOARD::CT_KEYBOARD() noexcept
	: m_KeyStates(0ull) {}

CT_KEYBOARD::~CT_KEYBOARD() noexcept = default;

/**
 * キー押し確認
 *
 * \param keyCode
 * \return bool
 */
bool CT_KEYBOARD::KeyIsPressed(const unsigned char& keyCode) const noexcept
{
	return m_KeyStates[keyCode];
}

/**
 * キー読込み
 *
 * \param
 * \return CT_KEY_EVENTS
 */
CT_KEY_EVENTS CT_KEYBOARD::ReadKey() noexcept
{
	if (!m_KeyBuffer.empty())
	{
		CT_KEY_EVENTS l_Event = m_KeyBuffer.front(); //次のイベントにアクセス
		m_KeyBuffer.pop();                           //イベントを削除する
		return l_Event;
	}

	return CT_KEY_EVENTS{};
}

/**
 * テキスト読込み
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
 * キーバッファの空き確認
 *
 * \param
 * \return bool
 */
bool CT_KEYBOARD::KeyIsEmpty() const noexcept
{
	return m_KeyBuffer.empty();
}

/**
 * テキストバッファの空き確認
 *
 * \param
 * \return bool
 */
bool CT_KEYBOARD::CharIsEmpty() const noexcept
{
	return m_CharBuffer.empty();
}

/**
 * キーバッファクリア
 *
 * \param
 * \return void
 */
void CT_KEYBOARD::ClearKeyBuffer() noexcept
{
	m_KeyBuffer = std::queue<CT_KEY_EVENTS>{};
}

/**
 * テキストバッファクリア
 *
 * \param
 * \return void
 */
void CT_KEYBOARD::ClearCharBuffer() noexcept
{
	m_CharBuffer = std::queue<unsigned char>{};
}

/**
 * 全バッファクリア
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
 * キー押し
 *
 * \param keyCode
 * \return void
 */
void CT_KEYBOARD::KeyPressed(const unsigned char& keyCode) noexcept
{
	m_KeyStates.set(keyCode);									//状態格納
	m_KeyBuffer.emplace(ET_KEY_STATUS::me_Press, keyCode); //キュープッシュ
	TruncateBuffer(m_KeyBuffer);								//バッファ上限管理
}

/**
 * キー離し
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
 * テキスト入力
 *
 * \param character
 * \return void
 */
void CT_KEYBOARD::CharInput(const unsigned char& character) noexcept
{
	m_CharBuffer.push(character);		//キュープッシュ
	TruncateBuffer(m_CharBuffer);	//バッファ上限管理
}

/**
 * キー状態リセット
 *
 * \param
 * \return void
 */
void CT_KEYBOARD::ClearState() noexcept
{
	m_KeyStates.reset();
}
