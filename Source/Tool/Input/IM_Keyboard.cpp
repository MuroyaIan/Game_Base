
//===== �C���N���[�h�� =====
#include <Tool/Input/IM_Keyboard.h>
#include <App.h>

//===== �N���X���� =====
INPUT_KB::INPUT_KB(APP& App) noexcept :
	m_App(App), m_State(0x0000),
	m_Press(), m_Trigger(), m_Release(), m_Repeat(), m_RepeatCnt(),m_RepeatInterval(),
	m_pPress(nullptr), m_pTrigger(nullptr), m_pRelease(nullptr), m_pRepeat(nullptr), m_pRepeatCnt(nullptr), m_pRepeatInterval(nullptr)
{
	//����p��񏉊���
	uShort RepeatInterval = static_cast<uShort>(m_App.GetTimeProc().GetTargetFPS() * 0.5f);		//���s�[�g�Ԋu�i�f�t�H���g0.5�b�j
	for (auto& ri : m_RepeatInterval)
		ri = RepeatInterval;
}

INPUT_KB::~INPUT_KB() noexcept
{
	//�|�C���^���
	m_pPress = nullptr;
	m_pTrigger = nullptr;
	m_pRelease = nullptr;
	m_pRepeat = nullptr;
	m_pRepeatCnt = nullptr;
	m_pRepeatInterval = nullptr;
}

//�X�V����
void INPUT_KB::Update() noexcept
{
	//�|�C���^���Z�b�g
	m_pPress = m_Press;
	m_pTrigger = m_Trigger;
	m_pRelease = m_Release;
	m_pRepeat = m_Repeat;
	m_pRepeatCnt = m_RepeatCnt;
	m_pRepeatInterval = m_RepeatInterval;

	//�L�[�{�[�h��Ԏ擾
	for (int Cnt = 0; Cnt < MAX_NUM_VK; Cnt++) {
		m_State = GetAsyncKeyState(Cnt);								//�L�[�̏����擾

		//������Z�o
		*m_pTrigger = (*m_pPress ^ m_State) & m_State;					//�g���K�[���
		*m_pRelease = (*m_pPress ^ m_State) & ~m_State;					//�����[�X���
		*m_pRepeat = *m_pTrigger;										//���s�[�g���

		//���s�[�g����
		if (m_State & 0x8000) {											//�����������
			*m_pRepeatCnt += 1;
			if (!(*m_pRepeatCnt % *m_pRepeatInterval))					//�w�莞�Ԍ�
				*m_pRepeat = m_State;									//0x8000���Z�b�g�i���s�[�g���j
			if (*m_pRepeatCnt >= 0xFFFF - 0xFFFF % *m_pRepeatInterval)	//�J�E���^����ݒ�
				*m_pRepeatCnt = 0;
		}
		else {
			*m_pRepeatCnt = 0;
			*m_pRepeat = 0x0000;	//���Z�b�g����
		}

		//���L�^
		*m_pPress = m_State;		//�������擾�@�ˁ@����̃r�b�g���Z�p

		//�|�C���^�X�V
		m_pPress++;
		m_pTrigger++;
		m_pRelease++;
		m_pRepeat++;
		m_pRepeatCnt++;
		m_pRepeatInterval++;
	}
}

//�v���X����
bool INPUT_KB::GetPress(int nKey) const noexcept
{
	return (static_cast<int>(m_Press[nKey]) & 0x8000) ? true : false;
}

//�g���K�[����
bool INPUT_KB::GetTrigger(int nKey) const noexcept
{
	return (static_cast<int>(m_Trigger[nKey]) & 0x8000) ? true : false;
}

//�����[�X����
bool INPUT_KB::GetRelease(int nKey) const noexcept
{
	return (static_cast<int>(m_Release[nKey]) & 0x8000) ? true : false;
}

//���s�[�g����
bool INPUT_KB::GetRepeat(int nKey, float fInterval)
{
	//��O����
	if (fInterval <= 0.0f)
		throw ERROR_EX2("���s�[�g�Ԋu��0�ȏ�ɂ��Ă��������B");

	m_RepeatInterval[nKey] = static_cast<uShort>(m_App.GetTimeProc().GetTargetFPS() * fInterval);	//���s�[�g�Ԋu�X�V
	return (static_cast<int>(m_Repeat[nKey]) & 0x8000) ? true : false;
}
