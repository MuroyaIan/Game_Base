
//===== インクルード部 =====
#include <Tool/Input/IM_GamePad.h>
#include <App.h>

namespace dx = DirectX;

//===== 追加ライブラリ =====
#pragma comment(lib, "xinput")

//===== クラス実装 =====
INPUT_PAD::INPUT_PAD(APP& App) noexcept :
	m_App(App), m_State(), m_PadUsed(), m_Vibration(), m_bUseVibration(),
	m_Press(), m_PrevPress(), m_Trigger(), m_Release(), m_Repeat(), m_RepeatCnt(), m_RepeatInterval(),
	m_pPress(nullptr), m_pPrevPress(nullptr), m_pTrigger(nullptr), m_pRelease(nullptr), m_pRepeat(nullptr), m_pRepeatCnt(nullptr), m_pRepeatInterval(nullptr),
	m_Val_L2(), m_Val_R2(), m_Val_L3(), m_Val_R3(), m_Vibe_L(), m_Vibe_R()
{
	//パッド情報初期化
	ZeroMemory(m_State, sizeof(m_State));

	//判定用情報初期化
	uShort RepeatInterval = static_cast<uShort>(m_App.GetTimeProc().GetTargetFPS() * 0.5f);		//リピート間隔（デフォルト0.5秒）
	m_pRepeatInterval = m_RepeatInterval[0];
	for (int Cnt = 0; Cnt < MAX_PAD_NUM * PAD_BTN_NUM; Cnt++) {
		*m_pRepeatInterval = RepeatInterval;
		m_pRepeatInterval++;
	}

	//振動初期化
	for (auto& uv: m_bUseVibration)
		uv = true;
}

INPUT_PAD::~INPUT_PAD() noexcept
{
	//ポインタ解放
	m_pPress = nullptr;
	m_pPrevPress = nullptr;
	m_pTrigger = nullptr;
	m_pRelease = nullptr;
	m_pRepeat = nullptr;
	m_pRepeatCnt = nullptr;
	m_pRepeatInterval = nullptr;
}

//入力情報更新
void INPUT_PAD::UpdateInputData(int nPadID) noexcept
{
	//ポインタリセット
	m_pPress = m_Press[nPadID];

	//入力情報格納（ボタン）
	for (int Cnt = 0; Cnt < PAD_BTN_NUM; Cnt++, m_pPress++) {

		//例外処理（トリガー）
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

		//プレス判定
		if (m_State[nPadID].Gamepad.wButtons & (0x0001 << Cnt))
			*m_pPress = 0x8000;
		else
			*m_pPress = 0x0000;
	}

	//入力情報格納（トリガー）
	m_Val_L2[nPadID] = m_State[nPadID].Gamepad.bLeftTrigger / 255.0f;
	m_Val_R2[nPadID] = m_State[nPadID].Gamepad.bRightTrigger / 255.0f;

	//入力情報格納（スティック）
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

//更新処理
void INPUT_PAD::Update() noexcept
{
	//PAD状態取得
	for (int Cnt = 0; Cnt < MAX_PAD_NUM; Cnt++) {

		//例外処理（未接続）
		m_PadUsed[Cnt] = XInputGetState(Cnt, &m_State[Cnt]);	//入力情報取得
		if (m_PadUsed[Cnt] != ERROR_SUCCESS)
			continue;

		//入力情報更新
		UpdateInputData(Cnt);

		//ポインタリセット
		m_pPress = m_Press[Cnt];
		m_pPrevPress = m_PrevPress[Cnt];
		m_pTrigger = m_Trigger[Cnt];
		m_pRelease = m_Release[Cnt];
		m_pRepeat = m_Repeat[Cnt];
		m_pRepeatCnt = m_RepeatCnt[Cnt];
		m_pRepeatInterval = m_RepeatInterval[Cnt];

		//判定情報
		for (int Btn = 0; Btn < PAD_BTN_NUM; Btn++) {

			//判定情報算出
			*m_pTrigger = (*m_pPrevPress ^ *m_pPress) & *m_pPress;			//トリガー情報
			*m_pRelease = (*m_pPrevPress ^ *m_pPress) & ~*m_pPress;			//リリース情報
			*m_pRepeat = *m_pTrigger;										//リピート情報

			//リピート制御
			if (*m_pPress & 0x8000) {										//押し続ける間
				*m_pRepeatCnt += 1;
				if (!(*m_pRepeatCnt % *m_pRepeatInterval))					//指定時間後
					*m_pRepeat = *m_pPress;									//0x8000をセット（リピート許可）
				if (*m_pRepeatCnt >= 0xFFFF - 0xFFFF % *m_pRepeatInterval)	//桁数上限調整
					*m_pRepeatCnt = 0;
			}
			else {
				*m_pRepeatCnt = 0;
				*m_pRepeat = 0x0000;	//リセット処理
			}

			//情報記録
			*m_pPrevPress = *m_pPress;	//押下情報のバックアップ　⇒　次回のビット演算用

			//ポインタ更新
			m_pPress++;
			m_pPrevPress++;
			m_pTrigger++;
			m_pRelease++;
			m_pRepeat++;
			m_pRepeatCnt++;
			m_pRepeatInterval++;
		}

		//振動更新
		if (m_bUseVibration[Cnt]) {
			m_Vibration[Cnt].wLeftMotorSpeed = static_cast<WORD>(m_Vibe_L[Cnt] * 65535);
			m_Vibration[Cnt].wRightMotorSpeed = static_cast<WORD>(m_Vibe_R[Cnt] * 65535);
			XInputSetState(Cnt, &m_Vibration[Cnt]);
		}
	}
}

//コントローラー接続確認
bool INPUT_PAD::CheckPad(eGAMEPAD_ID nPadID) const noexcept
{
	if (m_PadUsed[nPadID] == ERROR_SUCCESS)
		return true;
	else
		return false;
}

//プレス判定
bool INPUT_PAD::GetPress(eGAMEPAD_ID nPadID, int nBtn) const noexcept
{
	if (m_PadUsed[nPadID] != ERROR_SUCCESS)		//例外処理（未接続）
		return false;
	else
		return (static_cast<int>(m_Press[nPadID][nBtn]) & 0x8000) ? true : false;
}

//トリガー判定
bool INPUT_PAD::GetTrigger(eGAMEPAD_ID nPadID, int nBtn) const noexcept
{
	if (m_PadUsed[nPadID] != ERROR_SUCCESS)		//例外処理（未接続）
		return false;
	else
		return (static_cast<int>(m_Trigger[nPadID][nBtn]) & 0x8000) ? true : false;
}

//リリース判定
bool INPUT_PAD::GetRelease(eGAMEPAD_ID nPadID, int nBtn) const noexcept
{
	if (m_PadUsed[nPadID] != ERROR_SUCCESS)		//例外処理（未接続）
		return false;
	else
		return (static_cast<int>(m_Release[nPadID][nBtn]) & 0x8000) ? true : false;
}

//リピート判定
bool INPUT_PAD::GetRepeat(eGAMEPAD_ID nPadID, int nBtn, float fInterval)
{
	if (m_PadUsed[nPadID] != ERROR_SUCCESS)		//例外処理（未接続）
		return false;
	else {

		//例外処理
		if (fInterval <= 0.0f)
			throw ERROR_EX2("リピート間隔は0以上にしてください。");

		m_RepeatInterval[nPadID][nBtn] = static_cast<uShort>(m_App.GetTimeProc().GetTargetFPS() * fInterval);	//リピート間隔更新
		return (static_cast<int>(m_Repeat[nPadID][nBtn]) & 0x8000) ? true : false;
	}
}

//振動オン・オフ
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

//振動強度設定
void INPUT_PAD::SetVibration(eGAMEPAD_ID nPadID, float fPower)
{
	//例外処理
	if (fPower < 0.0f || fPower > 1.0f)
		throw ERROR_EX2("振動値は0～1の間です。");

	m_Vibe_L[nPadID] = fPower;
	m_Vibe_R[nPadID] = fPower;
}

void INPUT_PAD::SetVibration_L(eGAMEPAD_ID nPadID, float fPower)
{
	//例外処理
	if (fPower < 0.0f || fPower > 1.0f)
		throw ERROR_EX2("振動値は0～1の間です。");

	m_Vibe_L[nPadID] = fPower;
}

void INPUT_PAD::SetVibration_R(eGAMEPAD_ID nPadID, float fPower)
{
	//例外処理
	if (fPower < 0.0f || fPower > 1.0f)
		throw ERROR_EX2("振動値は0～1の間です。");

	m_Vibe_R[nPadID] = fPower;
}
