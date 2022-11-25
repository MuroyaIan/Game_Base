
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/cbLocal.h>

using vscb = VS_CBUFFER<CBD_MTX_LOCAL>;

//===== �ÓI�����o�ϐ� =====
std::unique_ptr<vscb> CB_LOCAL::m_pCBuffVS{};
int CB_LOCAL::m_RefCount = 0;

//===== �N���X���� =====
CB_LOCAL::CB_LOCAL(const GRAPHIC& Gfx, const CBD_MTX_LOCAL& LocalData) :
	BINDER(), m_LocalData(LocalData)
{
	//�萔�o�b�t�@������
	if (!m_pCBuffVS)
		m_pCBuffVS = std::make_unique<vscb>(Gfx, static_cast<UINT>(CB_SLOT_VS::LOCAL));
	m_RefCount++;
}

CB_LOCAL::~CB_LOCAL() noexcept
{
	//�o�b�t�@���
	m_RefCount--;
	if (m_RefCount == 0)
		m_pCBuffVS.reset();
}

//�o�C���h����
void CB_LOCAL::Bind(const GRAPHIC& Gfx) const noexcept
{
	//�o�b�t�@�X�V
	m_pCBuffVS->Update(Gfx, m_LocalData);

	//�o�C���h����
	m_pCBuffVS->Bind(Gfx);
}
