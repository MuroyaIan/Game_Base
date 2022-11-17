
//===== �C���N���[�h�� =====
#include <Tool/Input/IM_GamePad.h>
#include <App.h>

namespace dx = DirectX;

//===== �ǉ����C�u���� =====
#pragma comment(lib, "xinput")

//===== �N���X���� =====
INPUT_PAD::INPUT_PAD(APP& App) noexcept :
	m_App(App), m_State(), m_PadUsed(), m_Vibration(), m_bUseVibration(),
	m_Press(), m_PrevPress(), m_Trigger(), m_Release(), m_Repeat(), m_RepeatCnt(), m_RepeatInterval(),
	m_pPress(nullptr), m_pPrevPress(nullptr), m_pTrigger(nullptr), m_pRelease(nullptr), m_pRepeat(nullptr), m_pRepeatCnt(nullptr), m_pRepeatInterval(nullptr),
	m_Val_L2(), m_Val_R2(), m_Val_L3(), m_Val_R3(), m_Vibe_L(), m_Vibe_R()
{
	//�p�b�h��񏉊���
	ZeroMemory(m_State, sizeof(m_State));

	//����p��񏉊���
	uShort RepeatInterval = static_cast<uShort>(m_App.GetTimeProc().GetTargetFPS() * 0.5f);		//���s�[�g�Ԋu�i�f�t�H���g0.5�b�j
	m_pRepeatInterval = m_RepeatInterval[0];
	for (int Cnt = 0; Cnt < MAX_PAD_NUM * PAD_BTN_NUM; Cnt++) {
		*m_pRepeatInterval = RepeatInterval;
		m_pRepeatInterval++;
	}

	//�U��������
	for (auto& uv: m_bUseVibration)
		uv = true;
}

INPUT_PAD::~INPUT_PAD() noexcept
{
	//�|�C���^���
	m_pPress = nullptr;
	m_pPrevPress = nullptr;
	m_pTrigger = nullptr;
	m_pRelease = nullptr;
	m_pRepeat = nullptr;
	m_pRepeatCnt = nullptr;
	m_pRepeatInterval = nullptr;
}

//���͏��X�V
void INPUT_PAD::UpdateInputData(int nPadID) noexcept
{
	//�|�C���^���Z�b�g
	m_pPress = m_Press[nPadID];

	//���͏��i�[�i�{�^���j
	for (int Cnt = 0; Cnt < PAD_BTN_NUM; Cnt++, m_pPress++) {

		//��O�����i�g���K�[�j
		if (Cnt == DS4_L2) {
			if (m_State[nPadID].Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
				*m_pPress = 0x8000;
			else
				*m_pPress = 0x0000;
			continue;
		}
		else if (Cnt == DS4_R2) {
			if (m_State[nPadID].Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
				*m_pPress = 0x8000;
			else
				*m_pPress = 0x0000;
			continue;
		}

		//�v���X����
		if (m_State[nPadID].Gamepad.wButtons & (0x0001 << Cnt))
			*m_pPress = 0x8000;
		else
			*m_pPress = 0x0000;
	}

	//���͏��i�[�i�g���K�[�j
	m_Val_L2[nPadID] = m_State[nPadID].Gamepad.bLeftTrigger / 255.0f;
	m_Val_R2[nPadID] = m_State[nPadID].Gamepad.bRightTrigger / 255.0f;

	//���͏��i�[�i�X�e�B�b�N�j
	if (m_State[nPadID].Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && m_State[nPadID].Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		m_Val_L3[nPadID].x = 0.0f;
	else
		m_Val_L3[nPadID].x = m_State[nPadID].Gamepad.sThumbLX / static_cast<float>(0x8000);
	if (m_State[nPadID].Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && m_State[nPadID].Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		m_Val_L3[nPadID].y = 0.0f;
	else
		m_Val_L3[nPadID].y = m_State[nPadID].Gamepad.sThumbLY / static_cast<float>(0x8000);
	if (m_State[nPadID].Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && m_State[nPadID].Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		m_Val_R3[nPadID].x = 0.0f;
	else
		m_Val_R3[nPadID].x = m_State[nPadID].Gamepad.sThumbRX / static_cast<float>(0x8000);
	if (m_State[nPadID].Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && m_State[nPadID].Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		m_Val_R3[nPadID].y = 0.0f;
	else
		m_Val_R3[nPadID].y = m_State[nPadID].Gamepad.sThumbRY / static_cast<float>(0x8000);
}

//�X�V����
void INPUT_PAD::Update() noexcept
{
	//PAD��Ԏ擾
	for (int Cnt = 0; Cnt < MAX_PAD_NUM; Cnt++) {

		//��O�����i���ڑ��j
		m_PadUsed[Cnt] = XInputGetState(Cnt, &m_State[Cnt]);	//���͏��擾
		if (m_PadUsed[Cnt] != ERROR_SUCCESS)
			continue;

		//���͏��X�V
		UpdateInputData(Cnt);

		//�|�C���^���Z�b�g
		m_pPress = m_Press[Cnt];
		m_pPrevPress = m_PrevPress[Cnt];
		m_pTrigger = m_Trigger[Cnt];
		m_pRelease = m_Release[Cnt];
		m_pRepeat = m_Repeat[Cnt];
		m_pRepeatCnt = m_RepeatCnt[Cnt];
		m_pRepeatInterval = m_RepeatInterval[Cnt];

		//������
		for (int Btn = 0; Btn < PAD_BTN_NUM; Btn++) {

			//������Z�o
			*m_pTrigger = (*m_pPrevPress ^ *m_pPress) & *m_pPress;			//�g���K�[���
			*m_pRelease = (*m_pPrevPress ^ *m_pPress) & ~*m_pPress;			//�����[�X���
			*m_pRepeat = *m_pTrigger;										//���s�[�g���

			//���s�[�g����
			if (*m_pPress & 0x8000) {										//�����������
				*m_pRepeatCnt += 1;
				if (!(*m_pRepeatCnt % *m_pRepeatInterval))					//�w�莞�Ԍ�
					*m_pRepeat = *m_pPress;									//0x8000���Z�b�g�i���s�[�g���j
				if (*m_pRepeatCnt >= 0xFFFF - 0xFFFF % *m_pRepeatInterval)	//�����������
					*m_pRepeatCnt = 0;
			}
			else {
				*m_pRepeatCnt = 0;
				*m_pRepeat = 0x0000;	//���Z�b�g����
			}

			//���L�^
			*m_pPrevPress = *m_pPress;	//�������̃o�b�N�A�b�v�@�ˁ@����̃r�b�g���Z�p

			//�|�C���^�X�V
			m_pPress++;
			m_pPrevPress++;
			m_pTrigger++;
			m_pRelease++;
			m_pRepeat++;
			m_pRepeatCnt++;
			m_pRepeatInterval++;
		}

		//�U���X�V
		if (m_bUseVibration[Cnt]) {
			m_Vibration[Cnt].wLeftMotorSpeed = static_cast<WORD>(m_Vibe_L[Cnt] * 65535);
			m_Vibration[Cnt].wRightMotorSpeed = static_cast<WORD>(m_Vibe_R[Cnt] * 65535);
			XInputSetState(Cnt, &m_Vibration[Cnt]);
		}
	}
}

//�R���g���[���[�ڑ��m�F
bool INPUT_PAD::CheckPad(eGAMEPAD_ID nPadID) const noexcept
{
	if (m_PadUsed[nPadID] == ERROR_SUCCESS)
		return true;
	else
		return false;
}

//�v���X����
bool INPUT_PAD::GetPress(eGAMEPAD_ID nPadID, int nBtn) const noexcept
{
	if (m_PadUsed[nPadID] != ERROR_SUCCESS)		//��O�����i���ڑ��j
		return false;
	else
		return (static_cast<int>(m_Press[nPadID][nBtn]) & 0x8000) ? true : false;
}

//�g���K�[����
bool INPUT_PAD::GetTrigger(eGAMEPAD_ID nPadID, int nBtn) const noexcept
{
	if (m_PadUsed[nPadID] != ERROR_SUCCESS)		//��O�����i���ڑ��j
		return false;
	else
		return (static_cast<int>(m_Trigger[nPadID][nBtn]) & 0x8000) ? true : false;
}

//�����[�X����
bool INPUT_PAD::GetRelease(eGAMEPAD_ID nPadID, int nBtn) const noexcept
{
	if (m_PadUsed[nPadID] != ERROR_SUCCESS)		//��O�����i���ڑ��j
		return false;
	else
		return (static_cast<int>(m_Release[nPadID][nBtn]) & 0x8000) ? true : false;
}

//���s�[�g����
bool INPUT_PAD::GetRepeat(eGAMEPAD_ID nPadID, int nBtn, float fInterval)
{
	if (m_PadUsed[nPadID] != ERROR_SUCCESS)		//��O�����i���ڑ��j
		return false;
	else {

		//��O����
		if (fInterval <= 0.0f)
			throw ERROR_EX2("���s�[�g�Ԋu��0�ȏ�ɂ��Ă��������B");

		m_RepeatInterval[nPadID][nBtn] = static_cast<uShort>(m_App.GetTimeProc().GetTargetFPS() * fInterval);	//���s�[�g�Ԋu�X�V
		return (static_cast<int>(m_Repeat[nPadID][nBtn]) & 0x8000) ? true : false;
	}
}

//�U���I���E�I�t
void INPUT_PAD::UseVibration(eGAMEPAD_ID nPadID, bool bStart) noexcept
{
	if (bStart)
		m_bUseVibration[nPadID] = true;
	else {
		m_bUseVibration[nPadID] = false;
		m_Vibe_L[nPadID] = 0.0f;
		m_Vibe_R[nPadID] = 0.0f;
		m_Vibration[nPadID] = { 0, 0 };
		XInputSetState(nPadID, &m_Vibration[nPadID]);
	}
}

//�U�����x�ݒ�
void INPUT_PAD::SetVibration(eGAMEPAD_ID nPadID, float fPower)
{
	//��O����
	if (fPower < 0.0f || fPower > 1.0f)
		throw ERROR_EX2("�U���l��0�`1�̊Ԃł��B");

	m_Vibe_L[nPadID] = fPower;
	m_Vibe_R[nPadID] = fPower;
}

void INPUT_PAD::SetVibration_L(eGAMEPAD_ID nPadID, float fPower)
{
	//��O����
	if (fPower < 0.0f || fPower > 1.0f)
		throw ERROR_EX2("�U���l��0�`1�̊Ԃł��B");

	m_Vibe_L[nPadID] = fPower;
}

void INPUT_PAD::SetVibration_R(eGAMEPAD_ID nPadID, float fPower)
{
	//��O����
	if (fPower < 0.0f || fPower > 1.0f)
		throw ERROR_EX2("�U���l��0�`1�̊Ԃł��B");

	m_Vibe_R[nPadID] = fPower;
}
