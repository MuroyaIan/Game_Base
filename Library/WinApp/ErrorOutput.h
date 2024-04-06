/**
 * @file ErrorOutput.h
 * @brief エラー出力クラス
 * @author 室谷イアン
 * @date 2022/05/02
 * @履歴 2022/05/02：クラス作成
 *		 2022/11/14：コンストラクタ改善
 *		 2024/04/02：書式改善
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <exception>			//例外処理
#include <string>				//文字列クラス
#include <sstream>				//文字列ストリームクラス

//===== クラス定義 =====

//***** エラー出力 *****
class CT_ERROR_OUTPUT : public std::exception
{
public:

	//コピー＆ムーブ
	CT_ERROR_OUTPUT(const CT_ERROR_OUTPUT&) = default;
	CT_ERROR_OUTPUT& operator =(const CT_ERROR_OUTPUT&) = default;
	CT_ERROR_OUTPUT(CT_ERROR_OUTPUT&&) noexcept = default;
	CT_ERROR_OUTPUT& operator=(CT_ERROR_OUTPUT&&) noexcept = default;

	//プロトタイプ宣言
	explicit CT_ERROR_OUTPUT(const int& nLine, const char* chFile) noexcept;
	~CT_ERROR_OUTPUT() noexcept override;
	std::string GetErrorInfo() const noexcept;  //エラー情報取得
	const char* what() const noexcept override; //エラー情報出力

	//仮想関数
	virtual std::string GetType() const noexcept = 0; //エラータイプ取得

protected:

	//変数宣言
	mutable std::string m_InfoBuffer; //情報格納用バッファ

private:

	//変数宣言
	int m_Line;         //発生位置（何行目）
	std::string m_File; //発生ファイル名
};
