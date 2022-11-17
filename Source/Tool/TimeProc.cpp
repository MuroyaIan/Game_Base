
//===== �C���N���[�h�� =====
#include <Tool/TimeProc.h>

using namespace TimeProc;

//===== �萔�E�}�N����` =====
constexpr int SECOND = 1000;		//1�b�i�~���b�j
constexpr int HALF_SECOND = 500;	//0.5�b�i�~���b�j
constexpr int DEFAULT_FPS = 60;		//�f�t�H���gFPS

//===== �N���X���� =====

//***** ���Ԍv���N���X *****
GAME_TIME::GAME_TIME() noexcept
{
	//���Ԍv���p
	m_CurrentTime = GetTime::now();		//�����������擾
	m_PrevExecTime = m_CurrentTime;		//�����������̃o�b�N�A�b�v
	m_ExecInterval = 0;

	//FPS�v�Z�p
	m_PrevMeasTime = m_CurrentTime;		//�����������̃o�b�N�A�b�v
	m_MeasInterval = 0;
	m_MeasCnt = 0;
	m_MeasFPS = 0;

	//FPS�ݒ�
	m_FPS = DEFAULT_FPS;
	m_Limiter = static_cast<int>(1000.0f * SECOND / m_FPS);
}

GAME_TIME::GAME_TIME(int FPS) noexcept
{
	//���Ԍv���p
	m_CurrentTime = GetTime::now();		//�����������擾
	m_PrevExecTime = m_CurrentTime;		//�����������̃o�b�N�A�b�v
	m_ExecInterval = 0;

	//FPS�v�Z�p
	m_PrevMeasTime = m_CurrentTime;		//�����������̃o�b�N�A�b�v
	m_MeasInterval = 0;
	m_MeasCnt = 0;
	m_MeasFPS = 0;

	//FPS�ݒ�
	m_FPS = FPS;
	m_Limiter = static_cast<int>(1000.0f * SECOND / m_FPS);
}

GAME_TIME::~GAME_TIME() noexcept
{
}

//�X�V����
bool GAME_TIME::Update() noexcept
{
	//�v������
	m_CurrentTime = GetTime::now();																		//���ݎ����擾
	m_ExecInterval = Time::duration_cast<Time::microseconds>(m_CurrentTime - m_PrevExecTime).count();
	m_MeasInterval = Time::duration_cast<Time::milliseconds>(m_CurrentTime - m_PrevMeasTime).count();	//�o�ߎ��ԎZ�o

	//FPS�v�Z�i���x�~���b�j
	if (static_cast<int>(m_MeasInterval) >= HALF_SECOND) {										//�v�Z�p�x�@�ˁ@0.5�b/��
		m_MeasFPS = static_cast<int>(m_MeasCnt * SECOND / static_cast<float>(m_MeasInterval));	//FPS�v�Z
		m_PrevMeasTime = m_CurrentTime;															//�o�b�N�A�b�v�X�V
		m_MeasCnt = 0;																			//�J�E���^���Z�b�g
	}

	//FPS�`�F�b�N�i���x�}�C�N���b�j
	if (static_cast<int>(m_ExecInterval) >= m_Limiter) {
		m_PrevExecTime = m_CurrentTime;			//�o�b�N�A�b�v�X�V
		m_MeasCnt++;							//�J�E���^���Z
		return true;
	}
	else
		return false;
}

//***** �^�C�}�N���X *****
GAME_TIME::Timer::Timer() noexcept : m_TimerCnt(0), m_Time(0)
{
}
GAME_TIME::Timer::~Timer() noexcept
{
}

//�X�V����
void GAME_TIME::Timer::Update(int nFPS) noexcept
{
	m_TimerCnt++;
	if (m_TimerCnt >= nFPS) {
		m_TimerCnt = 0;
		m_Time++;
	}
}

//�^�C�}���Z�b�g
void GAME_TIME::Timer::Reset() noexcept
{
	m_TimerCnt = 0;
	m_Time = 0;
}
