/**
 * @file IM_Keyboard.h
 * @brief キーボード処理
 * @author 室谷イアン
 * @date 2022/07/28
 * @履歴 2022/07/28：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== 定数・マクロ定義 =====
constexpr int VK_0 = static_cast<int>('0');
constexpr int VK_1 = static_cast<int>('1');
constexpr int VK_2 = static_cast<int>('2');
constexpr int VK_3 = static_cast<int>('3');
constexpr int VK_4 = static_cast<int>('4');
constexpr int VK_5 = static_cast<int>('5');
constexpr int VK_6 = static_cast<int>('6');
constexpr int VK_7 = static_cast<int>('7');
constexpr int VK_8 = static_cast<int>('8');
constexpr int VK_9 = static_cast<int>('9');
constexpr int VK_A = static_cast<int>('A');
constexpr int VK_B = static_cast<int>('B');
constexpr int VK_C = static_cast<int>('C');
constexpr int VK_D = static_cast<int>('D');
constexpr int VK_E = static_cast<int>('E');
constexpr int VK_F = static_cast<int>('F');
constexpr int VK_G = static_cast<int>('G');
constexpr int VK_H = static_cast<int>('H');
constexpr int VK_I = static_cast<int>('I');
constexpr int VK_J = static_cast<int>('J');
constexpr int VK_K = static_cast<int>('K');
constexpr int VK_L = static_cast<int>('L');
constexpr int VK_M = static_cast<int>('M');
constexpr int VK_N = static_cast<int>('N');
constexpr int VK_O = static_cast<int>('O');
constexpr int VK_P = static_cast<int>('P');
constexpr int VK_Q = static_cast<int>('Q');
constexpr int VK_R = static_cast<int>('R');
constexpr int VK_S = static_cast<int>('S');
constexpr int VK_T = static_cast<int>('T');
constexpr int VK_U = static_cast<int>('U');
constexpr int VK_V = static_cast<int>('V');
constexpr int VK_W = static_cast<int>('W');
constexpr int VK_X = static_cast<int>('X');
constexpr int VK_Y = static_cast<int>('Y');
constexpr int VK_Z = static_cast<int>('Z');		//仮想キーコード
constexpr int MAX_NUM_VK = 256;					//仮想キーコードの最大数

//===== 前方宣言 =====
class APP;

//===== クラス定義 =====

//***** キーボード入力 *****
class INPUT_KB
{
public:

	//* プロトタイプ宣言 *
	explicit INPUT_KB(APP& App) noexcept;
	~INPUT_KB() noexcept;
	bool GetPress(int nKey) const noexcept;				//プレス判定
	bool GetTrigger(int nKey) const noexcept;			//トリガー判定
	bool GetRelease(int nKey) const noexcept;			//リリース判定
	bool GetRepeat(int nKey, float fInterval = 0.5f);	//リピート判定（デフォルト0.5秒）

private:

	using uShort = unsigned short;

	//* 変数宣言 *
	APP& m_App;								//App参照
	uShort m_State;							//入力状態取得用
	uShort m_Press[MAX_NUM_VK];				//押下状態判定用
	uShort m_Trigger[MAX_NUM_VK];			//トリガ状態判定用
	uShort m_Release[MAX_NUM_VK];			//リリース状態判定用
	uShort m_Repeat[MAX_NUM_VK];			//リピート状態判定用
	uShort m_RepeatCnt[MAX_NUM_VK];			//リピートカウンタ
	uShort m_RepeatInterval[MAX_NUM_VK];	//リピート間隔
	uShort* m_pPress;
	uShort* m_pTrigger;
	uShort* m_pRelease;
	uShort* m_pRepeat;
	uShort* m_pRepeatCnt;
	uShort* m_pRepeatInterval;				//ループ処理用ポインタ

	//* プロトタイプ宣言 *
	void Update() noexcept;					//更新処理

	//権限指定
	friend class INPUT_MGR;
};
