
//===== �C���N���[�h�� =====
#include <Geometry/Debug/DebugMgr.h>

//===== �N���X���� =====
DEBUG_MGR::DEBUG_MGR(APP& App) noexcept : m_Light(App.GetGfxPack(), App.GetCameraMgr())
{
}

DEBUG_MGR::~DEBUG_MGR() noexcept
{
}

//�X�V����
void DEBUG_MGR::Update()
{
	m_Light.Update();
}

//�`�揈��
void DEBUG_MGR::Draw() noexcept
{
	m_Light.Draw();
}

//�|���S�����擾
UINT DEBUG_MGR::GetPolygonNum() const noexcept
{
	UINT PolyNum = 0;
	PolyNum += m_Light.GetPolygonNum();

	return PolyNum;
}
