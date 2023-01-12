
//===== �C���N���[�h�� =====
#include <Geometry/Model/TestPlayer.h>
#include <Geometry/Model/Model.h>

//===== �N���X���� =====
PLAYER::PLAYER(APP& App) noexcept : m_pModel(), m_Input(App.GetInputMgr())
{
	m_pModel = std::make_unique<MODEL>(App, MODEL_MGR::MODEL_ID::Tarantella);
	for (size_t i = 0; i < 100; i++)
		m_pModel->AddInstance();
}

PLAYER::~PLAYER() noexcept
{
}

//�X�V����
void PLAYER::Update() noexcept
{
	if (m_Input.m_KB.GetPress(VK_E))
		m_pModel->ChangeAnimID(2);
	else
		m_pModel->ChangeAnimID(1);

	m_pModel->Update();
}

//�����ݏ���
void PLAYER::Draw() const noexcept(!IS_DEBUG)
{
	m_pModel->Draw();
}
