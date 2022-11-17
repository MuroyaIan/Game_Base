/**
 * @file ErrorOutput.h
 * @brief エラー出力クラス
 * @author 室谷イアン
 * @date 2022/05/02
 * @履歴 2022/05/02：クラス作成
 *		 2022/11/14：コンストラクタ改善
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <exception>			//例外処理
#include <string>				//文字列クラス
#include <sstream>				//文字列ストリームクラス

//===== クラス定義 =====

//***** エラー出力 *****
class ERROR_OUTPUT : public std::exception
{
public:

	//プロトタイプ宣言
	explicit ERROR_OUTPUT(int nLine, const char* chFile) noexcept;
	~ERROR_OUTPUT() noexcept override;
	virtual std::string GetType() const noexcept = 0;	//エラータイプ取得
	std::string GetErrorInfo() const noexcept;			//エラー情報取得
	const char* what() const noexcept override;			//エラー情報出力

protected:

	//変数宣言
	mutable std::string m_InfoBuffer;	//情報格納用バッファ

private:

	//変数宣言
	int m_Line;							//発生位置（何行目）
	std::string m_File;					//発生ファイル名
};
