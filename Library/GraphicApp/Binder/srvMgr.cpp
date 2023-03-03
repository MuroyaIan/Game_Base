
//===== インクルード部 =====
#include <GraphicApp/Binder/srvMgr.h>

//===== クラス実装 =====
SRV_MGR::SRV_MGR(SRV_PTR& srvPtrRef) noexcept :
	m_aSrvPtrVS(std::move(srvPtrRef.m_aSrvPtrVS)), m_aSrvPtrPS(std::move(srvPtrRef.m_aSrvPtrPS)),
	m_SrvSizeVS(static_cast<UINT>(m_aSrvPtrVS.size())), m_SrvSizePS(static_cast<UINT>(m_aSrvPtrPS.size()))
{}

SRV_MGR::~SRV_MGR() noexcept
{
	//ポインタ解放
	for (auto& p : m_aSrvPtrVS)
		p = nullptr;
	for (auto& p : m_aSrvPtrPS)
		p = nullptr;
}

//バインド処理
void SRV_MGR::Bind(const GRAPHIC& Gfx) const
{
	//それぞれのシェーダへバインドする
	if (m_SrvSizeVS > 0u)
		GetContext(Gfx)->VSSetShaderResources(0u, m_SrvSizeVS, m_aSrvPtrVS.data());
	if (m_SrvSizePS > 0u)
		GetContext(Gfx)->PSSetShaderResources(0u, m_SrvSizePS, m_aSrvPtrPS.data());
}
