
//===== �C���N���[�h�� =====
#include <Tool/Input/IM_Mouse.h>
#include <App.h>
#include <Tool/Input/IM_Keyboard.h>

//===== �N���X���� =====
INPUT_CURSOR::INPUT_CURSOR(APP& App, INPUT_KB& KB) noexcept :
	m_Window(App.GetWindowProc()), m_Mouse(App.GetWindowProc().m_Mouse), m_KB(KB), MoveVal()
{
}

INPUT_CURSOR::~INPUT_CURSOR() noexcept
{
}

//�v���X����
bool INPUT_CURSOR::GetPress(int nBtn) const noexcept
{
	return m_KB.GetPress(nBtn);
}

//�g���K�[����
bool INPUT_CURSOR::GetTrigger(int nBtn) const noexcept
{
	return m_KB.GetTrigger(nBtn);
}

//�����[�X����
bool INPUT_CURSOR::GetRelease(int nBtn) const noexcept
{
	return m_KB.GetRelease(nBtn);
}

//���s�[�g����
bool INPUT_CURSOR::GetRepeat(int nBtn, float fInterval)
{
	return m_KB.GetRepeat(nBtn, fInterval);
}

//���W�擾
DirectX::XMFLOAT2 INPUT_CURSOR::GetPos() const noexcept
{
	auto [x, y] = m_Mouse.GetPos();
	return DirectX::XMFLOAT2(static_cast<float>(x), static_cast<float>(y));
}

//�z�C�[���l�擾
int INPUT_CURSOR::GetWheelVal() const noexcept
{
	return m_Mouse.GetWheelVal();
}

//�}�E�X�\��
void INPUT_CURSOR::Show() const noexcept
{
	m_Window.EnableCursor();
}

//�}�E�X�s�\��
void INPUT_CURSOR::Hide() const noexcept
{
	m_Window.DisableCursor();
}

//�}�E�X�`���Ԋm�F
bool INPUT_CURSOR::IsDrawing() const noexcept
{
	return m_Window.IsUsingCursor();
}

//�}�E�X�g�p
void INPUT_CURSOR::EnableRawInput() const noexcept
{
	m_Window.m_Mouse.SetRawInput(true);
}

//�}�E�X�s�g�p
void INPUT_CURSOR::DisableRawInput() const noexcept
{
	m_Window.m_Mouse.SetRawInput(false);
}

//�X�V����
void INPUT_CURSOR::Update() noexcept
{
	//�}�E�X�ړ��ʎ擾
	MoveVal.x = 0;
	MoveVal.y = 0;						//�ړ��ʂ�0�N���A
	if (!m_Mouse.IsUsingRawInput())		//RawInput�s�g�p�̏ꍇ�A���������O
		return;
	while (true) {
		const auto d = m_Mouse.ReadRawDelta();

		//�E�o����(�ړ��ʂ����ׂĎ擾)
		if (d.bClear)
			break;

		//�f���^���̈ړ��ʂ�S�����Z
		MoveVal.x += d.x;
		MoveVal.y += d.y;
	}
}
