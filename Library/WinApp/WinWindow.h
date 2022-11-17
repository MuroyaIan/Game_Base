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
#include <WinApp/BaseWindow.h>	//インターフェース
#include <WinApp/WinEO.h>		//エラー出力
#include <vector>				//動的配列

//===== クラス定義 =====
class WIN_WINDOW : public IF_WINDOW
{
public:

	//プロトタイプ宣言
	WIN_WINDOW(LPCWSTR WindowName, int nWndWidth, int nWndHeight, int nWndPosX = CW_USEDEFAULT, int nWndPosY = CW_USEDEFAULT);
	~WIN_WINDOW() noexcept(false) override;
	void Transform(int nWndPosX, int nWndPosY, int nWndWidth = 0, int nWndHeight = 0) override;		//トランスフォーム
	void TitlePrint(const std::string& Text) const override;										//タイトル出力
	void TitlePrint_MousePos() const override;														//タイトル出力（マウス座標）
	void TitlePrint_WheelVal() override;															//タイトル出力（ホイール値）
	void EnableCursor() noexcept override;
	void DisableCursor() noexcept override;															//マウス使用・不使用
	bool IsUsingCursor() const noexcept override;													//マウス使用状態確認

	std::pair<int, int> GetPos() const noexcept override	//ウィンドウ左上座標取得
	{
		return { m_PosX, m_PosY };
	}
	std::pair<int, int> GetSize() const noexcept override	//ウィンドウサイズ取得
	{
		return { m_Width, m_Height };
	}
	const HWND GetHandle() const noexcept					//ハンドル取得
	{
		return m_hWindow;
	}
	const bool IsUsingImgui() const noexcept override		//Imgui使用確認
	{
		return m_useImgui;
	}

private:

	//変数宣言
	static constexpr const LPCWSTR CLASS_NAME = L"WindowClass_Game";	//クラス名
	HINSTANCE m_hAppInst;												//Instanceハンドル
	int m_PosX;
	int m_PosY;						//位置
	int m_Width;
	int m_Height;					//幅・高さ（クライアント領域）
	HWND m_hWindow;					//ハンドル
	bool m_bDrawCursor;				//マウス描画フラグ
	std::vector<BYTE> m_RawBuffer;	//RawInput用バッファ
	bool m_useImgui;				//Imgui使用中フラグ

	//プロトタイプ宣言
	static LRESULT CALLBACK WndProc_Init(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;	//WndProc初期化
	static LRESULT CALLBACK WndProc_Call(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;	//WndProc呼び出し
	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;						//WndProc本処理
	void HideCursor() noexcept;
	void ShowCursor() noexcept;																			//マウス非表示・表示
	void LockCursor() noexcept;
	void UnlockCursor() noexcept;																		//マウスをウィンドウ内にロック・ロック解除
};
