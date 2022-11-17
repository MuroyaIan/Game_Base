
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/cbLocal.h>
#include <Tool/gMath.h>

using vcbBone = VERTEX_CBUFFER<CB_MTX_LOCAL>;

//===== �ÓI�����o�ϐ� =====
std::unique_ptr<vcbBone> CB_LOCAL::m_pVcBuff = nullptr;

//===== �N���X���� =====
CB_LOCAL::CB_LOCAL(GRAPHIC& Gfx, const CB_MTX_LOCAL& LocalData) :
	BINDER(), m_LocalData(LocalData)
{
	//�萔�o�b�t�@������
	if (!m_pVcBuff)
		m_pVcBuff = std::make_unique<vcbBone>(Gfx, static_cast<UINT>(CB_SLOT_VS::LOCAL));
}

CB_LOCAL::~CB_LOCAL() noexcept
{
}

//�o�C���h����
void CB_LOCAL::Bind(GRAPHIC& Gfx) noexcept
{
	//�o�b�t�@�X�V
	m_pVcBuff->Update(Gfx, m_LocalData);

	//�o�C���h����
	m_pVcBuff->Bind(Gfx);
}
