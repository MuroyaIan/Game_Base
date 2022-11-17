
//===== �C���N���[�h�� =====
#include <Geometry/Debug/DebugMgr.h>

namespace dx = DirectX;

//===== �N���X���� =====
DEBUG_MGR::DEBUG_MGR(APP_64& App) noexcept :
	m_Gfx(App.GetGfxPack()), m_Light(App.GetGfxPack())
{
}

DEBUG_MGR::~DEBUG_MGR() noexcept
{
}

//�X�V����
void DEBUG_MGR::Update() noexcept
{
	m_Light.Update();
}

//�`�揈��
void DEBUG_MGR::Draw(GRAPHIC& Gfx) const noexcept(!IS_DEBUG)
{
	m_Light.Draw(Gfx);
}

//�|���S�����擾
UINT DEBUG_MGR::GetPolygonNum() const noexcept
{
	UINT PolyNum = 0;
	PolyNum += m_Light.GetPolygonNum();

	return PolyNum;
}
