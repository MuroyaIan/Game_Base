
//===== インクルード部 =====
#include <Light/Light.h>

#ifdef IMGUI
#
#	include <Geometry/Debug/DebugMgr.h>
#
#endif // IMGUI

//===== クラス実装 =====
LIGHT::LIGHT(APP& App) noexcept

#ifdef IMGUI

	: m_Mesh(App.GetDebugMgr().GetLightMesh()), m_MeshIndex(0), m_Scale(1.0f, 1.0f, 1.0f)

#endif // IMGUI

{
	(void)App;

#ifdef IMGUI

	//インスタンス作成
	m_MeshIndex = m_Mesh.AddInstance();

#endif // IMGUI

}

LIGHT::~LIGHT() noexcept
{
}

//更新処理
void LIGHT::Update() const noexcept
{
}
