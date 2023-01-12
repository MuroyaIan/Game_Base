
//===== インクルード部 =====
#include <Geometry/Model/TestPlayer.h>
#include <Geometry/Model/Model.h>

//===== クラス実装 =====
PLAYER::PLAYER(APP& App) noexcept : m_pModel(), m_Input(App.GetInputMgr())
{
	m_pModel = std::make_unique<MODEL>(App, MODEL_MGR::MODEL_ID::Tarantella);
	for (size_t i = 0; i < 100; i++)
		m_pModel->AddInstance();
}

PLAYER::~PLAYER() noexcept
{
}

//更新処理
void PLAYER::Update() noexcept
{
	if (m_Input.m_KB.GetPress(VK_E))
		m_pModel->ChangeAnimID(2);
	else
		m_pModel->ChangeAnimID(1);

	m_pModel->Update();
}

//書込み処理
void PLAYER::Draw() const noexcept(!IS_DEBUG)
{
	m_pModel->Draw();
}
