/**
 * @file BaseWindow.h
 * @brief ウィンドウ用インターフェース
 * @author 室谷イアン
 * @date 2022/11/15
 * @履歴 2022/11/15：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <string>				//文字列クラス
#include <WinApp/Keyboard.h>	//キーボード処理
#include <WinApp/Mouse.h>		//マウス処理

//===== クラス定義 =====
class IF_WINDOW
{
public:

	//変数宣言
	KEYBOARD m_Keyboard;	//キーボード処理
	MOUSE m_Mouse;			//マウス処理

	//プロトタイプ宣言
	explicit IF_WINDOW() noexcept : m_Keyboard(), m_Mouse() {}
	virtual ~IF_WINDOW() noexcept(false) {}
	virtual void Transform(int nWndPosX, int nWndPosY, int nWndWidth = 0, int nWndHeight = 0) = 0;	//トランスフォーム
	virtual void TitlePrint(const std::string& Text) const = 0;										//タイトル出力
	virtual void TitlePrint_MousePos() const = 0;													//タイトル出力（マウス座標）
	virtual void TitlePrint_WheelVal() = 0;															//タイトル出力（ホイール値）
	virtual void EnableCursor() noexcept = 0;														//マウス使用・不使用
	virtual void DisableCursor() noexcept = 0;
	virtual bool IsUsingCursor() const noexcept = 0;												//マウス使用状態確認

	virtual std::pair<int, int> GetPos() const noexcept = 0;	//ウィンドウ左上座標取得
	virtual std::pair<int, int> GetSize() const noexcept = 0;	//ウィンドウサイズ取得
	virtual const bool IsUsingImgui() const noexcept = 0;		//Imgui使用確認
};
