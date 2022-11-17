
//===== �C���N���[�h�� =====
#include <WinApp/Mouse.h>

#ifdef _WIN64

#include <WinApp/WinConfig.h>

#else

#define WHEEL_DELTA (1)

#endif // _WIN64

//===== �N���X���� =====

//***** �}�E�X�C�x���g ****
MOUSE_EVENTS::MOUSE_EVENTS() noexcept : m_Type(MOUSE_STATUS::Invalid), m_Info()
{
}

MOUSE_EVENTS::MOUSE_EVENTS(MOUSE_STATUS Type, const MOUSE_INFO& Parent) noexcept :
	m_Type(Type), m_Info(Parent)
{
}

MOUSE_EVENTS::~MOUSE_EVENTS() noexcept
{
}

//***** �}�E�X���� *****
MOUSE::MOUSE() noexcept : m_Buffer(), m_Info(), m_WheelDelta(0), m_WheelVal(0),
	m_RawDeltaBuffer(), m_bUseRawInput(false)
{
}

MOUSE::~MOUSE() noexcept
{
}

//�}�E�XXY���W�擾
std::pair<int, int> MOUSE::GetPos() const noexcept
{
	return { m_Info.nPosX, m_Info.nPosY };
}

int MOUSE::GetPosX() const noexcept
{
	return m_Info.nPosX;
}

int MOUSE::GetPosY() const noexcept
{
	return m_Info.nPosY;
}

//�z�C�[���l�擾
int MOUSE::GetWheelVal() const noexcept
{
	return m_WheelVal;
}

//�E�B���h�E���ɂ��邩�m�F
bool MOUSE::IsInWindow() const noexcept
{
	return m_Info.bIsInWindow;
}

//�}�E�X���N���b�N�m�F
bool MOUSE::LeftIsPressed() const noexcept
{
	return m_Info.bLeftIsPressed;
}

//�}�E�X�E�N���b�N�m�F
bool MOUSE::RightIsPressed() const noexcept
{
	return m_Info.bRightIsPressed;
}

//�o�b�t�@�Ǎ���
MOUSE_EVENTS MOUSE::ReadBuffer() noexcept
{
	//��O����
	if (m_Buffer.empty())
		return MOUSE_EVENTS{};

	const MOUSE_EVENTS Event = m_Buffer.front();	//���̃C�x���g�ɃA�N�Z�X
	m_Buffer.pop();									//�C�x���g���폜����
	return Event;
}

//�o�b�t�@�̋󂫊m�F
bool MOUSE::IsEmpty() const noexcept
{
	return m_Buffer.empty();
}

//�o�b�t�@�N���A
void MOUSE::ClearBuffer() noexcept
{
	m_Buffer = std::queue<MOUSE_EVENTS>{};
}

//RawInput�o�b�t�@�Ǎ���
MOUSE::RAW_DELTA MOUSE::ReadRawDelta() noexcept
{
	//��O����
	if (m_RawDeltaBuffer.empty())
		return RAW_DELTA{};

	const RAW_DELTA Data = m_RawDeltaBuffer.front();	//���̃C�x���g�ɃA�N�Z�X
	m_RawDeltaBuffer.pop();								//�C�x���g���폜����
	return Data;
}

//RawInput�g�p����
void MOUSE::SetRawInput(bool bUse) noexcept
{
	m_bUseRawInput = bUse;
}

//RawInput�g�p��Ԋm�F
bool MOUSE::IsUsingRawInput() const noexcept
{
	return m_bUseRawInput;
}

//�o�b�t�@�؂�̂�
void MOUSE::TruncateBuffer() noexcept
{
	while (m_Buffer.size() > nBufferSize)	//����T�C�Y�Ɏ��܂�܂�
		m_Buffer.pop();						//�L���[�|�b�v
}

//�}�E�X�ړ�
void MOUSE::MouseMove(int PosX, int PosY) noexcept
{
	m_Info.nPosX = PosX;
	m_Info.nPosY = PosY;							//���W�i�[
	m_Buffer.push({ MOUSE_STATUS::Move, m_Info });	//�L���[�v�b�V��
	TruncateBuffer();								//�o�b�t�@����Ǘ�
}

//�E�B���h�E�O�ɍs��
void MOUSE::LeaveWindow() noexcept
{
	m_Info.bIsInWindow = false;
	m_Buffer.push({ MOUSE_STATUS::LeaveWindow, m_Info });
	TruncateBuffer();
}

//�E�B���h�E���ɓ���
void MOUSE::Enterwindow() noexcept
{
	m_Info.bIsInWindow = true;
	m_Buffer.push({ MOUSE_STATUS::EnterWindow, m_Info });
	TruncateBuffer();
}

//���N���b�N�I��
void MOUSE::LeftPressed() noexcept
{
	m_Info.bLeftIsPressed = true;
	m_Buffer.push({ MOUSE_STATUS::L_Press, m_Info });
	TruncateBuffer();
}

//���N���b�N�I�t
void MOUSE::LeftReleased() noexcept
{
	m_Info.bLeftIsPressed = false;
	m_Buffer.push({ MOUSE_STATUS::L_Release, m_Info });
	TruncateBuffer();
}

//�E�N���b�N�I��
void MOUSE::RightPressed() noexcept
{
	m_Info.bRightIsPressed = true;
	m_Buffer.push({ MOUSE_STATUS::R_Press, m_Info });
	TruncateBuffer();
}

//�E�N���b�N�I�t
void MOUSE::RightReleased() noexcept
{
	m_Info.bRightIsPressed = false;
	m_Buffer.push({ MOUSE_STATUS::R_Release, m_Info });
	TruncateBuffer();
}

//�z�C�[���A�b�v
void MOUSE::WheelUp() noexcept
{
	m_Buffer.push({ MOUSE_STATUS::WheelUp, m_Info });
	TruncateBuffer();
	m_WheelVal++;
}

//�z�C�[���_�E��
void MOUSE::WheelDown() noexcept
{
	m_Buffer.push({ MOUSE_STATUS::WheelDown, m_Info });
	TruncateBuffer();
	m_WheelVal--;
}

//�z�C�[������
void MOUSE::WheelProc(int nDelta) noexcept
{
	m_WheelDelta += nDelta;

	//�z�C�[���������x����(1��̃��b�Z�[�W�ŉ���)
	static int nResponse = WHEEL_DELTA * 1;
	if (m_WheelDelta >= nResponse) {
		m_WheelDelta = 0;
		WheelUp();
	}
	if (m_WheelDelta <= -nResponse) {
		m_WheelDelta = 0;
		WheelDown();
	}
}

//RawInput�o�b�t�@�؂�̂�
void MOUSE::TruncateRawInputBuffer() noexcept
{
	while (m_RawDeltaBuffer.size() > nBufferSize)	//����T�C�Y�Ɏ��܂�܂�
		m_RawDeltaBuffer.pop();						//�L���[�|�b�v
}

//RawInput���擾
void MOUSE::GetRawDelta(int dx, int dy) noexcept
{
	m_RawDeltaBuffer.push({ dx, dy });
	TruncateRawInputBuffer();
}
