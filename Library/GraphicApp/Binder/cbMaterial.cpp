
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/cbMaterial.h>
#include <GraphicApp/Drawer/vsdRef.h>

using pcbMta = PIXEL_CBUFFER<MATERIAL_DATA>;

//===== �ÓI�����o�ϐ� =====
std::unique_ptr<pcbMta> CB_MATERIAL::m_pPcBuff = nullptr;

//===== �N���X���� =====
CB_MATERIAL::CB_MATERIAL(GRAPHIC& Gfx, const MATERIAL_DATA& Material) :
	BINDER(), m_Material(Material)
{
	//�萔�o�b�t�@������
	if (!m_pPcBuff)
		m_pPcBuff = std::make_unique<pcbMta>(Gfx, static_cast<UINT>(CB_SLOT_PS::MATERIAL));
}

CB_MATERIAL::~CB_MATERIAL() noexcept
{
}

//�o�C���h����
void CB_MATERIAL::Bind(GRAPHIC& Gfx) noexcept
{
	//�o�b�t�@�X�V
	m_pPcBuff->Update(Gfx, m_Material);

	//�o�C���h����
	m_pPcBuff->Bind(Gfx);
}
