
//===== インクルード部 =====
#include <GraphicApp/Binder/cbMgr.h>

//===== クラス実装 =====
CBUFF_MGR::CBUFF_MGR(CB_PTR& cbPtrRef) noexcept :
	m_aBuffPtrVS(std::move(cbPtrRef.m_aBuffPtrVS)), m_aBuffPtrPS(std::move(cbPtrRef.m_aBuffPtrPS)),
	m_BuffSizeVS(static_cast<UINT>(m_aBuffPtrVS.size())), m_BuffSizePS(static_cast<UINT>(m_aBuffPtrPS.size()))
{}

CBUFF_MGR::~CBUFF_MGR() noexcept
{}

//バインド処理
void CBUFF_MGR::Bind(const GRAPHIC& Gfx) const noexcept
{
	//それぞれのシェーダへバインドする
	if (m_BuffSizeVS > 0u)
		BINDER::GetContext(Gfx)->VSSetConstantBuffers(0u, m_BuffSizeVS, m_aBuffPtrVS.data());
	if (m_BuffSizePS > 0u)
		BINDER::GetContext(Gfx)->PSSetConstantBuffers(0u, m_BuffSizePS, m_aBuffPtrPS.data());
}
