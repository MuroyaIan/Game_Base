//===== �C���N���[�h�� =====
#include <WinApp/Mouse.h>

#ifdef _WIN64

#include <WinApp/WinConfig.h>

#else

#define WHEEL_DELTA (1)

#endif // _WIN64

//===== �N���X���� =====

//***** �}�E�X�C�x���g ****
CT_MOUSE_EVENTS::CT_MOUSE_EVENTS() noexcept
	: m_Type(ET_MOUSE_STATUS::me_Invalid)
	, m_Info() {}

CT_MOUSE_EVENTS::CT_MOUSE_EVENTS(const ET_MOUSE_STATUS& type, const ST_MOUSE_INFO& parent) noexcept
	: m_Type(type)
	, m_Info(parent) {}

CT_MOUSE_EVENTS::~CT_MOUSE_EVENTS() noexcept = default;

//***** �}�E�X���� *****
CT_MOUSE::CT_MOUSE() noexcept
	: m_Buffer()
	, m_Info()
	, m_WheelDelta(0)
	, m_WheelVal(0)
	, m_RawDeltaBuffer()
	, m_bUseRawInput(false) {}

CT_MOUSE::~CT_MOUSE() noexcept = default;

/**
 * �}�E�XXY���W�擾
 *
 * \param
 * \return std::pair<int, int>
 */
std::pair<int, int> CT_MOUSE::GetPos() const noexcept
{
	return {m_Info.ms_PosX, m_Info.ms_PosY};
}

/**
 * �}�E�XX���W�擾
 *
 * \param
 * \return int
 */
int CT_MOUSE::GetPosX() const noexcept
{
	return m_Info.ms_PosX;
}

/**
 * �}�E�XY���W�擾
 *
 * \param
 * \return int
 */
int CT_MOUSE::GetPosY() const noexcept
{
	return m_Info.ms_PosY;
}

/**
 * �z�C�[���l�擾
 *
 * \param
 * \return int
 */
int CT_MOUSE::GetWheelVal() const noexcept
{
	return m_WheelVal;
}

/**
 * �E�B���h�E���ɂ��邩�m�F
 *
 * \param
 * \return bool
 */
bool CT_MOUSE::IsInWindow() const noexcept
{
	return m_Info.ms_bIsInWindow;
}

/**
 * �}�E�X���N���b�N�m�F
 *
 * \param
 * \return bool
 */
bool CT_MOUSE::LeftIsPressed() const noexcept
{
	return m_Info.ms_bLeftIsPressed;
}

/**
 * �}�E�X�E�N���b�N�m�F
 *
 * \param
 * \return bool
 */
bool CT_MOUSE::RightIsPressed() const noexcept
{
	return m_Info.ms_bRightIsPressed;
}

/**
 * �o�b�t�@�Ǎ���
 *
 * \param
 * \return CT_MOUSE_EVENTS
 */
CT_MOUSE_EVENTS CT_MOUSE::ReadBuffer() noexcept
{
	//��O����
	if (m_Buffer.empty())
		return CT_MOUSE_EVENTS{};

	const CT_MOUSE_EVENTS l_Event = m_Buffer.front(); //���̃C�x���g�ɃA�N�Z�X
	m_Buffer.pop();                                   //�C�x���g���폜����
	return l_Event;
}

/**
 * �o�b�t�@�̋󂫊m�F
 *
 * \param
 * \return bool
 */
bool CT_MOUSE::IsEmpty() const noexcept
{
	return m_Buffer.empty();
}

/**
 * �o�b�t�@�N���A
 *
 * \param
 * \return void
 */
void CT_MOUSE::ClearBuffer() noexcept
{
	m_Buffer = std::queue<CT_MOUSE_EVENTS>{};
}

/**
 * RawInput�o�b�t�@�Ǎ���
 *
 * \param
 * \return CT_MOUSE::ST_RAW_DELTA
 */
CT_MOUSE::ST_RAW_DELTA CT_MOUSE::ReadRawDelta() noexcept
{
	//��O����(�L���[����ɂȂ���)
	if (m_RawDeltaBuffer.empty())
		return ST_RAW_DELTA{true};

	const ST_RAW_DELTA l_Data = m_RawDeltaBuffer.front(); //���̃C�x���g�ɃA�N�Z�X
	m_RawDeltaBuffer.pop();                               //�C�x���g���폜����
	return l_Data;
}

/**
 * RawInput�g�p����
 *
 * \param bUse
 * \return void
 */
void CT_MOUSE::SetRawInput(const bool& bUse) noexcept
{
	m_bUseRawInput = bUse;
}

/**
 * RawInput�g�p��Ԋm�F
 *
 * \param
 * \return void
 */
bool CT_MOUSE::IsUsingRawInput() const noexcept
{
	return m_bUseRawInput;
}

/**
 * �o�b�t�@�؂�̂�
 *
 * \param
 * \return void
 */
void CT_MOUSE::TruncateBuffer() noexcept
{
	while (m_Buffer.size() > c_BufferSize) //����T�C�Y�Ɏ��܂�܂�
		m_Buffer.pop();                    //�L���[�|�b�v
}

/**
 * �}�E�X�ړ�
 *
 * \param posX
 * \param posY
 * \return void
 */
void CT_MOUSE::MouseMove(const int& posX, const int& posY) noexcept
{
	m_Info.ms_PosX = posX;
	m_Info.ms_PosY = posY;										//���W�i�[
	m_Buffer.emplace(ET_MOUSE_STATUS::me_Move, m_Info);		//�L���[�v�b�V��
	TruncateBuffer();											//�o�b�t�@����Ǘ�
}

/**
 * �E�B���h�E�O�ɍs��
 *
 * \param
 * \return void
 */
void CT_MOUSE::LeaveWindow() noexcept
{
	m_Info.ms_bIsInWindow = false;
	m_Buffer.emplace(ET_MOUSE_STATUS::me_LeaveWindow, m_Info);
	TruncateBuffer();
}

/**
 * �E�B���h�E���ɓ���
 *
 * \param
 * \return void
 */
void CT_MOUSE::EnterWindow() noexcept
{
	m_Info.ms_bIsInWindow = true;
	m_Buffer.emplace(ET_MOUSE_STATUS::me_EnterWindow, m_Info);
	TruncateBuffer();
}

/**
 * ���N���b�N�I��
 *
 * \param
 * \return void
 */
void CT_MOUSE::LeftPressed() noexcept
{
	m_Info.ms_bLeftIsPressed = true;
	m_Buffer.emplace(ET_MOUSE_STATUS::me_L_Press, m_Info);
	TruncateBuffer();
}

/**
 * ���N���b�N�I�t
 *
 * \param
 * \return void
 */
void CT_MOUSE::LeftReleased() noexcept
{
	m_Info.ms_bLeftIsPressed = false;
	m_Buffer.emplace(ET_MOUSE_STATUS::me_L_Release, m_Info);
	TruncateBuffer();
}

/**
 * �E�N���b�N�I��
 *
 * \param
 * \return void
 */
void CT_MOUSE::RightPressed() noexcept
{
	m_Info.ms_bRightIsPressed = true;
	m_Buffer.emplace(ET_MOUSE_STATUS::me_R_Press, m_Info);
	TruncateBuffer();
}

/**
 * �E�N���b�N�I�t
 *
 * \param
 * \return void
 */
void CT_MOUSE::RightReleased() noexcept
{
	m_Info.ms_bRightIsPressed = false;
	m_Buffer.emplace(ET_MOUSE_STATUS::me_R_Release, m_Info);
	TruncateBuffer();
}

/**
 * �z�C�[���A�b�v
 *
 * \param
 * \return void
 */
void CT_MOUSE::WheelUp() noexcept
{
	m_Buffer.emplace(ET_MOUSE_STATUS::me_WheelUp, m_Info);
	TruncateBuffer();
	m_WheelVal++;
}

/**
 * �z�C�[���_�E��
 *
 * \param
 * \return void
 */
void CT_MOUSE::WheelDown() noexcept
{
	m_Buffer.emplace(ET_MOUSE_STATUS::me_WheelDown, m_Info);
	TruncateBuffer();
	m_WheelVal--;
}

/**
 * �z�C�[������
 *
 * \param nDelta
 * \return void
 */
void CT_MOUSE::WheelProc(const int& nDelta) noexcept
{
	m_WheelDelta += nDelta;

	//�z�C�[���������x����(1��̃��b�Z�[�W�ŉ���)
	static int l_NResponse = WHEEL_DELTA * 1;
	if (m_WheelDelta >= l_NResponse)
	{
		m_WheelDelta = 0;
		WheelUp();
	}
	if (m_WheelDelta <= -l_NResponse)
	{
		m_WheelDelta = 0;
		WheelDown();
	}
}

/**
 * RawInput�o�b�t�@�؂�̂�
 *
 * \param
 * \return void
 */
void CT_MOUSE::TruncateRawInputBuffer() noexcept
{
	while (m_RawDeltaBuffer.size() > c_BufferSize) //����T�C�Y�Ɏ��܂�܂�
		m_RawDeltaBuffer.pop();                    //�L���[�|�b�v
}

/**
 * RawInput���擾
 *
 * \param dx
 * \param dy
 * \return void
 */
void CT_MOUSE::GetRawDelta(const int& dx, const int& dy) noexcept
{
	m_RawDeltaBuffer.emplace(dx, dy);
	TruncateRawInputBuffer();
}
