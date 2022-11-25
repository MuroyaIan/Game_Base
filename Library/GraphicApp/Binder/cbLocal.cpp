
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/cbLocal.h>
#include <Tool/gMath.h>

using vcbBone = VERTEX_CBUFFER<CBD_MTX_LOCAL>;

//===== �ÓI�����o�ϐ� =====
std::unique_ptr<vcbBone> CB_LOCAL::m_pVcBuff = nullptr;
int CB_LOCAL::m_RefCount = 0;

//===== �N���X���� =====
CB_LOCAL::CB_LOCAL(GRAPHIC& Gfx, const CBD_MTX_LOCAL& LocalData) :
	BINDER(), m_LocalData(LocalData)
{
	//�萔�o�b�t�@������
	if (!m_pVcBuff)
		m_pVcBuff = std::make_unique<vcbBone>(Gfx, static_cast<UINT>(CB_SLOT_VS::LOCAL));
	m_RefCount++;
}

CB_LOCAL::~CB_LOCAL() noexcept
{
	//�o�b�t�@���
	m_RefCount--;
	if (m_RefCount == 0)
		m_pVcBuff.reset();
}

//�o�C���h����
void CB_LOCAL::Bind(const GRAPHIC& Gfx) const noexcept
{
	//�o�b�t�@�X�V
	m_pVcBuff->Update(Gfx, m_LocalData);

	//�o�C���h����
	m_pVcBuff->Bind(Gfx);
}
