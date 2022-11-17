
//===== インクルード部 =====
#include <Tool/Input/InputMgr.h>
#include <App_Win64.h>

//===== クラス実装 =====
INPUT_MGR::INPUT_MGR(APP_64& App) noexcept :
	m_KB(App), m_Mouse(App, m_KB), m_GamePad(App)
{
}

INPUT_MGR::~INPUT_MGR() noexcept
{
}

//更新処理
void INPUT_MGR::Update() noexcept
{
	//キーボード更新
	m_KB.Update();

	//マウス更新
	m_Mouse.Update();

	//コントローラ更新
	m_GamePad.Update();
}
