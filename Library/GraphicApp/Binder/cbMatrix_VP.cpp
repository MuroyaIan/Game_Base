
//===== インクルード部 =====
#include <GraphicApp/Binder/cbMatrix_VP.h>
#include <Tool/gMath.h>

namespace dx = DirectX;
using vcbMtx = VERTEX_CBUFFER<CBD_MTX_VP>;

//===== 静的メンバ変数 =====
std::unique_ptr<vcbMtx> CB_MTX_VP::m_pVcBuff = nullptr;
int CB_MTX_VP::m_RefCount = 0;

//===== クラス実装 =====
CB_MTX_VP::CB_MTX_VP(GRAPHIC& Gfx) : BINDER()
{
	//定数バッファ初期化
	if (!m_pVcBuff)
		m_pVcBuff = std::make_unique<vcbMtx>(Gfx, static_cast<UINT>(CB_SLOT_VS::CAMERA));
	m_RefCount++;
}

CB_MTX_VP::~CB_MTX_VP() noexcept
{
	//バッファ解放
	m_RefCount--;
	if (m_RefCount == 0)
		m_pVcBuff.reset();
}

//バインド処理
void CB_MTX_VP::Bind(const GRAPHIC& Gfx) noexcept
{
	//バッファ更新
	dx::XMFLOAT4X4 View = Gfx.GetViewMtx();
	dx::XMFLOAT4X4 Proj = Gfx.GetProjectionMtx();
	gMath::MtxTranspose4x4_SSE(&View._11);
	gMath::MtxTranspose4x4_SSE(&Proj._11);
	CBD_MTX_VP aMtx = { View, Proj };
	m_pVcBuff->Update(Gfx, aMtx);

	//バインド処理
	m_pVcBuff->Bind(Gfx);
}
