
//===== インクルード部 =====
#include <Tool/Input/IM_Mouse.h>
#include <App.h>
#include <Tool/Input/IM_Keyboard.h>

//===== クラス実装 =====
INPUT_CURSOR::INPUT_CURSOR(APP& App, INPUT_KB& KB) noexcept :
	m_Window(App.GetWindowProc()), m_Mouse(App.GetWindowProc().m_Mouse), m_KB(KB), MoveVal()
{
}

INPUT_CURSOR::~INPUT_CURSOR() noexcept
{
}

//プレス判定
bool INPUT_CURSOR::GetPress(int nBtn) const noexcept
{
	return m_KB.GetPress(nBtn);
}

//トリガー判定
bool INPUT_CURSOR::GetTrigger(int nBtn) const noexcept
{
	return m_KB.GetTrigger(nBtn);
}

//リリース判定
bool INPUT_CURSOR::GetRelease(int nBtn) const noexcept
{
	return m_KB.GetRelease(nBtn);
}

//リピート判定
bool INPUT_CURSOR::GetRepeat(int nBtn, float fInterval)
{
	return m_KB.GetRepeat(nBtn, fInterval);
}

//座標取得
DirectX::XMFLOAT2 INPUT_CURSOR::GetPos() const noexcept
{
	auto [x, y] = m_Mouse.GetPos();
	return DirectX::XMFLOAT2(static_cast<float>(x), static_cast<float>(y));
}

//ホイール値取得
int INPUT_CURSOR::GetWheelVal() const noexcept
{
	return m_Mouse.GetWheelVal();
}

//マウス表示
void INPUT_CURSOR::Show() const noexcept
{
	m_Window.EnableCursor();
}

//マウス不表示
void INPUT_CURSOR::Hide() const noexcept
{
	m_Window.DisableCursor();
}

//マウス描画状態確認
bool INPUT_CURSOR::IsDrawing() const noexcept
{
	return m_Window.IsUsingCursor();
}

//マウス使用
void INPUT_CURSOR::EnableRawInput() const noexcept
{
	m_Window.m_Mouse.SetRawInput(true);
}

//マウス不使用
void INPUT_CURSOR::DisableRawInput() const noexcept
{
	m_Window.m_Mouse.SetRawInput(false);
}

//更新処理
void INPUT_CURSOR::Update() noexcept
{
	//マウス移動量取得
	MoveVal.x = 0;
	MoveVal.y = 0;						//移動量を0クリア
	if (!m_Mouse.IsUsingRawInput())		//RawInput不使用の場合、処理を除外
		return;
	while (true) {
		const auto d = m_Mouse.ReadRawDelta();

		//脱出処理(移動量をすべて取得)
		if (d.bClear)
			break;

		//デルタ分の移動量を全部加算
		MoveVal.x += d.x;
		MoveVal.y += d.y;
	}
}
