/**
 * @file WinWindow.h
 * @brief ゲームウィンドウ処理
 * @author 室谷イアン
 * @date 2022/05/01
 * @履歴 2022/05/01：ウィンドウクラス作成
 *		 2022/05/02：エラー出力クラス作成、キーボード受付処理実装
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <WinApp/BaseWindow.h>			//インターフェース
#include <WinApp/ErrorOutput_Win.h>		//エラー出力
#include <vector>						//動的配列

//===== クラス定義 =====
class CT_IW_WIN final : public CT_IF_WINDOW
{
public:

	//コピー＆ムーブ
	CT_IW_WIN(const CT_IW_WIN&) = default;
	CT_IW_WIN& operator =(const CT_IW_WIN&) = default;
	CT_IW_WIN(CT_IW_WIN&&) noexcept = default;
	CT_IW_WIN& operator=(CT_IW_WIN&&) noexcept = default;

	//プロトタイプ宣言
	explicit CT_IW_WIN(const LPCWSTR& windowName, const int& nWndWidth, const int& nWndHeight, const int& nWndPosX = CW_USEDEFAULT, const int& nWndPosY = CW_USEDEFAULT);
	~CT_IW_WIN() noexcept(false) override;
	void Transform(const int& nWndPosX, const int& nWndPosY, const int& nWndWidth = 0, const int& nWndHeight = 0) override; //トランスフォーム
	void TitlePrint(const std::string& text) const override;                                                                //タイトル出力
	void TitlePrint_MousePos() const override;                                                                              //タイトル出力（マウス座標）
	void TitlePrint_WheelVal() override;                                                                                    //タイトル出力（ホイール値）
	void EnableCursor() noexcept override;                                                                                  //マウス使用
	void DisableCursor() noexcept override;                                                                                 //マウス不使用
	[[nodiscard]] bool IsUsingCursor() const noexcept override;                                                             //マウス使用状態確認

	[[nodiscard]] std::pair<int, int> GetPos() const noexcept override //ウィンドウ左上座標取得
	{
		return {m_PosX, m_PosY};
	}

	[[nodiscard]] std::pair<int, int> GetSize() const noexcept override //ウィンドウサイズ取得
	{
		return {m_Width, m_Height};
	}

	[[nodiscard]] bool IsUsingImgui() const noexcept override //Imgui使用確認
	{
		return m_bUseImgui;
	}

	[[nodiscard]] HWND GetHandle() const noexcept //ハンドル取得
	{
		return m_WinHandle;
	}

private:

	//変数宣言
	static constexpr LPCWSTR c_Class_Name = L"WindowClass_Game"; //クラス名
	HINSTANCE m_AppInst;                                         //Instanceハンドル
	int m_PosX;                                                  //ウィンドウ左上X座標
	int m_PosY;                                                  //ウィンドウ左上Y座標
	int m_Width;                                                 //幅（クライアント領域）
	int m_Height;                                                //高さ（クライアント領域）
	HWND m_WinHandle;                                            //ハンドル
	bool m_bDrawCursor;                                          //マウス描画フラグ
	std::vector<BYTE> m_RawBuffer;                               //RawInput用バッファ
	bool m_bUseImgui;                                            //Imgui使用中フラグ

	//プロトタイプ宣言
	static LRESULT CALLBACK WndProc_Init(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;        //WndProc初期化
	static LRESULT CALLBACK WndProc_Call(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;        //WndProc呼び出し
	LRESULT WndProc(const HWND& hWnd, const UINT& uMsg, const WPARAM& wParam, const LPARAM& lParam) noexcept; //WndProc本処理
	static void HideCursor() noexcept;                                                                        //マウス非表示
	static void ShowCursor() noexcept;                                                                        //マウス表示
	void LockCursor() const noexcept;                                                                         //マウスをウィンドウ内にロック
	void UnlockCursor() const noexcept;                                                                       //マウスをウィンドウ内にロック解除
};
