
//===== インクルード部 =====
#include <GraphicApp/Binder/cbLocal.h>
#include <Tool/gMath.h>

using vcbBone = VERTEX_CBUFFER<CBD_MTX_LOCAL>;

//===== 静的メンバ変数 =====
std::unique_ptr<vcbBone> CB_LOCAL::m_pVcBuff = nullptr;
int CB_LOCAL::m_RefCount = 0;

//===== クラス実装 =====
CB_LOCAL::CB_LOCAL(GRAPHIC& Gfx, const CBD_MTX_LOCAL& LocalData) :
	BINDER(), m_LocalData(LocalData)
{
	//定数バッファ初期化
	if (!m_pVcBuff)
		m_pVcBuff = std::make_unique<vcbBone>(Gfx, static_cast<UINT>(CB_SLOT_VS::LOCAL));
	m_RefCount++;
}

CB_LOCAL::~CB_LOCAL() noexcept
{
	//バッファ解放
	m_RefCount--;
	if (m_RefCount == 0)
		m_pVcBuff.reset();
}

//バインド処理
void CB_LOCAL::Bind(const GRAPHIC& Gfx) const noexcept
{
	//バッファ更新
	m_pVcBuff->Update(Gfx, m_LocalData);

	//バインド処理
	m_pVcBuff->Bind(Gfx);
}
