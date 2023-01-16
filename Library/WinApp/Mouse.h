/**
 * @file Mouse.h
 * @brief マウス処理
 * @author 室谷イアン
 * @date 2022/05/03
 * @履歴 2022/05/03：クラス作成
 *		 2022/11/15：OS別処理への対応
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <utility>				//std::pair
#include <queue>				//FIFOコンテナ

//===== 構造体定義 =====
struct MOUSE_INFO				//マウス情報
{
	//変数宣言
	bool bLeftIsPressed;		//左クリック
	bool bRightIsPressed;		//右クリック
	int nPosX;
	int nPosY;					//マウス座標
	bool bIsInWindow;			//ウィンドウ内にいる

	//コンストラクタ
	MOUSE_INFO() noexcept :
		bLeftIsPressed(false), bRightIsPressed(false),
		nPosX(0), nPosY(0), bIsInWindow(false)
	{}
	MOUSE_INFO(const MOUSE_INFO& Parent) noexcept :
		bLeftIsPressed(Parent.bLeftIsPressed),
		bRightIsPressed(Parent.bRightIsPressed),
		nPosX(Parent.nPosX), nPosY(Parent.nPosY),
		bIsInWindow(Parent.bIsInWindow)
	{}
	~MOUSE_INFO() noexcept {}
};

//===== クラス定義 =====

//***** マウス状態 ****
enum class MOUSE_STATUS
{
	L_Press,
	L_Release,
	R_Press,
	R_Release,
	WheelUp,
	WheelDown,
	Move,
	EnterWindow,
	LeaveWindow,
	Invalid
};

//***** マウスイベント ****
class MOUSE_EVENTS
{
public:

	//プロトタイプ宣言
	explicit MOUSE_EVENTS() noexcept;
	MOUSE_EVENTS(MOUSE_STATUS Type, const MOUSE_INFO& Parent) noexcept;
	~MOUSE_EVENTS() noexcept;

	bool IsValid() const noexcept					//マウスイベント有効確認
	{
		return (m_Type != MOUSE_STATUS::Invalid) ? true : false;
	}
	MOUSE_STATUS GetType() const noexcept			//マウス状態取得
	{
		return m_Type;
	}
	std::pair<int, int> GetPos() const noexcept		//マウスXY座標取得
	{
		return{ m_Info.nPosX, m_Info.nPosY };
	}
	int GetPosX() const noexcept
	{
		return m_Info.nPosX;
	}
	int GetPosY() const noexcept
	{
		return m_Info.nPosY;
	}
	bool IsInWindow() const noexcept				//ウィンドウ内にいるか確認
	{
		return m_Info.bIsInWindow;
	}
	bool LeftIsPressed() const noexcept				//マウス左右クリック確認
	{
		return m_Info.bLeftIsPressed;
	}
	bool RightIsPressed() const noexcept
	{
		return m_Info.bRightIsPressed;
	}

private:

	//変数宣言
	MOUSE_STATUS m_Type;	//マウスの状態
	MOUSE_INFO m_Info;		//マウス情報
};

//***** マウス処理 *****
class MOUSE
{
public:

	//RawInput用構造体（マウス変化量）
	struct RAW_DELTA
	{
		int x;
		int y;
		bool bClear;	//NULLデータであることを示すフラグ

		RAW_DELTA(bool bClr = false) noexcept : x(0), y(0), bClear(bClr)
		{}
		RAW_DELTA(int xIn, int yIn, bool bClr = false) noexcept :
			x(xIn), y(yIn), bClear(bClr)
		{}
		~RAW_DELTA() noexcept
		{}
	};

	//プロトタイプ宣言
	explicit MOUSE() noexcept;
	~MOUSE() noexcept;
	std::pair<int, int> GetPos() const noexcept;	//マウス座標取得
	int GetPosX() const noexcept;					//マウスX座標取得
	int GetPosY() const noexcept;					//マウスY座標取得
	int GetWheelVal() const noexcept;				//ホイール値取得
	bool IsInWindow() const noexcept;				//ウィンドウ内にいるか確認
	bool LeftIsPressed() const noexcept;			//マウス左クリック確認
	bool RightIsPressed() const noexcept;			//マウス右クリック確認
	MOUSE_EVENTS ReadBuffer() noexcept;				//バッファ読込み
	bool IsEmpty() const noexcept;					//バッファの空き確認
	void ClearBuffer() noexcept;					//バッファクリア
	RAW_DELTA ReadRawDelta() noexcept;				//RawInputバッファ読込み
	void SetRawInput(bool bUse) noexcept;			//RawInput使用制御
	bool IsUsingRawInput() const noexcept;			//RawInput使用状態確認

private:

	//変数宣言
	static constexpr unsigned int nBufferSize = 16u;	//キューのサイズ（フレームごとの最大受付数）
	std::queue<MOUSE_EVENTS> m_Buffer;					//マウスイベント用キュー
	MOUSE_INFO m_Info;									//マウス情報
	int m_WheelDelta;									//ホイール操作量
	int m_WheelVal;										//ホイール値
	std::queue<RAW_DELTA> m_RawDeltaBuffer;				//RawInput用キュー
	bool m_bUseRawInput;								//RawInput使用フラグ

	//プロトタイプ宣言
	void TruncateBuffer() noexcept;					//バッファ切り捨て
	void MouseMove(int PosX, int PosY) noexcept;	//マウス移動
	void LeaveWindow() noexcept;					//ウィンドウ外に行く
	void Enterwindow() noexcept;					//ウィンドウ内に入る
	void LeftPressed() noexcept;					//左クリックオン
	void LeftReleased() noexcept;					//左クリックオフ
	void RightPressed() noexcept;					//右クリックオン
	void RightReleased() noexcept;					//右クリックオフ
	void WheelUp() noexcept;						//ホイールアップ
	void WheelDown() noexcept;						//ホイールダウン
	void WheelProc(int nDelta) noexcept;			//ホイール処理
	void TruncateRawInputBuffer() noexcept;			//RawInputバッファ切り捨て
	void GetRawDelta(int dx, int dy) noexcept;		//RawInput情報取得

	//権限指定
	friend class WIN_WINDOW;
};
