
//===== インクルード部 =====
#include <Tool/Input/IM_Mouse.h>
#include <App.h>
#include <Tool/Input/IM_Keyboard.h>

//===== クラス実装 =====
INPUT_CURSOR::INPUT_CURSOR(APP& App, INPUT_KB& KB) noexcept :
	m_Mouse(App.GetWindowProc().m_Mouse), m_KB(KB), MoveVal()
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

//移動量取得
DirectX::XMINT2 INPUT_CURSOR::GetMoveVal() const noexcept
{
	return MoveVal;
}

//更新処理
void INPUT_CURSOR::Update() noexcept
{
	//マウス移動量取得
	if (!m_Mouse.IsUsingRawInput())
		return;
	MoveVal.x = 0;
	MoveVal.y = 0;
	while (true) {
		const auto d = m_Mouse.ReadRawDelta();
		MoveVal.x += d.x;
		MoveVal.y += d.y;

		//脱出処理
		if (d.x == 0 && d.y == 0)
			break;
	}
}
