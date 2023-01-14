
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/cbLocal.h>

using cb = CONSTANT_BUFFER<CBD_MTX_LOCAL>;

//===== �ÓI�����o�ϐ� =====
std::unique_ptr<cb> CB_LOCAL::m_pCBuff{};
int CB_LOCAL::m_RefCount = 0;

//===== �N���X���� =====
CB_LOCAL::CB_LOCAL(const GRAPHIC& Gfx, CB_PTR* cbPtr, const CBD_MTX_LOCAL& LocalData) :
	BINDER(), m_LocalData(LocalData)
{
	//�萔�o�b�t�@������
	if (!m_pCBuff)
		m_pCBuff = std::make_unique<cb>(Gfx, cbPtr, true);
	else
		m_pCBuff->SetBuffPtr(cbPtr);
	m_RefCount++;
}

CB_LOCAL::~CB_LOCAL() noexcept
{
	//�o�b�t�@���
	m_RefCount--;
	if (m_RefCount == 0)
		m_pCBuff.reset();
}

//�o�C���h����(�f�[�^�X�V)
void CB_LOCAL::Bind(const GRAPHIC& Gfx) const noexcept
{
	//�o�b�t�@�X�V
	m_pCBuff->Update(Gfx, m_LocalData);
}
