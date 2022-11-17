/**
 * @file IM_Mouse.h
 * @brief マウス処理
 * @author 室谷イアン
 * @date 2022/07/28
 * @履歴 2022/07/28：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <WinApp/WinWindow.h>
#include <GraphicApp/Graphic.h>

//===== 定数・マクロ定義 =====
constexpr int MOUSE_L = static_cast<int>(VK_LBUTTON);		//マウスの左ボタン
constexpr int MOUSE_R = static_cast<int>(VK_RBUTTON);		//マウスの右ボタン
constexpr int MOUSE_M = static_cast<int>(VK_MBUTTON);		//マウスの中央ボタン
constexpr int MOUSE_X1 = static_cast<int>(VK_XBUTTON1);		//マウスの第1拡張ボタン
constexpr int MOUSE_X2 = static_cast<int>(VK_XBUTTON2);		//マウスの第2拡張ボタン

//===== 前方宣言 =====
class APP;
class MOUSE;
class INPUT_KB;

//===== クラス定義 =====

//***** マウス入力 *****
class INPUT_CURSOR
{
public:

	//* プロトタイプ宣言 *
	INPUT_CURSOR(APP& App, INPUT_KB& KB) noexcept;
	~INPUT_CURSOR() noexcept;
	bool GetPress(int nBtn) const noexcept;				//プレス判定
	bool GetTrigger(int nBtn) const noexcept;			//トリガー判定
	bool GetRelease(int nBtn) const noexcept;			//リリース判定
	bool GetRepeat(int nBtn, float fInterval = 0.5f);	//リピート判定（デフォルト0.5秒）

	DirectX::XMFLOAT2 GetPos() const noexcept;			//座標取得
	int GetWheelVal() const noexcept;					//ホイール値取得
	DirectX::XMINT2 GetMoveVal() const noexcept;		//移動量取得

private:

	//* 変数宣言 *
	MOUSE& m_Mouse;				//マウス参照
	INPUT_KB& m_KB;				//キーボード参照
	DirectX::XMINT2 MoveVal;	//移動量（毎フレーム）

	//* プロトタイプ宣言 *
	void Update() noexcept;		//更新処理

	//権限指定
	friend class INPUT_MGR;
};
