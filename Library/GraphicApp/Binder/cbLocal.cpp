
//===== インクルード部 =====
#include <GraphicApp/Binder/cbLocal.h>
#include <Tool/gMath.h>

using vcbBone = VERTEX_CBUFFER<CB_MTX_LOCAL>;

//===== 静的メンバ変数 =====
std::unique_ptr<vcbBone> CB_LOCAL::m_pVcBuff = nullptr;

//===== クラス実装 =====
CB_LOCAL::CB_LOCAL(GRAPHIC& Gfx, const CB_MTX_LOCAL& LocalData) :
	BINDER(), m_LocalData(LocalData)
{
	//定数バッファ初期化
	if (!m_pVcBuff)
		m_pVcBuff = std::make_unique<vcbBone>(Gfx, static_cast<UINT>(CB_SLOT_VS::LOCAL));
}

CB_LOCAL::~CB_LOCAL() noexcept
{
}

//バインド処理
void CB_LOCAL::Bind(GRAPHIC& Gfx) noexcept
{
	//バッファ更新
	m_pVcBuff->Update(Gfx, m_LocalData);

	//バインド処理
	m_pVcBuff->Bind(Gfx);
}
