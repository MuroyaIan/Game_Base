/**
 * @file ErrorOutput_Win.h
 * @brief エラー出力クラス(Window)
 * @author 室谷イアン
 * @date 2022/11/15
 * @履歴 2022/11/15：クラス作成
 *		 2024/04/02：書式改善
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <WinApp/ErrorOutput.h>
#include <WinApp/WinConfig.h>

//===== クラス定義 =====

//***** エラー出力（Windows用） *****
class CT_EO_WIN final : public CT_ERROR_OUTPUT
{
public:

	//コピー＆ムーブ
	CT_EO_WIN(const CT_EO_WIN&) = default;
	CT_EO_WIN& operator =(const CT_EO_WIN&) = default;
	CT_EO_WIN(CT_EO_WIN&&) noexcept = default;
	CT_EO_WIN& operator=(CT_EO_WIN&&) noexcept = default;

	//プロトタイプ宣言
	explicit CT_EO_WIN(const int& nLine, const char* chFile, const HRESULT& hr, const char* str = nullptr) noexcept;
	~CT_EO_WIN() noexcept override;
	std::string GetType() const noexcept override;                  //エラータイプ取得
	std::string GetErrorCodeInfo(const HRESULT& hr) const noexcept; //エラーコード情報取得
	const char* what() const noexcept override;                     //エラー情報出力

private:

	//変数宣言
	HRESULT m_Hr;           //メッセージ識別子
	std::string m_StrError; //カスタムエラー情報
};

//===== マクロ定義 =====
#define ERROR_DEFAULT() CT_EO_WIN{__LINE__, __FILE__, HRESULT_FROM_WIN32(GetLastError())}
#define ERROR_EX(hr) CT_EO_WIN{__LINE__, __FILE__, (hr)}
#define ERROR_EX2(str) CT_EO_WIN{__LINE__, __FILE__, S_OK, (str)}
