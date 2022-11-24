
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/cbMatrix_LWVP.h>
#include <Tool/gMath.h>

namespace dx = DirectX;
using vcbMtx = VERTEX_CBUFFER<CBD_MTX_LWVP>;

//===== �ÓI�����o�ϐ� =====
std::unique_ptr<vcbMtx> CB_MTX_LWVP::m_pVcBuff = nullptr;
int CB_MTX_LWVP::m_RefCount = 0;

//===== �N���X���� =====
CB_MTX_LWVP::CB_MTX_LWVP(GRAPHIC& Gfx, const DRAWER& Parent, const DirectX::XMFLOAT4X4& mtxL) :
	BINDER(), m_Parent(Parent), m_mtxL(mtxL)
{
	//�萔�o�b�t�@������
	if (!m_pVcBuff)
		m_pVcBuff = std::make_unique<vcbMtx>(Gfx);
	m_RefCount++;
}

CB_MTX_LWVP::~CB_MTX_LWVP() noexcept
{
	//�o�b�t�@���
	m_RefCount--;
	if (m_RefCount == 0)
		m_pVcBuff.reset();
}

//�o�C���h����
void CB_MTX_LWVP::Bind(const GRAPHIC& Gfx) noexcept
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
	CBD_MTX_LWVP aMtx = { Local, World, View, Proj };
	m_pVcBuff->Update(Gfx, aMtx);

	//�o�C���h����
	m_pVcBuff->Bind(Gfx);
}
