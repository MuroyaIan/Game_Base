
//===== インクルード部 =====
#include <Tool/Input/IM_Keyboard.h>
#include <App.h>

//===== クラス実装 =====
INPUT_KB::INPUT_KB(APP& App) noexcept :
	m_App(App), m_State(0x0000),
	m_Press(), m_Trigger(), m_Release(), m_Repeat(), m_RepeatCnt(),m_RepeatInterval(),
	m_pPress(nullptr), m_pTrigger(nullptr), m_pRelease(nullptr), m_pRepeat(nullptr), m_pRepeatCnt(nullptr), m_pRepeatInterval(nullptr)
{
	//判定用情報初期化
	uShort RepeatInterval = static_cast<uShort>(m_App.GetTimeProc().GetTargetFPS() * 0.5f);		//リピート間隔（デフォルト0.5秒）
	for (auto& ri : m_RepeatInterval)
		ri = RepeatInterval;
}

INPUT_KB::~INPUT_KB() noexcept
{
	//ポインタ解放
	m_pPress = nullptr;
	m_pTrigger = nullptr;
	m_pRelease = nullptr;
	m_pRepeat = nullptr;
	m_pRepeatCnt = nullptr;
	m_pRepeatInterval = nullptr;
}

//更新処理
void INPUT_KB::Update() noexcept
{
	//ポインタリセット
	m_pPress = m_Press;
	m_pTrigger = m_Trigger;
	m_pRelease = m_Release;
	m_pRepeat = m_Repeat;
	m_pRepeatCnt = m_RepeatCnt;
	m_pRepeatInterval = m_RepeatInterval;

	//キーボード状態取得
	for (int Cnt = 0; Cnt < MAX_NUM_VK; Cnt++) {
		m_State = GetAsyncKeyState(Cnt);								//キーの情報を取得

		//判定情報算出
		*m_pTrigger = (*m_pPress ^ m_State) & m_State;					//トリガー情報
		*m_pRelease = (*m_pPress ^ m_State) & ~m_State;					//リリース情報
		*m_pRepeat = *m_pTrigger;										//リピート情報

		//リピート制御
		if (m_State & 0x8000) {											//押し続ける間
			*m_pRepeatCnt += 1;
			if (!(*m_pRepeatCnt % *m_pRepeatInterval))					//指定時間後
				*m_pRepeat = m_State;									//0x8000をセット（リピート許可）
			if (*m_pRepeatCnt >= 0xFFFF - 0xFFFF % *m_pRepeatInterval)	//カウンタ上限設定
				*m_pRepeatCnt = 0;
		}
		else {
			*m_pRepeatCnt = 0;
			*m_pRepeat = 0x0000;	//リセット処理
		}

		//情報記録
		*m_pPress = m_State;		//押下情報取得　⇒　次回のビット演算用

		//ポインタ更新
		m_pPress++;
		m_pTrigger++;
		m_pRelease++;
		m_pRepeat++;
		m_pRepeatCnt++;
		m_pRepeatInterval++;
	}
}

//プレス判定
bool INPUT_KB::GetPress(int nKey) const noexcept
{
	return (static_cast<int>(m_Press[nKey]) & 0x8000) ? true : false;
}

//トリガー判定
bool INPUT_KB::GetTrigger(int nKey) const noexcept
{
	return (static_cast<int>(m_Trigger[nKey]) & 0x8000) ? true : false;
}

//リリース判定
bool INPUT_KB::GetRelease(int nKey) const noexcept
{
	return (static_cast<int>(m_Release[nKey]) & 0x8000) ? true : false;
}

//リピート判定
bool INPUT_KB::GetRepeat(int nKey, float fInterval)
{
	//例外処理
	if (fInterval <= 0.0f)
		throw ERROR_EX2("リピート間隔は0以上にしてください。");

	m_RepeatInterval[nKey] = static_cast<uShort>(m_App.GetTimeProc().GetTargetFPS() * fInterval);	//リピート間隔更新
	return (static_cast<int>(m_Repeat[nKey]) & 0x8000) ? true : false;
}
