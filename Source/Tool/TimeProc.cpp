
//===== インクルード部 =====
#include <Tool/TimeProc.h>

using namespace TimeProc;

//===== 定数・マクロ定義 =====
constexpr int SECOND = 1000;		//1秒（ミリ秒）
constexpr int HALF_SECOND = 500;	//0.5秒（ミリ秒）
constexpr int DEFAULT_FPS = 60;		//デフォルトFPS

//===== クラス実装 =====

//***** 時間計測クラス *****
GAME_TIME::GAME_TIME() noexcept
{
	//時間計測用
	m_CurrentTime = GetTime::now();		//初期化時刻取得
	m_PrevExecTime = m_CurrentTime;		//初期化時刻のバックアップ
	m_ExecInterval = 0;

	//FPS計算用
	m_PrevMeasTime = m_CurrentTime;		//初期化時刻のバックアップ
	m_MeasInterval = 0;
	m_MeasCnt = 0;
	m_MeasFPS = 0;

	//FPS設定
	m_FPS = DEFAULT_FPS;
	m_Limiter = static_cast<int>(1000.0f * SECOND / m_FPS);
}

GAME_TIME::GAME_TIME(int FPS) noexcept
{
	//時間計測用
	m_CurrentTime = GetTime::now();		//初期化時刻取得
	m_PrevExecTime = m_CurrentTime;		//初期化時刻のバックアップ
	m_ExecInterval = 0;

	//FPS計算用
	m_PrevMeasTime = m_CurrentTime;		//初期化時刻のバックアップ
	m_MeasInterval = 0;
	m_MeasCnt = 0;
	m_MeasFPS = 0;

	//FPS設定
	m_FPS = FPS;
	m_Limiter = static_cast<int>(1000.0f * SECOND / m_FPS);
}

GAME_TIME::~GAME_TIME() noexcept
{
}

//更新処理
bool GAME_TIME::Update() noexcept
{
	//計測処理
	m_CurrentTime = GetTime::now();																		//現在時刻取得
	m_ExecInterval = Time::duration_cast<Time::microseconds>(m_CurrentTime - m_PrevExecTime).count();
	m_MeasInterval = Time::duration_cast<Time::milliseconds>(m_CurrentTime - m_PrevMeasTime).count();	//経過時間算出

	//FPS計算（精度ミリ秒）
	if (static_cast<int>(m_MeasInterval) >= HALF_SECOND) {										//計算頻度　⇒　0.5秒/回
		m_MeasFPS = static_cast<int>(m_MeasCnt * SECOND / static_cast<float>(m_MeasInterval));	//FPS計算
		m_PrevMeasTime = m_CurrentTime;															//バックアップ更新
		m_MeasCnt = 0;																			//カウンタリセット
	}

	//FPSチェック（精度マイクロ秒）
	if (static_cast<int>(m_ExecInterval) >= m_Limiter) {
		m_PrevExecTime = m_CurrentTime;			//バックアップ更新
		m_MeasCnt++;							//カウンタ加算
		return true;
	}
	else
		return false;
}

//***** タイマクラス *****
GAME_TIME::Timer::Timer() noexcept : m_TimerCnt(0), m_Time(0)
{
}
GAME_TIME::Timer::~Timer() noexcept
{
}

//更新処理
void GAME_TIME::Timer::Update(int nFPS) noexcept
{
	m_TimerCnt++;
	if (m_TimerCnt >= nFPS) {
		m_TimerCnt = 0;
		m_Time++;
	}
}

//タイマリセット
void GAME_TIME::Timer::Reset() noexcept
{
	m_TimerCnt = 0;
	m_Time = 0;
}
