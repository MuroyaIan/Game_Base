
//===== インクルード部 =====
#include <Geometry/Debug/DebugMgr.h>

//===== クラス実装 =====
DEBUG_MGR::DEBUG_MGR(APP& App) noexcept : m_Light(App.GetGfxPack(), App.GetCameraMgr())
{
}

DEBUG_MGR::~DEBUG_MGR() noexcept
{
}

//更新処理
void DEBUG_MGR::Update() noexcept
{
	m_Light.Update();
}

//描画処理
void DEBUG_MGR::Draw() const noexcept
{
	m_Light.Draw();
}

//ポリゴン数取得
UINT DEBUG_MGR::GetPolygonNum() const noexcept
{
	UINT PolyNum = 0;
	PolyNum += m_Light.GetPolygonNum();

	return PolyNum;
}
