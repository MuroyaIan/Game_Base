/**
 * @file TextIO.h
 * @brief テキスト入出力ヘッダファイル群
 * @author 室谷イアン
 * @date 2022/06/09
 * @履歴 2022/06/09：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <iostream>					//標準入出力
#include <string>					//文字列クラス
#include <sstream>					//文字列ストリームクラス
#include <Tool/gMemory.h>

//===== プロトタイプ宣言 =====
namespace gText
{
	//文字列分割
	std::vector<std::string> Split(const std::string& Text, char Sep);
}
