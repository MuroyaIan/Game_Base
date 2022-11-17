/**
 * @file gMemory.h
 * @brief メモリ管理ヘッダファイル群
 * @author 室谷イアン
 * @date 2022/06/10
 * @履歴 2022/06/10：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <memory>					//スマートポインタ
#include <array>					//固定配列
#include <vector>					//動的配列
#include <list>						//リスト
#include <map>						//連想配列
#include <algorithm>

//===== 定数・マクロ定義 =====
#ifdef _DEBUG
#
#	define NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)	//メモリリーク検出
#
#else
#
#	define NEW new
#
#endif // _DEBUG

#ifndef SAFE_RELEASE
#
#	define SAFE_RELEASE(x)	do { if(x) { (x)->Release(); (x)=nullptr; } } while(0)
#
#endif // !SAFE_RELEASE

#ifndef SAFE_DELETE
#
#	define SAFE_DELETE(x)	do { if(x) { delete(x); (x)=nullptr; } } while(0)
#
#endif // !SAFE_DELETE

#ifndef SAFE_DELETE_ARRAY
#
#	define SAFE_DELETE_ARRAY(x)	do { if(x) { delete[](x); (x)=nullptr; } } while(0)
#
#endif // !SAFE_DELETE_ARRAY
