
//===== �C���N���[�h�� =====
#include <Light/Light.h>

#ifdef IMGUI
#
#	include <Geometry/Debug/DebugMgr.h>
#
#endif // IMGUI

//===== �N���X���� =====
LIGHT::LIGHT(APP& App) noexcept

#ifdef IMGUI

	: m_Mesh(App.GetDebugMgr().GetLightMesh()), m_MeshIndex(0), m_Scale(1.0f, 1.0f, 1.0f)

#endif // IMGUI

{
	(void)App;

#ifdef IMGUI

	//�C���X�^���X�쐬
	m_MeshIndex = m_Mesh.AddInstance();

#endif // IMGUI

}

LIGHT::~LIGHT() noexcept
{
}

//�X�V����
void LIGHT::Update() const noexcept
{
}
