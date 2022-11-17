
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/cbMatrix_T.h>
#include <Tool/gMath.h>

namespace dx = DirectX;
using vcbMtx = VERTEX_CBUFFER<dx::XMFLOAT4X4>;

//===== �ÓI�����o�ϐ� =====
std::unique_ptr<vcbMtx> CB_MTX_T::m_pVcBuff = nullptr;

//===== �N���X���� =====
CB_MTX_T::CB_MTX_T(GRAPHIC& Gfx, const DRAWER& Parent) :
	BINDER(), m_Parent(Parent)
{
	//�萔�o�b�t�@������
	if (!m_pVcBuff)
		m_pVcBuff = std::make_unique<vcbMtx>(Gfx);
}

CB_MTX_T::~CB_MTX_T() noexcept
{
}

//�o�C���h����
void CB_MTX_T::Bind(GRAPHIC& Gfx) noexcept
{
	//�o�b�t�@�X�V
	dx::XMFLOAT4X4 World = m_Parent.GetTransformMtx();
	dx::XMFLOAT4X4 View = Gfx.GetViewMtx();
	dx::XMFLOAT4X4 Proj = Gfx.GetProjectionMtx();
	dx::XMFLOAT4X4 mtxWVP = World;
	gMath::MtxMultiply4x4_AVX(&mtxWVP._11, &View._11);
	gMath::MtxMultiply4x4_AVX(&mtxWVP._11, &Proj._11);
	gMath::MtxTranspose4x4_SSE(&mtxWVP._11);
	m_pVcBuff->Update(Gfx, mtxWVP);

	//�o�C���h����
	m_pVcBuff->Bind(Gfx);
}
