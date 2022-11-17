
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/cbMatrix_LWVP.h>
#include <Tool/gMath.h>

namespace dx = DirectX;
using vcbMtx = VERTEX_CBUFFER<CB_MTX_L_W_V_P>;

//===== �ÓI�����o�ϐ� =====
std::unique_ptr<vcbMtx> CB_MTX_LWVP::m_pVcBuff = nullptr;

//===== �N���X���� =====
CB_MTX_LWVP::CB_MTX_LWVP(GRAPHIC& Gfx, const DRAWER& Parent, const DirectX::XMFLOAT4X4& mtxL) :
	BINDER(), m_Parent(Parent), m_mtxL(mtxL)
{
	//�萔�o�b�t�@������
	if (!m_pVcBuff)
		m_pVcBuff = std::make_unique<vcbMtx>(Gfx);
}

CB_MTX_LWVP::~CB_MTX_LWVP() noexcept
{
}

//�o�C���h����
void CB_MTX_LWVP::Bind(GRAPHIC& Gfx) noexcept
{
	//�o�b�t�@�X�V
	dx::XMFLOAT4X4 Local = m_mtxL;
	dx::XMFLOAT4X4 World = m_Parent.GetTransformMtx();
	dx::XMFLOAT4X4 View = Gfx.GetViewMtx();
	dx::XMFLOAT4X4 Proj = Gfx.GetProjectionMtx();
	gMath::MtxTranspose4x4_SSE(&Local._11);
	gMath::MtxTranspose4x4_SSE(&World._11);
	gMath::MtxTranspose4x4_SSE(&View._11);
	gMath::MtxTranspose4x4_SSE(&Proj._11);
	CB_MTX_L_W_V_P aMtx = { Local, World, View, Proj };
	m_pVcBuff->Update(Gfx, aMtx);

	//�o�C���h����
	m_pVcBuff->Bind(Gfx);
}
