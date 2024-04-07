/**
 * @file Keyboard.h
 * @brief キーボード処理
 * @author 室谷イアン
 * @date 2022/05/02
 * @履歴 2022/05/02：クラス作成
 *		 2024/04/04：書式改善
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <bitset>				//ビット集合クラス
#include <queue>				//FIFOコンテナ

//===== クラス定義 =====

//***** キー状態 *****
enum class ET_KEY_STATUS
{
	me_Press,
	me_Release,
	me_Invalid
};

//***** キーイベント *****
class CT_KEY_EVENTS
{
public:

	//コピー＆ムーブ
	CT_KEY_EVENTS(const CT_KEY_EVENTS&) = default;
	CT_KEY_EVENTS& operator =(const CT_KEY_EVENTS&) = default;
	CT_KEY_EVENTS(CT_KEY_EVENTS&&) noexcept = default;
	CT_KEY_EVENTS& operator=(CT_KEY_EVENTS&&) noexcept = default;

	//プロトタイプ宣言
	explicit CT_KEY_EVENTS() noexcept;
	CT_KEY_EVENTS(const ET_KEY_STATUS& type, const unsigned char& code) noexcept;
	~CT_KEY_EVENTS() noexcept;

	[[nodiscard]] bool IsPress() const noexcept //キー押し確認
	{
		return m_Type == ET_KEY_STATUS::me_Press;
	}

	[[nodiscard]] bool IsRelease() const noexcept //キー離し確認
	{
		return m_Type == ET_KEY_STATUS::me_Release;
	}

	[[nodiscard]] bool IsValid() const noexcept //キーイベント有効確認
	{
		return m_Type == ET_KEY_STATUS::me_Invalid;
	}

	[[nodiscard]] unsigned char GetCode() const noexcept //キーコード取得
	{
		return m_Code;
	}

private:

	//変数宣言
	ET_KEY_STATUS m_Type; //キーの状態
	unsigned char m_Code; //仮想キーコードの値
};

//***** キーボード処理 *****
class CT_KEYBOARD
{
public:

	//コピー＆ムーブ
	CT_KEYBOARD(const CT_KEYBOARD&) = default;
	CT_KEYBOARD& operator =(const CT_KEYBOARD&) = default;
	CT_KEYBOARD(CT_KEYBOARD&&) noexcept = default;
	CT_KEYBOARD& operator=(CT_KEYBOARD&&) noexcept = default;

	//プロトタイプ宣言
	explicit CT_KEYBOARD() noexcept;
	~CT_KEYBOARD() noexcept;
	[[nodiscard]] bool KeyIsPressed(const unsigned char& keyCode) const noexcept; //キー押し確認
	CT_KEY_EVENTS ReadKey() noexcept;                                             //キー読込み
	unsigned char ReadChar() noexcept;                                            //テキスト読込み
	[[nodiscard]] bool KeyIsEmpty() const noexcept;                               //キーバッファの空き確認
	[[nodiscard]] bool CharIsEmpty() const noexcept;                              //テキストバッファの空き確認
	void ClearKeyBuffer() noexcept;                                               //キーバッファクリア
	void ClearCharBuffer() noexcept;                                              //テキストバッファクリア
	void ClearBuffer() noexcept;                                                  //全バッファクリア

private:

	//変数宣言
	static constexpr unsigned int c_Keys = 256u;      //キー数
	static constexpr unsigned int c_BufferSize = 16u; //キューのサイズ（フレームごとの最大受付数）
	std::bitset<c_Keys> m_KeyStates;                  //256個のキーに対応するビット集合
	std::queue<CT_KEY_EVENTS> m_KeyBuffer;            //キーイベント用キュー
	std::queue<unsigned char> m_CharBuffer;           //テキスト入力用キュー

	//プロトタイプ宣言
	template<typename t_Buffer>
	static void TruncateBuffer(std::queue<t_Buffer>& buffer) noexcept	//バッファ切り捨て
	{
		while (buffer.size() > c_BufferSize)							//上限サイズに収まるまで
			buffer.pop();												//キューポップ
	}

	void KeyPressed(const unsigned char& keyCode) noexcept;  //キー押し
	void KeyReleased(const unsigned char& keyCode) noexcept; //キー離し
	void CharInput(const unsigned char& character) noexcept; //テキスト入力
	void ClearState() noexcept;                              //キー状態リセット

	//権限指定
	friend class CT_IW_WIN;
};
