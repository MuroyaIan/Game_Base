
//===== �C���N���[�h�� =====
#include <Tool/Input/InputMgr.h>
#include <App_Win64.h>

//===== �N���X���� =====
INPUT_MGR::INPUT_MGR(APP_64& App) noexcept :
	m_KB(App), m_Mouse(App, m_KB), m_GamePad(App)
{
}

INPUT_MGR::~INPUT_MGR() noexcept
{
}

//�X�V����
void INPUT_MGR::Update() noexcept
{
	//�L�[�{�[�h�X�V
	m_KB.Update();

	//�}�E�X�X�V
	m_Mouse.Update();

	//�R���g���[���X�V
	m_GamePad.Update();
}
