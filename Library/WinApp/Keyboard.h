/**
 * @file Keyboard.h
 * @brief キーボード処理
 * @author 室谷イアン
 * @date 2022/05/02
 * @履歴 2022/05/02：クラス作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <bitset>				//ビット集合クラス
#include <queue>				//FIFOコンテナ

//===== クラス定義 =====

//***** キー状態 *****
enum class KEY_STATUS
{
	Press,
	Release,
	Invalid
};

//***** キーイベント *****
class KEY_EVENTS
{
public:

	//プロトタイプ宣言
	explicit KEY_EVENTS() noexcept;
	KEY_EVENTS(KEY_STATUS Type, unsigned char Code) noexcept;
	~KEY_EVENTS() noexcept;

	bool IsPress() const noexcept								//キー押し確認
	{
		return (m_Type == KEY_STATUS::Press) ? true : false;
	}

	bool IsRelease() const noexcept								//キー離し確認
	{
		return (m_Type == KEY_STATUS::Release) ? true : false;
	}

	bool IsValid() const noexcept								//キーイベント有効確認
	{
		return (m_Type == KEY_STATUS::Invalid) ? true : false;
	}

	unsigned char GetCode() const noexcept						//キーコード取得
	{
		return m_Code;
	}

private:

	//変数宣言
	KEY_STATUS m_Type;		//キーの状態
	unsigned char m_Code;	//仮想キーコードの値
};

//***** キーボード処理 *****
class KEYBOARD
{
public:

	//プロトタイプ宣言
	explicit KEYBOARD() noexcept;
	~KEYBOARD() noexcept;
	bool KeyIsPressed(unsigned char KeyCode) const noexcept;	//キー押し確認
	KEY_EVENTS ReadKey() noexcept;								//キー読込み
	unsigned char ReadChar() noexcept;							//テキスト読込み
	bool KeyIsEmpty() const noexcept;							//キーバッファの空き確認
	bool CharIsEmpty() const noexcept;							//テキストバッファの空き確認
	void ClearKeyBuffer() noexcept;								//キーバッファクリア
	void ClearCharBuffer() noexcept;							//テキストバッファクリア
	void ClearBuffer() noexcept;								//全バッファクリア

private:

	//変数宣言
	static constexpr unsigned int nKeys = 256u;			//キー数
	static constexpr unsigned int nBufferSize = 16u;	//キューのサイズ（フレームごとの最大受付数）
	std::bitset<nKeys> m_KeyStates;						//256個のキーに対応するビット集合
	std::queue<KEY_EVENTS> m_KeyBuffer;					//キーイベント用キュー
	std::queue<unsigned char> m_CharBuffer;				//テキスト入力用キュー

	//プロトタイプ宣言
	template<typename T>
	static void TruncateBuffer(std::queue<T>& Buffer) noexcept		//バッファ切り捨て
	{
		while (Buffer.size() > nBufferSize)							//上限サイズに収まるまで
			Buffer.pop();											//キューポップ
	}

	void KeyPressed(unsigned char KeyCode) noexcept;				//キー押し
	void KeyReleased(unsigned char KeyCode) noexcept;				//キー離し
	void CharInput(unsigned char Character) noexcept;				//テキスト入力
	void ClearState() noexcept;										//キー状態リセット

	//権限指定
	friend class WIN_WINDOW;
};
