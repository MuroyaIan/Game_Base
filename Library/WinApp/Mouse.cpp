//===== インクルード部 =====
#include <WinApp/Mouse.h>

#ifdef _WIN64

#include <WinApp/WinConfig.h>

#else

#define WHEEL_DELTA (1)

#endif // _WIN64

//===== クラス実装 =====

//***** マウスイベント ****
CT_MOUSE_EVENTS::CT_MOUSE_EVENTS() noexcept
	: m_Type(ET_MOUSE_STATUS::me_Invalid)
	, m_Info() {}

CT_MOUSE_EVENTS::CT_MOUSE_EVENTS(const ET_MOUSE_STATUS& type, const ST_MOUSE_INFO& parent) noexcept
	: m_Type(type)
	, m_Info(parent) {}

CT_MOUSE_EVENTS::~CT_MOUSE_EVENTS() noexcept = default;

//***** マウス処理 *****
CT_MOUSE::CT_MOUSE() noexcept
	: m_Buffer()
	, m_Info()
	, m_WheelDelta(0)
	, m_WheelVal(0)
	, m_RawDeltaBuffer()
	, m_bUseRawInput(false) {}

CT_MOUSE::~CT_MOUSE() noexcept = default;

/**
 * マウスXY座標取得
 *
 * \param
 * \return std::pair<int, int>
 */
std::pair<int, int> CT_MOUSE::GetPos() const noexcept
{
	return {m_Info.ms_PosX, m_Info.ms_PosY};
}

/**
 * マウスX座標取得
 *
 * \param
 * \return int
 */
int CT_MOUSE::GetPosX() const noexcept
{
	return m_Info.ms_PosX;
}

/**
 * マウスY座標取得
 *
 * \param
 * \return int
 */
int CT_MOUSE::GetPosY() const noexcept
{
	return m_Info.ms_PosY;
}

/**
 * ホイール値取得
 *
 * \param
 * \return int
 */
int CT_MOUSE::GetWheelVal() const noexcept
{
	return m_WheelVal;
}

/**
 * ウィンドウ内にいるか確認
 *
 * \param
 * \return bool
 */
bool CT_MOUSE::IsInWindow() const noexcept
{
	return m_Info.ms_bIsInWindow;
}

/**
 * マウス左クリック確認
 *
 * \param
 * \return bool
 */
bool CT_MOUSE::LeftIsPressed() const noexcept
{
	return m_Info.ms_bLeftIsPressed;
}

/**
 * マウス右クリック確認
 *
 * \param
 * \return bool
 */
bool CT_MOUSE::RightIsPressed() const noexcept
{
	return m_Info.ms_bRightIsPressed;
}

/**
 * バッファ読込み
 *
 * \param
 * \return CT_MOUSE_EVENTS
 */
CT_MOUSE_EVENTS CT_MOUSE::ReadBuffer() noexcept
{
	//例外処理
	if (m_Buffer.empty())
		return CT_MOUSE_EVENTS{};

	const CT_MOUSE_EVENTS l_Event = m_Buffer.front(); //次のイベントにアクセス
	m_Buffer.pop();                                   //イベントを削除する
	return l_Event;
}

/**
 * バッファの空き確認
 *
 * \param
 * \return bool
 */
bool CT_MOUSE::IsEmpty() const noexcept
{
	return m_Buffer.empty();
}

/**
 * バッファクリア
 *
 * \param
 * \return void
 */
void CT_MOUSE::ClearBuffer() noexcept
{
	m_Buffer = std::queue<CT_MOUSE_EVENTS>{};
}

/**
 * RawInputバッファ読込み
 *
 * \param
 * \return CT_MOUSE::ST_RAW_DELTA
 */
CT_MOUSE::ST_RAW_DELTA CT_MOUSE::ReadRawDelta() noexcept
{
	//例外処理(キューが空になった)
	if (m_RawDeltaBuffer.empty())
		return ST_RAW_DELTA{true};

	const ST_RAW_DELTA l_Data = m_RawDeltaBuffer.front(); //次のイベントにアクセス
	m_RawDeltaBuffer.pop();                               //イベントを削除する
	return l_Data;
}

/**
 * RawInput使用制御
 *
 * \param bUse
 * \return void
 */
void CT_MOUSE::SetRawInput(const bool& bUse) noexcept
{
	m_bUseRawInput = bUse;
}

/**
 * RawInput使用状態確認
 *
 * \param
 * \return void
 */
bool CT_MOUSE::IsUsingRawInput() const noexcept
{
	return m_bUseRawInput;
}

/**
 * バッファ切り捨て
 *
 * \param
 * \return void
 */
void CT_MOUSE::TruncateBuffer() noexcept
{
	while (m_Buffer.size() > c_BufferSize) //上限サイズに収まるまで
		m_Buffer.pop();                    //キューポップ
}

/**
 * マウス移動
 *
 * \param posX
 * \param posY
 * \return void
 */
void CT_MOUSE::MouseMove(const int& posX, const int& posY) noexcept
{
	m_Info.ms_PosX = posX;
	m_Info.ms_PosY = posY;										//座標格納
	m_Buffer.emplace(ET_MOUSE_STATUS::me_Move, m_Info);		//キュープッシュ
	TruncateBuffer();											//バッファ上限管理
}

/**
 * ウィンドウ外に行く
 *
 * \param
 * \return void
 */
void CT_MOUSE::LeaveWindow() noexcept
{
	m_Info.ms_bIsInWindow = false;
	m_Buffer.emplace(ET_MOUSE_STATUS::me_LeaveWindow, m_Info);
	TruncateBuffer();
}

/**
 * ウィンドウ内に入る
 *
 * \param
 * \return void
 */
void CT_MOUSE::EnterWindow() noexcept
{
	m_Info.ms_bIsInWindow = true;
	m_Buffer.emplace(ET_MOUSE_STATUS::me_EnterWindow, m_Info);
	TruncateBuffer();
}

/**
 * 左クリックオン
 *
 * \param
 * \return void
 */
void CT_MOUSE::LeftPressed() noexcept
{
	m_Info.ms_bLeftIsPressed = true;
	m_Buffer.emplace(ET_MOUSE_STATUS::me_L_Press, m_Info);
	TruncateBuffer();
}

/**
 * 左クリックオフ
 *
 * \param
 * \return void
 */
void CT_MOUSE::LeftReleased() noexcept
{
	m_Info.ms_bLeftIsPressed = false;
	m_Buffer.emplace(ET_MOUSE_STATUS::me_L_Release, m_Info);
	TruncateBuffer();
}

/**
 * 右クリックオン
 *
 * \param
 * \return void
 */
void CT_MOUSE::RightPressed() noexcept
{
	m_Info.ms_bRightIsPressed = true;
	m_Buffer.emplace(ET_MOUSE_STATUS::me_R_Press, m_Info);
	TruncateBuffer();
}

/**
 * 右クリックオフ
 *
 * \param
 * \return void
 */
void CT_MOUSE::RightReleased() noexcept
{
	m_Info.ms_bRightIsPressed = false;
	m_Buffer.emplace(ET_MOUSE_STATUS::me_R_Release, m_Info);
	TruncateBuffer();
}

/**
 * ホイールアップ
 *
 * \param
 * \return void
 */
void CT_MOUSE::WheelUp() noexcept
{
	m_Buffer.emplace(ET_MOUSE_STATUS::me_WheelUp, m_Info);
	TruncateBuffer();
	m_WheelVal++;
}

/**
 * ホイールダウン
 *
 * \param
 * \return void
 */
void CT_MOUSE::WheelDown() noexcept
{
	m_Buffer.emplace(ET_MOUSE_STATUS::me_WheelDown, m_Info);
	TruncateBuffer();
	m_WheelVal--;
}

/**
 * ホイール処理
 *
 * \param nDelta
 * \return void
 */
void CT_MOUSE::WheelProc(const int& nDelta) noexcept
{
	m_WheelDelta += nDelta;

	//ホイール応答精度制御(1回のメッセージで応答)
	static int l_NResponse = WHEEL_DELTA * 1;
	if (m_WheelDelta >= l_NResponse)
	{
		m_WheelDelta = 0;
		WheelUp();
	}
	if (m_WheelDelta <= -l_NResponse)
	{
		m_WheelDelta = 0;
		WheelDown();
	}
}

/**
 * RawInputバッファ切り捨て
 *
 * \param
 * \return void
 */
void CT_MOUSE::TruncateRawInputBuffer() noexcept
{
	while (m_RawDeltaBuffer.size() > c_BufferSize) //上限サイズに収まるまで
		m_RawDeltaBuffer.pop();                    //キューポップ
}

/**
 * RawInput情報取得
 *
 * \param dx
 * \param dy
 * \return void
 */
void CT_MOUSE::GetRawDelta(const int& dx, const int& dy) noexcept
{
	m_RawDeltaBuffer.emplace(dx, dy);
	TruncateRawInputBuffer();
}
