
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/cbMatrix_LWVP.h>
#include <GraphicApp/Drawer/Drawer.h>
#include <Tool/gMath.h>

namespace dx = DirectX;
using cb = CONSTANT_BUFFER<CBD_MTX_LWVP>;

//===== �ÓI�����o�ϐ� =====
std::unique_ptr<cb> CB_MTX_LWVP::m_pCBuff{};
int CB_MTX_LWVP::m_RefCount = 0;

//===== �N���X���� =====
CB_MTX_LWVP::CB_MTX_LWVP(const CT_GRAPHIC& Gfx, CB_PTR* cbPtr, const DRAWER& Parent, const dx::XMFLOAT4X4& mtxL) :
	CT_BINDER(), m_Parent(Parent), m_mtxL(mtxL)
{
	//�萔�o�b�t�@������
	if (!m_pCBuff)
		m_pCBuff = std::make_unique<cb>(Gfx, cbPtr, true);
	else
		m_pCBuff->SetBuffPtr(cbPtr);
	m_RefCount++;
}

CB_MTX_LWVP::~CB_MTX_LWVP() noexcept
{
	//�o�b�t�@���
	m_RefCount--;
	if (m_RefCount == 0)
		m_pCBuff.reset();
}

//�o�C���h����(�f�[�^�X�V)
void CB_MTX_LWVP::Bind(const CT_GRAPHIC& Gfx) const
{
	//�o�b�t�@�X�V
	CBD_MTX_LWVP aMtx;
	aMtx.ms_MtxLocal = m_mtxL;
	aMtx.ms_MtxWorld = m_Parent.GetWorldMatrix();
	aMtx.ms_MtxView = Gfx.GetViewMtx();
	aMtx.ms_MtxProj = Gfx.GetProjectionMtx();
	gMath::MtxTranspose4x4_SSE(&aMtx.ms_MtxLocal._11);
	gMath::MtxTranspose4x4_SSE(&aMtx.ms_MtxWorld._11);
	gMath::MtxTranspose4x4_SSE(&aMtx.ms_MtxView._11);
	gMath::MtxTranspose4x4_SSE(&aMtx.ms_MtxProj._11);
	m_pCBuff->Update(Gfx, aMtx);
}
