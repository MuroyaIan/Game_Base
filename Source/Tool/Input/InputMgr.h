/**
 * @file InputMgr.h
 * @brief 入力処理マネージャ
 * @author 室谷イアン
 * @date 2022/07/28
 * @履歴 2022/07/28：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <Tool/Input/IM_Keyboard.h>		//キーボード入力
#include <Tool/Input/IM_Mouse.h>		//マウス入力
#include <Tool/Input/IM_GamePad.h>		//コントローラ入力

//===== 前方宣言 =====
class APP_64;

//===== クラス定義 =====

//***** 入力マネージャ *****
class INPUT_MGR
{
public:

	//* 変数宣言 *
	INPUT_KB m_KB;						//キーボード処理
	INPUT_CURSOR m_Mouse;				//マウス処理
	INPUT_PAD m_GamePad;				//コントローラ処理

	//* プロトタイプ宣言 *
	INPUT_MGR(APP_64& App) noexcept;
	~INPUT_MGR() noexcept;
	void Update() noexcept;				//更新処理
};
