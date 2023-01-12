
//===== インクルード部 =====
#include <GraphicApp/Binder/cbMatrix_LWVP.h>
#include <GraphicApp/Drawer/Drawer.h>
#include <Tool/gMath.h>

namespace dx = DirectX;
using cb = CONSTANT_BUFFER<CBD_MTX_LWVP>;

//===== 静的メンバ変数 =====
std::unique_ptr<cb> CB_MTX_LWVP::m_pCBuff{};
int CB_MTX_LWVP::m_RefCount = 0;

//===== クラス実装 =====
CB_MTX_LWVP::CB_MTX_LWVP(const GRAPHIC& Gfx, CB_PTR* cbPtr, const DRAWER& Parent, const DirectX::XMFLOAT4X4& mtxL) :
	BINDER(), m_Parent(Parent), m_mtxL(mtxL)
{
	//定数バッファ初期化
	if (!m_pCBuff)
		m_pCBuff = std::make_unique<cb>(Gfx, cbPtr, true);
	else
		m_pCBuff->SetBuffPtr(cbPtr);
	m_RefCount++;
}

CB_MTX_LWVP::~CB_MTX_LWVP() noexcept
{
	//バッファ解放
	m_RefCount--;
	if (m_RefCount == 0)
		m_pCBuff.reset();
}

//バインド処理
void CB_MTX_LWVP::Bind(const GRAPHIC& Gfx) const noexcept
{
	//バッファ更新
	dx::XMFLOAT4X4 Local = m_mtxL;
	dx::XMFLOAT4X4 World = m_Parent.GetWorldMatrix();
	dx::XMFLOAT4X4 View = Gfx.GetViewMtx();
	dx::XMFLOAT4X4 Proj = Gfx.GetProjectionMtx();
	gMath::MtxTranspose4x4_SSE(&Local._11);
	gMath::MtxTranspose4x4_SSE(&World._11);
	gMath::MtxTranspose4x4_SSE(&View._11);
	gMath::MtxTranspose4x4_SSE(&Proj._11);
	CBD_MTX_LWVP aMtx = { Local, World, View, Proj };
	m_pCBuff->Update(Gfx, aMtx);

	//バインド処理
	m_pCBuff->Bind(Gfx);
}
