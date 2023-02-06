/**
 * @file TimeProc.h
 * @brief 時間計測処理
 * @author 室谷イアン
 * @date 2022/06/10
 * @履歴 2022/06/10：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <chrono>					//時間計測

namespace TimeProc {

	namespace Time = std::chrono;
	using GetTime = Time::system_clock;
	using TimeVal = GetTime::time_point;
}

//===== クラス定義 =====

//***** 時間計測クラス *****
class GAME_TIME
{
public:

	//タイマクラス
	class Timer
	{
	public:

		//プロトタイプ宣言
		explicit Timer() noexcept;
		~Timer() noexcept;
		void Update(int nFPS) noexcept;		//更新処理
		void Reset() noexcept;				//タイマリセット

		int GetTime() const noexcept		//タイマ時間取得
		{
			return m_Time;
		}

	private:

		//変数宣言
		int m_TimerCnt;						//タイマカウンタ
		int m_Time;							//タイマ時間
	};

public:

	//プロトタイプ宣言
	explicit GAME_TIME(int FPS = -1) noexcept;
	~GAME_TIME() noexcept;
	bool Update() noexcept;				//更新処理

	int GetFPS() const noexcept			//FPS取得(実測値)
	{
		return m_MeasFPS;
	}
	int GetTargetFPS() const noexcept	//FPS取得(目標値)
	{
		return m_FPS;
	}

private:

	//変数宣言
	TimeProc::TimeVal m_CurrentTime;	//現在時刻
	TimeProc::TimeVal m_PrevExecTime;	//前回実行時刻
	long long m_ExecInterval;			//実行間隔（マイクロ秒）

	TimeProc::TimeVal m_PrevMeasTime;	//前回FPS計測時刻
	long long m_MeasInterval;			//FPS計測間隔（ミリ秒）
	int m_MeasCnt;						//計測カウンタ
	int m_MeasFPS;						//FPS計測値

	int m_FPS;							//FPS値
	int m_Limiter;						//FPS制限値（マイクロ秒）
};
