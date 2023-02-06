
//===== インクルード部 =====
#include <Light/PointLight.h>
#include <Light/LightMgr.h>
#include <Tool/gMath.h>

#ifdef IMGUI
#
#	include <Geometry/Debug/DebugMgr.h>
#
#endif // IMGUI

namespace dx = DirectX;

//===== クラス実装 =====
POINT_LIGHT::POINT_LIGHT(APP& App, float Range) noexcept :
	LIGHT(App), m_LightData(), m_Range(100.0f), m_LightMgr(App.GetLightMgr())
{
	m_LightData.Color_D = { 1.0f, 1.0f, 1.0f, 1.0f };

	//減衰係数初期化
	ClacAttenuation(Range);
}

POINT_LIGHT::~POINT_LIGHT() noexcept
{
}

//更新処理
void POINT_LIGHT::Update() const noexcept
{
	//データ更新
	m_LightMgr.AddPointLight(m_LightData);

#ifdef IMGUI

	//デバッグ更新
	dx::XMFLOAT3 pos{ m_LightData.Pos.x, m_LightData.Pos.y, m_LightData.Pos.z };
	m_Mesh.SetPos(pos, m_MeshIndex);
	m_Mesh.SetScale(m_Scale, m_MeshIndex);

#endif // IMGUI

}

//減衰計算
void POINT_LIGHT::ClacAttenuation(float Range) noexcept
{
	//例外処理
	if (m_Range == Range)
		return;

	//計算処理
	float Coef = Range / m_Range;
	m_LightData.AttLinear = m_LightData.AttLinear / Coef;
	m_LightData.AttQuadratic = m_LightData.AttQuadratic / std::powf(Coef, 2.0f);;
}
