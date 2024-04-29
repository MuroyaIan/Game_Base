
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/cbMaterial.h>
#include <GraphicApp/Drawer/vsdRef.h>

using cb = CONSTANT_BUFFER<CBD_MATERIAL>;

//===== �ÓI�����o�ϐ� =====
std::unique_ptr<cb> CB_MATERIAL::m_pCBuff{};
int CB_MATERIAL::m_RefCount = 0;

//===== �N���X���� =====
CB_MATERIAL::CB_MATERIAL(const CT_GRAPHIC& Gfx, CB_PTR* cbPtr, const CBD_MATERIAL& Material) :
	CT_BINDER(), m_Material(Material)
{
	//�萔�o�b�t�@������
	if (!m_pCBuff)
		m_pCBuff = std::make_unique<cb>(Gfx, cbPtr, false, true);
	else
		m_pCBuff->SetBuffPtr(cbPtr);
	m_RefCount++;
}

CB_MATERIAL::~CB_MATERIAL() noexcept
{
	//�o�b�t�@���
	m_RefCount--;
	if (m_RefCount == 0)
		m_pCBuff.reset();
}

//�o�C���h����(�f�[�^�X�V)
void CB_MATERIAL::Bind(const CT_GRAPHIC& Gfx) const
{
	//�o�b�t�@�X�V
	m_pCBuff->Update(Gfx, m_Material);
}
