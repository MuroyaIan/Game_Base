
//===== インクルード部 =====
#include <Geometry/Debug/DebugMgr.h>

namespace dx = DirectX;

//===== クラス実装 =====
DEBUG_MGR::DEBUG_MGR(APP_64& App) noexcept :
	m_Gfx(App.GetGfxPack()), m_Light(App.GetGfxPack())
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
void DEBUG_MGR::Draw(GRAPHIC& Gfx) const noexcept(!IS_DEBUG)
{
	m_Light.Draw(Gfx);
}

//ポリゴン数取得
UINT DEBUG_MGR::GetPolygonNum() const noexcept
{
	UINT PolyNum = 0;
	PolyNum += m_Light.GetPolygonNum();

	return PolyNum;
}
