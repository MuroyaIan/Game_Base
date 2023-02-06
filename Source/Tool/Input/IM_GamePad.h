/**
 * @file IM_GamePad.h
 * @brief コントローラ処理
 * @author 室谷イアン
 * @date 2022/07/28
 * @履歴 2022/07/28：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <WinApp/WinEO.h>			//エラー出力
#include <Xinput.h>					//XBOXコントローラー処理
#include <DirectXMath.h>

//===== 定数・マクロ定義 =====
constexpr int DS4_UP		= 0;
constexpr int DS4_DOWN		= 1;
constexpr int DS4_LEFT		= 2;
constexpr int DS4_RIGHT		= 3;
constexpr int DS4_OPTIONS	= 4;
constexpr int DS4_SHARE		= 5;
constexpr int DS4_L3		= 6;
constexpr int DS4_R3		= 7;
constexpr int DS4_L1		= 8;
constexpr int DS4_R1		= 9;
constexpr int DS4_L2		= 10;
constexpr int DS4_R2		= 11;
constexpr int DS4_CROSS		= 12;
constexpr int DS4_CIRCLE	= 13;
constexpr int DS4_SQUARE	= 14;
constexpr int DS4_TRIANGLE	= 15;
constexpr int PAD_BTN_NUM	= 16;	//最大ボタン数

enum eGAMEPAD_ID					//コントローラID（最大接続数4）
{
	PAD_1,
	PAD_2,
	PAD_3,
	PAD_4,

	MAX_PAD_NUM
};

//===== 前方宣言 =====
class APP;

//===== クラス定義 =====

//***** コントローラ入力 *****
class INPUT_PAD
{
public:

	//* プロトタイプ宣言 *
	explicit INPUT_PAD(APP& App) noexcept;
	~INPUT_PAD() noexcept;

	bool CheckPad(eGAMEPAD_ID nPadID) const noexcept;						//コントローラー接続確認
	bool GetPress(eGAMEPAD_ID nPadID, int nBtn) const noexcept;				//プレス判定
	bool GetTrigger(eGAMEPAD_ID nPadID, int nBtn) const noexcept;			//トリガー判定
	bool GetRelease(eGAMEPAD_ID nPadID, int nBtn) const noexcept;			//リリース判定
	bool GetRepeat(eGAMEPAD_ID nPadID, int nBtn, float fInterval = 0.5f);	//リピート判定（デフォルト0.5秒）

	void UseVibration(eGAMEPAD_ID nPadID, bool bStart) noexcept;			//振動オン・オフ
	void SetVibration(eGAMEPAD_ID nPadID, float fPower);
	void SetVibration_L(eGAMEPAD_ID nPadID, float fPower);
	void SetVibration_R(eGAMEPAD_ID nPadID, float fPower);					//振動強度設定

	DirectX::XMFLOAT2 GetStickL3(eGAMEPAD_ID nPadID) const noexcept
	{
		return m_Val_L3[nPadID];
	}
	DirectX::XMFLOAT2 GetStickR3(eGAMEPAD_ID nPadID) const noexcept			//左右スティックの傾き量取得
	{
		return m_Val_R3[nPadID];
	}
	float GetValL2(eGAMEPAD_ID nPadID) const noexcept
	{
		return m_Val_L2[nPadID];
	}
	float GetValR2(eGAMEPAD_ID nPadID) const noexcept						//左右トリガーの押し量取得
	{
		return m_Val_R2[nPadID];
	}
	float GetVibeL(eGAMEPAD_ID nPadID) const noexcept
	{
		return m_Vibe_L[nPadID];
	}
	float GetVibeR(eGAMEPAD_ID nPadID) const noexcept						//左右振動値取得
	{
		return m_Vibe_R[nPadID];
	}

private:

	using uShort = unsigned short;

	//* 変数宣言 *
	APP& m_App;											//App参照
	XINPUT_STATE m_State[MAX_PAD_NUM];					//PAD情報
	DWORD m_PadUsed[MAX_PAD_NUM];						//PAD接続状態
	XINPUT_VIBRATION m_Vibration[MAX_PAD_NUM];			//振動情報
	bool m_bUseVibration[MAX_PAD_NUM];					//振動使用許可

	uShort m_Press[MAX_PAD_NUM][PAD_BTN_NUM];			//押下状態判定用
	uShort m_PrevPress[MAX_PAD_NUM][PAD_BTN_NUM];		//押下状態判定用(前回実行時)
	uShort m_Trigger[MAX_PAD_NUM][PAD_BTN_NUM];			//トリガ状態判定用
	uShort m_Release[MAX_PAD_NUM][PAD_BTN_NUM];			//リリース状態判定用
	uShort m_Repeat[MAX_PAD_NUM][PAD_BTN_NUM];			//リピート状態判定用
	uShort m_RepeatCnt[MAX_PAD_NUM][PAD_BTN_NUM];		//リピートカウンタ
	uShort m_RepeatInterval[MAX_PAD_NUM][PAD_BTN_NUM];	//リピート間隔
	uShort* m_pPress;
	uShort* m_pPrevPress;
	uShort* m_pTrigger;
	uShort* m_pRelease;
	uShort* m_pRepeat;
	uShort* m_pRepeatCnt;
	uShort* m_pRepeatInterval;							//ループ処理用ポインタ

	float m_Val_L2[MAX_PAD_NUM];
	float m_Val_R2[MAX_PAD_NUM];						//左右トリガーの入力量（0～255）
	DirectX::XMFLOAT2 m_Val_L3[MAX_PAD_NUM];
	DirectX::XMFLOAT2 m_Val_R3[MAX_PAD_NUM];			//左右スティックの傾き量（-32768～32767）⇒　左手座標系
	float m_Vibe_L[MAX_PAD_NUM];
	float m_Vibe_R[MAX_PAD_NUM];						//左右の振動値（0～65535）

	//* プロトタイプ宣言 *
	void UpdateInputData(int nPadID) noexcept;			//入力情報更新
	void Update() noexcept;								//更新処理

	//権限指定
	friend class INPUT_MGR;
};
