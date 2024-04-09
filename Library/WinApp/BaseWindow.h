/**
 * @file BaseWindow.h
 * @brief ウィンドウ用インターフェース
 * @author 室谷イアン
 * @date 2022/11/15
 * @履歴 2022/11/15：ファイル作成
 *		 2024/04/07：書式改善
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <string>				//文字列クラス
#include <WinApp/Keyboard.h>	//キーボード処理
#include <WinApp/Mouse.h>		//マウス処理

//===== クラス定義 =====
class CT_IF_WINDOW
{
public:

	//コピー＆ムーブ
	CT_IF_WINDOW(const CT_IF_WINDOW&) = default;
	CT_IF_WINDOW& operator =(const CT_IF_WINDOW&) = default;
	CT_IF_WINDOW(CT_IF_WINDOW&&) noexcept = default;
	CT_IF_WINDOW& operator=(CT_IF_WINDOW&&) noexcept = default;

	//変数宣言
	CT_KEYBOARD m_Keyboard; //キーボード処理
	CT_MOUSE m_Mouse;       //マウス処理

	//コンストラクタ
	explicit CT_IF_WINDOW() noexcept = default;
	virtual ~CT_IF_WINDOW() noexcept(false) = default;

	//プロトタイプ宣言
	virtual void Transform(const int& nWndPosX, const int& nWndPosY, const int& nWndWidth = 0, const int& nWndHeight = 0) = 0; //トランスフォーム
	virtual void TitlePrint(const std::string& text) const = 0;                                                                //タイトル出力
	virtual void TitlePrint_MousePos() const = 0;                                                                              //タイトル出力（マウス座標）
	virtual void TitlePrint_WheelVal() = 0;                                                                                    //タイトル出力（ホイール値）
	virtual void EnableCursor() noexcept = 0;                                                                                  //マウス使用・不使用
	virtual void DisableCursor() noexcept = 0;
	[[nodiscard]] virtual bool IsUsingCursor() const noexcept = 0;			//マウス使用状態確認

	[[nodiscard]] virtual std::pair<int, int> GetPos() const noexcept = 0;  //ウィンドウ左上座標取得
	[[nodiscard]] virtual std::pair<int, int> GetSize() const noexcept = 0; //ウィンドウサイズ取得
	[[nodiscard]] virtual bool IsUsingImgui() const noexcept = 0;           //Imgui使用確認
};
