
//===== インクルード部 =====
#include <GraphicApp/Binder/cbLocal.h>

using cb = CONSTANT_BUFFER<CBD_MTX_LOCAL>;

//===== 静的メンバ変数 =====
std::unique_ptr<cb> CB_LOCAL::m_pCBuff{};
int CB_LOCAL::m_RefCount = 0;

//===== クラス実装 =====
CB_LOCAL::CB_LOCAL(const GRAPHIC& Gfx, CB_PTR* cbPtr, const CBD_MTX_LOCAL& LocalData) :
	BINDER(), m_LocalData(LocalData)
{
	//定数バッファ初期化
	if (!m_pCBuff)
		m_pCBuff = std::make_unique<cb>(Gfx, cbPtr, true);
	else
		m_pCBuff->SetBuffPtr(cbPtr);
	m_RefCount++;
}

CB_LOCAL::~CB_LOCAL() noexcept
{
	//バッファ解放
	m_RefCount--;
	if (m_RefCount == 0)
		m_pCBuff.reset();
}

//バインド処理(データ更新)
void CB_LOCAL::Bind(const GRAPHIC& Gfx) const noexcept
{
	//バッファ更新
	m_pCBuff->Update(Gfx, m_LocalData);
}
