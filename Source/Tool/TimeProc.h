/**
 * @file TimeProc.h
 * @brief ���Ԍv������
 * @author ���J�C�A��
 * @date 2022/06/10
 * @���� 2022/06/10�F�t�@�C���쐬
 */

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include <chrono>					//���Ԍv��

namespace TimeProc {

	namespace Time = std::chrono;
	using GetTime = Time::system_clock;
	using TimeVal = GetTime::time_point;
}

//===== �N���X��` =====

//***** ���Ԍv���N���X *****
class GAME_TIME
{
public:

	//�^�C�}�N���X
	class Timer
	{
	public:

		//�v���g�^�C�v�錾
		explicit Timer() noexcept;
		~Timer() noexcept;
		void Update(int nFPS) noexcept;		//�X�V����
		void Reset() noexcept;				//�^�C�}���Z�b�g

		int GetTime() const noexcept		//�^�C�}���Ԏ擾
		{
			return m_Time;
		}

	private:

		//�ϐ��錾
		int m_TimerCnt;						//�^�C�}�J�E���^
		int m_Time;							//�^�C�}����
	};

public:

	//�v���g�^�C�v�錾
	explicit GAME_TIME(int FPS = -1) noexcept;
	~GAME_TIME() noexcept;
	bool Update() noexcept;				//�X�V����

	int GetFPS() const noexcept			//FPS�擾(�����l)
	{
		return m_MeasFPS;
	}
	int GetTargetFPS() const noexcept	//FPS�擾(�ڕW�l)
	{
		return m_FPS;
	}

private:

	//�ϐ��錾
	TimeProc::TimeVal m_CurrentTime;	//���ݎ���
	TimeProc::TimeVal m_PrevExecTime;	//�O����s����
	long long m_ExecInterval;			//���s�Ԋu�i�}�C�N���b�j

	TimeProc::TimeVal m_PrevMeasTime;	//�O��FPS�v������
	long long m_MeasInterval;			//FPS�v���Ԋu�i�~���b�j
	int m_MeasCnt;						//�v���J�E���^
	int m_MeasFPS;						//FPS�v���l

	int m_FPS;							//FPS�l
	int m_Limiter;						//FPS�����l�i�}�C�N���b�j
};
