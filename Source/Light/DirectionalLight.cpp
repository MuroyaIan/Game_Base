
//===== インクルード部 =====
#include <Light/DirectionalLight.h>

#ifdef IMGUI
#
#	include <Geometry/Debug/DebugMgr.h>
#
#endif // IMGUI

namespace dx = DirectX;

//===== クラス実装 =====
DIRECTIONAL_LIGHT::DIRECTIONAL_LIGHT(APP& App) noexcept : LIGHT(App), m_LightMgr(App.GetLightMgr()), m_LightData()
{
	m_LightData.Color_D = { 1.0f, 1.0f, 1.0f, 1.0f };
}

DIRECTIONAL_LIGHT::~DIRECTIONAL_LIGHT() noexcept
{
}

//更新処理
void DIRECTIONAL_LIGHT::Update() const noexcept
{
	//データ更新
	m_LightMgr.GetData().DirectionalLight = m_LightData;

#ifdef IMGUI

	//デバッグ更新
	dx::XMFLOAT3 pos{ m_LightData.Pos.x, m_LightData.Pos.y, m_LightData.Pos.z };
	m_Mesh.SetPos(pos, m_MeshIndex);
	m_Mesh.SetScale(m_Scale, m_MeshIndex);

#endif // IMGUI

}
