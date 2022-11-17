
//===== インクルード部 =====
#include <WinApp/Mouse.h>

#ifdef _WIN64

#include <WinApp/WinConfig.h>

#else

#define WHEEL_DELTA (1)

#endif // _WIN64

//===== クラス実装 =====

//***** マウスイベント ****
MOUSE_EVENTS::MOUSE_EVENTS() noexcept : m_Type(MOUSE_STATUS::Invalid), m_Info()
{
}

MOUSE_EVENTS::MOUSE_EVENTS(MOUSE_STATUS Type, const MOUSE_INFO& Parent) noexcept :
	m_Type(Type), m_Info(Parent)
{
}

MOUSE_EVENTS::~MOUSE_EVENTS() noexcept
{
}

//***** マウス処理 *****
MOUSE::MOUSE() noexcept : m_Buffer(), m_Info(), m_WheelDelta(0), m_WheelVal(0),
	m_RawDeltaBuffer(), m_bUseRawInput(false)
{
}

MOUSE::~MOUSE() noexcept
{
}

//マウスXY座標取得
std::pair<int, int> MOUSE::GetPos() const noexcept
{
	return { m_Info.nPosX, m_Info.nPosY };
}

int MOUSE::GetPosX() const noexcept
{
	return m_Info.nPosX;
}

int MOUSE::GetPosY() const noexcept
{
	return m_Info.nPosY;
}

//ホイール値取得
int MOUSE::GetWheelVal() const noexcept
{
	return m_WheelVal;
}

//ウィンドウ内にいるか確認
bool MOUSE::IsInWindow() const noexcept
{
	return m_Info.bIsInWindow;
}

//マウス左クリック確認
bool MOUSE::LeftIsPressed() const noexcept
{
	return m_Info.bLeftIsPressed;
}

//マウス右クリック確認
bool MOUSE::RightIsPressed() const noexcept
{
	return m_Info.bRightIsPressed;
}

//バッファ読込み
MOUSE_EVENTS MOUSE::ReadBuffer() noexcept
{
	//例外処理
	if (m_Buffer.empty())
		return MOUSE_EVENTS{};

	const MOUSE_EVENTS Event = m_Buffer.front();	//次のイベントにアクセス
	m_Buffer.pop();									//イベントを削除する
	return Event;
}

//バッファの空き確認
bool MOUSE::IsEmpty() const noexcept
{
	return m_Buffer.empty();
}

//バッファクリア
void MOUSE::ClearBuffer() noexcept
{
	m_Buffer = std::queue<MOUSE_EVENTS>{};
}

//RawInputバッファ読込み
MOUSE::RAW_DELTA MOUSE::ReadRawDelta() noexcept
{
	//例外処理
	if (m_RawDeltaBuffer.empty())
		return RAW_DELTA{};

	const RAW_DELTA Data = m_RawDeltaBuffer.front();	//次のイベントにアクセス
	m_RawDeltaBuffer.pop();								//イベントを削除する
	return Data;
}

//RawInput使用制御
void MOUSE::SetRawInput(bool bUse) noexcept
{
	m_bUseRawInput = bUse;
}

//RawInput使用状態確認
bool MOUSE::IsUsingRawInput() const noexcept
{
	return m_bUseRawInput;
}

//バッファ切り捨て
void MOUSE::TruncateBuffer() noexcept
{
	while (m_Buffer.size() > nBufferSize)	//上限サイズに収まるまで
		m_Buffer.pop();						//キューポップ
}

//マウス移動
void MOUSE::MouseMove(int PosX, int PosY) noexcept
{
	m_Info.nPosX = PosX;
	m_Info.nPosY = PosY;							//座標格納
	m_Buffer.push({ MOUSE_STATUS::Move, m_Info });	//キュープッシュ
	TruncateBuffer();								//バッファ上限管理
}

//ウィンドウ外に行く
void MOUSE::LeaveWindow() noexcept
{
	m_Info.bIsInWindow = false;
	m_Buffer.push({ MOUSE_STATUS::LeaveWindow, m_Info });
	TruncateBuffer();
}

//ウィンドウ内に入る
void MOUSE::Enterwindow() noexcept
{
	m_Info.bIsInWindow = true;
	m_Buffer.push({ MOUSE_STATUS::EnterWindow, m_Info });
	TruncateBuffer();
}

//左クリックオン
void MOUSE::LeftPressed() noexcept
{
	m_Info.bLeftIsPressed = true;
	m_Buffer.push({ MOUSE_STATUS::L_Press, m_Info });
	TruncateBuffer();
}

//左クリックオフ
void MOUSE::LeftReleased() noexcept
{
	m_Info.bLeftIsPressed = false;
	m_Buffer.push({ MOUSE_STATUS::L_Release, m_Info });
	TruncateBuffer();
}

//右クリックオン
void MOUSE::RightPressed() noexcept
{
	m_Info.bRightIsPressed = true;
	m_Buffer.push({ MOUSE_STATUS::R_Press, m_Info });
	TruncateBuffer();
}

//右クリックオフ
void MOUSE::RightReleased() noexcept
{
	m_Info.bRightIsPressed = false;
	m_Buffer.push({ MOUSE_STATUS::R_Release, m_Info });
	TruncateBuffer();
}

//ホイールアップ
void MOUSE::WheelUp() noexcept
{
	m_Buffer.push({ MOUSE_STATUS::WheelUp, m_Info });
	TruncateBuffer();
	m_WheelVal++;
}

//ホイールダウン
void MOUSE::WheelDown() noexcept
{
	m_Buffer.push({ MOUSE_STATUS::WheelDown, m_Info });
	TruncateBuffer();
	m_WheelVal--;
}

//ホイール処理
void MOUSE::WheelProc(int nDelta) noexcept
{
	m_WheelDelta += nDelta;

	//ホイール応答精度制御(1回のメッセージで応答)
	static int nResponse = WHEEL_DELTA * 1;
	if (m_WheelDelta >= nResponse) {
		m_WheelDelta = 0;
		WheelUp();
	}
	if (m_WheelDelta <= -nResponse) {
		m_WheelDelta = 0;
		WheelDown();
	}
}

//RawInputバッファ切り捨て
void MOUSE::TruncateRawInputBuffer() noexcept
{
	while (m_RawDeltaBuffer.size() > nBufferSize)	//上限サイズに収まるまで
		m_RawDeltaBuffer.pop();						//キューポップ
}

//RawInput情報取得
void MOUSE::GetRawDelta(int dx, int dy) noexcept
{
	m_RawDeltaBuffer.push({ dx, dy });
	TruncateRawInputBuffer();
}
