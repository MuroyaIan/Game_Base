
//===== �C���N���[�h�� =====
#include <Tool/Input/IM_Mouse.h>
#include <App.h>
#include <Tool/Input/IM_Keyboard.h>

//===== �N���X���� =====
INPUT_CURSOR::INPUT_CURSOR(APP& App, INPUT_KB& KB) noexcept :
	m_Mouse(App.GetWindowProc().m_Mouse), m_KB(KB), MoveVal()
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

//�ړ��ʎ擾
DirectX::XMINT2 INPUT_CURSOR::GetMoveVal() const noexcept
{
	return MoveVal;
}

//�X�V����
void INPUT_CURSOR::Update() noexcept
{
	//�}�E�X�ړ��ʎ擾
	if (!m_Mouse.IsUsingRawInput())
		return;
	MoveVal.x = 0;
	MoveVal.y = 0;
	while (true) {
		const auto d = m_Mouse.ReadRawDelta();
		MoveVal.x += d.x;
		MoveVal.y += d.y;

		//�E�o����
		if (d.x == 0 && d.y == 0)
			break;
	}
}
