
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/srvMgr.h>

//===== �N���X���� =====
SRV_MGR::SRV_MGR(SRV_PTR& srvPtrRef) noexcept :
	m_aSrvPtrVS(std::move(srvPtrRef.m_aSrvPtrVS)), m_aSrvPtrPS(std::move(srvPtrRef.m_aSrvPtrPS)),
	m_SrvSizeVS(static_cast<UINT>(m_aSrvPtrVS.size())), m_SrvSizePS(static_cast<UINT>(m_aSrvPtrPS.size()))
{}

SRV_MGR::~SRV_MGR() noexcept
{
	//�|�C���^���
	for (auto& p : m_aSrvPtrVS)
		p = nullptr;
	for (auto& p : m_aSrvPtrPS)
		p = nullptr;
}

//�o�C���h����
void SRV_MGR::Bind(const GRAPHIC& Gfx) const
{
	//���ꂼ��̃V�F�[�_�փo�C���h����
	if (m_SrvSizeVS > 0u)
		GetContext(Gfx)->VSSetShaderResources(0u, m_SrvSizeVS, m_aSrvPtrVS.data());
	if (m_SrvSizePS > 0u)
		GetContext(Gfx)->PSSetShaderResources(0u, m_SrvSizePS, m_aSrvPtrPS.data());
}
