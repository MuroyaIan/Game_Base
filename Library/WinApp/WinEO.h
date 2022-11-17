/**
 * @file WinEO.h
 * @brief エラー出力クラス(Window)
 * @author 室谷イアン
 * @date 2022/11/15
 * @履歴 2022/11/15：クラス作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <WinApp/ErrorOutput.h>
#include <WinApp/WinConfig.h>

//===== クラス定義 =====
class WIN_EO : public ERROR_OUTPUT
{
public:

	//プロトタイプ宣言
	explicit WIN_EO(int nLine, const char* chFile, HRESULT hr, const char* str = nullptr) noexcept;
	~WIN_EO() noexcept override;
	std::string GetType() const noexcept override;				//エラータイプ取得
	std::string GetErrorCodeInfo(HRESULT hr) const noexcept;	//エラーコード情報取得
	const char* what() const noexcept override;					//エラー情報出力

private:

	//変数宣言
	HRESULT m_hr;				//メッセージ識別子
	std::string m_strError;		//カスタムエラー情報
};

//===== マクロ定義 =====
#define ERROR_DEFAULT() WIN_EO{__LINE__, __FILE__, HRESULT_FROM_WIN32(GetLastError())}
#define ERROR_EX(hr) WIN_EO{__LINE__, __FILE__, (hr)}
#define ERROR_EX2(str) WIN_EO{__LINE__, __FILE__, S_OK, (str)}
