
//===== インクルード部 =====
#include <GraphicApp/Binder/cbMatrix_T.h>
#include <GraphicApp/Drawer/Drawer.h>
#include <Tool/gMath.h>

namespace dx = DirectX;
using vscb = VS_CBUFFER<dx::XMFLOAT4X4>;

//===== 静的メンバ変数 =====
std::unique_ptr<vscb> CB_MTX_T::m_pCBuffVS{};
int CB_MTX_T::m_RefCount = 0;

//===== クラス実装 =====
CB_MTX_T::CB_MTX_T(const GRAPHIC& Gfx, const DRAWER& Parent) :
	BINDER(), m_Parent(Parent)
{
	//定数バッファ初期化
	if (!m_pCBuffVS)
		m_pCBuffVS = std::make_unique<vscb>(Gfx);
	m_RefCount++;
}

CB_MTX_T::~CB_MTX_T() noexcept
{
	//バッファ解放
	m_RefCount--;
	if (m_RefCount == 0)
		m_pCBuffVS.reset();
}

//バインド処理
void CB_MTX_T::Bind(const GRAPHIC& Gfx) const noexcept
{
	//バッファ更新
	dx::XMFLOAT4X4 World = m_Parent.GetTransformMtx();
	dx::XMFLOAT4X4 View = Gfx.GetViewMtx();
	dx::XMFLOAT4X4 Proj = Gfx.GetProjectionMtx();
	dx::XMFLOAT4X4 mtxWVP = World;
	gMath::MtxMultiply4x4_AVX(&mtxWVP._11, &View._11);
	gMath::MtxMultiply4x4_AVX(&mtxWVP._11, &Proj._11);
	gMath::MtxTranspose4x4_SSE(&mtxWVP._11);
	m_pCBuffVS->Update(Gfx, mtxWVP);

	//バインド処理
	m_pCBuffVS->Bind(Gfx);
}
