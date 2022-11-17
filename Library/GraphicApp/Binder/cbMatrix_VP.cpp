
//===== インクルード部 =====
#include <GraphicApp/Binder/cbMatrix_VP.h>
#include <Tool/gMath.h>

namespace dx = DirectX;
using vcbMtx = VERTEX_CBUFFER<CB_MTX_V_P>;

//===== 静的メンバ変数 =====
std::unique_ptr<vcbMtx> CB_MTX_VP::m_pVcBuff = nullptr;

//===== クラス実装 =====
CB_MTX_VP::CB_MTX_VP(GRAPHIC& Gfx) : BINDER()
{
	//定数バッファ初期化
	if (!m_pVcBuff)
		m_pVcBuff = std::make_unique<vcbMtx>(Gfx, static_cast<UINT>(CB_SLOT_VS::CAMERA));
}

CB_MTX_VP::~CB_MTX_VP() noexcept
{
}

//バインド処理
void CB_MTX_VP::Bind(GRAPHIC& Gfx) noexcept
{
	//バッファ更新
	dx::XMFLOAT4X4 View = Gfx.GetViewMtx();
	dx::XMFLOAT4X4 Proj = Gfx.GetProjectionMtx();
	gMath::MtxTranspose4x4_SSE(&View._11);
	gMath::MtxTranspose4x4_SSE(&Proj._11);
	CB_MTX_V_P aMtx = { View, Proj };
	m_pVcBuff->Update(Gfx, aMtx);

	//バインド処理
	m_pVcBuff->Bind(Gfx);
}
