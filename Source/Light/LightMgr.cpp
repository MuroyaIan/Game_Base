
//===== �C���N���[�h�� =====
#include <Light/LightMgr.h>
#include <GraphicApp/Binder/BinderRef.h>

//===== �N���X���� =====
LIGHT_MGR::LIGHT_MGR(APP& App) noexcept :
	m_Gfx(App.GetGfxPack()), m_LightData(), m_UsedData(0), m_UsedData_backup(m_UsedData), m_pcBuff()
{
	//�萔�o�b�t�@�쐬
	m_pcBuff = std::make_unique<PS_CBUFFER<LIGHT_PACK>>(m_Gfx.m_DX, static_cast<UINT>(CB_SLOT_PS::Light));
}

LIGHT_MGR::~LIGHT_MGR() noexcept
{
}

//�����ݏ���
void LIGHT_MGR::Draw() noexcept
{
	//�萔�o�b�t�@�X�V
	m_pcBuff->Update(m_Gfx.m_DX, m_LightData);
	m_pcBuff->Bind(m_Gfx.m_DX);

	//���C�g�o�^���Z�b�g
	m_UsedData_backup = m_UsedData;
	m_UsedData = 0;
}

//�|�C���g���C�g�o�^
void LIGHT_MGR::AddPointLight(LIGHT::LIGHT_POINT Data) noexcept
{
	//��O����
	if (m_UsedData >= LIGHT_NUM)
		return;

	//�f�[�^�X�V
	m_LightData.PointLight[m_UsedData] = Data;
	m_UsedData++;
}
