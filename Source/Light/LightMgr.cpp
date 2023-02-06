
//===== �C���N���[�h�� =====
#include <Light/LightMgr.h>
#include <GraphicApp/Binder/BinderRef.h>

//===== �N���X���� =====
LIGHT_MGR::LIGHT_MGR(APP& App) noexcept :
	m_Gfx(App.GetGfxPack()), m_cBuffRef(dynamic_cast<CONSTANT_BUFFER<LIGHT_PACK>&>(m_Gfx.m_ShaderMgr.GetBinder(SHADER_MGR::BINDER_ID::CB_PS_LIGHT))),
	m_LightData(), m_UsedData(0), m_UsedData_backup(m_UsedData)
{
}

LIGHT_MGR::~LIGHT_MGR() noexcept
{
}

//�X�V����
void LIGHT_MGR::Update()
{
	//�萔�o�b�t�@�X�V
	m_cBuffRef.Update(m_Gfx.m_DX, m_LightData);

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
