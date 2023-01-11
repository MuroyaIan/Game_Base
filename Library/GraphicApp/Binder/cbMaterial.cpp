
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/cbMaterial.h>
#include <GraphicApp/Drawer/vsdRef.h>

using pscb = PS_CBUFFER<CBD_MATERIAL>;

//===== �ÓI�����o�ϐ� =====
std::unique_ptr<pscb> CB_MATERIAL::m_pCBuffPS{};
int CB_MATERIAL::m_RefCount = 0;

//===== �N���X���� =====
CB_MATERIAL::CB_MATERIAL(const GRAPHIC& Gfx, const CBD_MATERIAL& Material) :
	BINDER(), m_Material(Material)
{
	//�萔�o�b�t�@������
	if (!m_pCBuffPS)
		m_pCBuffPS = std::make_unique<pscb>(Gfx, static_cast<UINT>(CB_SLOT_PS::Material));
	m_RefCount++;
}

CB_MATERIAL::~CB_MATERIAL() noexcept
{
	//�o�b�t�@���
	m_RefCount--;
	if (m_RefCount == 0)
		m_pCBuffPS.reset();
}

//�o�C���h����
void CB_MATERIAL::Bind(const GRAPHIC& Gfx) const noexcept
{
	//�o�b�t�@�X�V
	m_pCBuffPS->Update(Gfx, m_Material);

	//�o�C���h����
	m_pCBuffPS->Bind(Gfx);
}
