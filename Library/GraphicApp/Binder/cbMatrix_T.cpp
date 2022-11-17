
//===== インクルード部 =====
#include <GraphicApp/Binder/cbMatrix_T.h>
#include <Tool/gMath.h>

namespace dx = DirectX;
using vcbMtx = VERTEX_CBUFFER<dx::XMFLOAT4X4>;

//===== 静的メンバ変数 =====
std::unique_ptr<vcbMtx> CB_MTX_T::m_pVcBuff = nullptr;

//===== クラス実装 =====
CB_MTX_T::CB_MTX_T(GRAPHIC& Gfx, const DRAWER& Parent) :
	BINDER(), m_Parent(Parent)
{
	//定数バッファ初期化
	if (!m_pVcBuff)
		m_pVcBuff = std::make_unique<vcbMtx>(Gfx);
}

CB_MTX_T::~CB_MTX_T() noexcept
{
}

//バインド処理
void CB_MTX_T::Bind(GRAPHIC& Gfx) noexcept
{
	//バッファ更新
	dx::XMFLOAT4X4 World = m_Parent.GetTransformMtx();
	dx::XMFLOAT4X4 View = Gfx.GetViewMtx();
	dx::XMFLOAT4X4 Proj = Gfx.GetProjectionMtx();
	dx::XMFLOAT4X4 mtxWVP = World;
	gMath::MtxMultiply4x4_AVX(&mtxWVP._11, &View._11);
	gMath::MtxMultiply4x4_AVX(&mtxWVP._11, &Proj._11);
	gMath::MtxTranspose4x4_SSE(&mtxWVP._11);
	m_pVcBuff->Update(Gfx, mtxWVP);

	//バインド処理
	m_pVcBuff->Bind(Gfx);
}
