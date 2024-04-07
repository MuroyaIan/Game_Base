/**
 * @file Mouse.h
 * @brief マウス処理
 * @author 室谷イアン
 * @date 2022/05/03
 * @履歴 2022/05/03：クラス作成
 *		 2022/11/15：OS別処理への対応
 *		 2024/04/06：書式改善
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <utility>				//std::pair
#include <queue>				//FIFOコンテナ

//===== 構造体定義 =====

//***** マウス情報 *****
struct ST_MOUSE_INFO
{
	//コピー＆ムーブ
	ST_MOUSE_INFO(const ST_MOUSE_INFO&) = default;
	ST_MOUSE_INFO& operator =(const ST_MOUSE_INFO&) = default;
	ST_MOUSE_INFO(ST_MOUSE_INFO&&) noexcept = default;
	ST_MOUSE_INFO& operator=(ST_MOUSE_INFO&&) noexcept = default;

	//変数宣言
	bool ms_bLeftIsPressed;		//左クリック
	bool ms_bRightIsPressed;	//右クリック
	int ms_PosX;
	int ms_PosY;				//マウス座標
	bool ms_bIsInWindow;		//ウィンドウ内にいる

	//コンストラクタ
	explicit ST_MOUSE_INFO() noexcept
		: ms_bLeftIsPressed(false)
		, ms_bRightIsPressed(false)
		, ms_PosX(0)
		, ms_PosY(0)
		, ms_bIsInWindow(false) {}

	~ST_MOUSE_INFO() noexcept = default;
};

//===== クラス定義 =====

//***** マウス状態 ****
enum class ET_MOUSE_STATUS
{
	me_L_Press,
	me_L_Release,
	me_R_Press,
	me_R_Release,
	me_WheelUp,
	me_WheelDown,
	me_Move,
	me_EnterWindow,
	me_LeaveWindow,
	me_Invalid
};

//***** マウスイベント ****
class CT_MOUSE_EVENTS
{
public:

	//コピー＆ムーブ
	CT_MOUSE_EVENTS(const CT_MOUSE_EVENTS&) = default;
	CT_MOUSE_EVENTS& operator =(const CT_MOUSE_EVENTS&) = default;
	CT_MOUSE_EVENTS(CT_MOUSE_EVENTS&&) noexcept = default;
	CT_MOUSE_EVENTS& operator=(CT_MOUSE_EVENTS&&) noexcept = default;

	//プロトタイプ宣言
	explicit CT_MOUSE_EVENTS() noexcept;
	CT_MOUSE_EVENTS(const ET_MOUSE_STATUS& type, const ST_MOUSE_INFO& parent) noexcept;
	~CT_MOUSE_EVENTS() noexcept;

	[[nodiscard]] bool IsValid() const noexcept //マウスイベント有効確認
	{
		return m_Type != ET_MOUSE_STATUS::me_Invalid;
	}

	[[nodiscard]] ET_MOUSE_STATUS GetType() const noexcept //マウス状態取得
	{
		return m_Type;
	}

	[[nodiscard]] std::pair<int, int> GetPos() const noexcept //マウスXY座標取得
	{
		return {m_Info.ms_PosX, m_Info.ms_PosY};
	}

	[[nodiscard]] int GetPosX() const noexcept
	{
		return m_Info.ms_PosX;
	}

	[[nodiscard]] int GetPosY() const noexcept
	{
		return m_Info.ms_PosY;
	}

	[[nodiscard]] bool IsInWindow() const noexcept //ウィンドウ内にいるか確認
	{
		return m_Info.ms_bIsInWindow;
	}

	[[nodiscard]] bool LeftIsPressed() const noexcept //マウス左右クリック確認
	{
		return m_Info.ms_bLeftIsPressed;
	}

	[[nodiscard]] bool RightIsPressed() const noexcept
	{
		return m_Info.ms_bRightIsPressed;
	}

private:

	//変数宣言
	ET_MOUSE_STATUS m_Type; //マウスの状態
	ST_MOUSE_INFO m_Info;   //マウス情報
};

//***** マウス処理 *****
class CT_MOUSE
{
public:

	//RawInput用構造体（マウス変化量）
	struct ST_RAW_DELTA
	{
		//コピー＆ムーブ
		ST_RAW_DELTA(const ST_RAW_DELTA&) = default;
		ST_RAW_DELTA& operator =(const ST_RAW_DELTA&) = default;
		ST_RAW_DELTA(ST_RAW_DELTA&&) noexcept = default;
		ST_RAW_DELTA& operator=(ST_RAW_DELTA&&) noexcept = default;

		//変数宣言
		int ms_X;
		int ms_Y;
		bool ms_bClear; //NULLデータであることを示すフラグ

		//コンストラクタ
		explicit ST_RAW_DELTA(const bool& bClr = false) noexcept
			: ms_X(0)
			, ms_Y(0)
			, ms_bClear(bClr) {}

		ST_RAW_DELTA(const int& xIn, const int& yIn, const bool& bClr = false) noexcept
			: ms_X(xIn)
			, ms_Y(yIn)
			, ms_bClear(bClr) {}

		~ST_RAW_DELTA() noexcept = default;
	};

	//コピー＆ムーブ
	CT_MOUSE(const CT_MOUSE&) = default;
	CT_MOUSE& operator =(const CT_MOUSE&) = default;
	CT_MOUSE(CT_MOUSE&&) noexcept = default;
	CT_MOUSE& operator=(CT_MOUSE&&) noexcept = default;

	//プロトタイプ宣言
	explicit CT_MOUSE() noexcept;
	~CT_MOUSE() noexcept;
	[[nodiscard]] std::pair<int, int> GetPos() const noexcept; //マウス座標取得
	[[nodiscard]] int GetPosX() const noexcept;                //マウスX座標取得
	[[nodiscard]] int GetPosY() const noexcept;                //マウスY座標取得
	[[nodiscard]] int GetWheelVal() const noexcept;            //ホイール値取得
	[[nodiscard]] bool IsInWindow() const noexcept;            //ウィンドウ内にいるか確認
	[[nodiscard]] bool LeftIsPressed() const noexcept;         //マウス左クリック確認
	[[nodiscard]] bool RightIsPressed() const noexcept;        //マウス右クリック確認
	CT_MOUSE_EVENTS ReadBuffer() noexcept;                     //バッファ読込み
	[[nodiscard]] bool IsEmpty() const noexcept;               //バッファの空き確認
	void ClearBuffer() noexcept;                               //バッファクリア
	ST_RAW_DELTA ReadRawDelta() noexcept;                      //RawInputバッファ読込み
	void SetRawInput(const bool& bUse) noexcept;               //RawInput使用制御
	[[nodiscard]] bool IsUsingRawInput() const noexcept;       //RawInput使用状態確認

private:

	//変数宣言
	static constexpr unsigned int c_BufferSize = 16u; //キューのサイズ（フレームごとの最大受付数）
	std::queue<CT_MOUSE_EVENTS> m_Buffer;             //マウスイベント用キュー
	ST_MOUSE_INFO m_Info;                             //マウス情報
	int m_WheelDelta;                                 //ホイール操作量
	int m_WheelVal;                                   //ホイール値
	std::queue<ST_RAW_DELTA> m_RawDeltaBuffer;        //RawInput用キュー
	bool m_bUseRawInput;                              //RawInput使用フラグ

	//プロトタイプ宣言
	void TruncateBuffer() noexcept;                            //バッファ切り捨て
	void MouseMove(const int& posX, const int& posY) noexcept; //マウス移動
	void LeaveWindow() noexcept;                               //ウィンドウ外に行く
	void EnterWindow() noexcept;                               //ウィンドウ内に入る
	void LeftPressed() noexcept;                               //左クリックオン
	void LeftReleased() noexcept;                              //左クリックオフ
	void RightPressed() noexcept;                              //右クリックオン
	void RightReleased() noexcept;                             //右クリックオフ
	void WheelUp() noexcept;                                   //ホイールアップ
	void WheelDown() noexcept;                                 //ホイールダウン
	void WheelProc(const int& nDelta) noexcept;                //ホイール処理
	void TruncateRawInputBuffer() noexcept;                    //RawInputバッファ切り捨て
	void GetRawDelta(const int& dx, const int& dy) noexcept;   //RawInput情報取得

	//権限指定
	friend class CT_IW_WIN;
};
