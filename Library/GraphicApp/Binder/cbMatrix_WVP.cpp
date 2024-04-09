
//===== インクルード部 =====
#include <GraphicApp/Binder/cbMatrix_WVP.h>
#include <GraphicApp/Drawer/Drawer.h>
#include <Tool/gMath.h>

namespace dx = DirectX;
using cb = CONSTANT_BUFFER<dx::XMFLOAT4X4>;

//===== 静的メンバ変数 =====
std::unique_ptr<cb> CB_MTX_WVP::m_pCBuff{};
int CB_MTX_WVP::m_RefCount = 0;

//===== クラス実装 =====
CB_MTX_WVP::CB_MTX_WVP(const CT_GRAPHIC& Gfx, CB_PTR* cbPtr, const DRAWER& Parent) :
	BINDER(), m_Parent(Parent)
{
	//定数バッファ初期化
	if (!m_pCBuff)
		m_pCBuff = std::make_unique<cb>(Gfx, cbPtr, true);
	else
		m_pCBuff->SetBuffPtr(cbPtr);
	m_RefCount++;
}

CB_MTX_WVP::~CB_MTX_WVP() noexcept
{
	//バッファ解放
	m_RefCount--;
	if (m_RefCount == 0)
		m_pCBuff.reset();
}

//バインド処理(データ更新)
void CB_MTX_WVP::Bind(const CT_GRAPHIC& Gfx) const
{
	//バッファ更新
	dx::XMFLOAT4X4 World = m_Parent.GetWorldMatrix();
	dx::XMFLOAT4X4 View = Gfx.GetViewMtx();
	dx::XMFLOAT4X4 Proj = Gfx.GetProjectionMtx();
	dx::XMFLOAT4X4 mtxWVP = World;
	gMath::MtxMultiply4x4_AVX(&mtxWVP._11, &View._11);
	gMath::MtxMultiply4x4_AVX(&mtxWVP._11, &Proj._11);
	gMath::MtxTranspose4x4_SSE(&mtxWVP._11);
	m_pCBuff->Update(Gfx, mtxWVP);
}
