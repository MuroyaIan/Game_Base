/**
 * @file WinConfig.h
 * @brief OS環境設定用ヘッダ(Windows)
 * @author 室谷イアン
 * @date 2022/05/04
 * @履歴 2022/05/04：ヘッダ作成
 *		 2022/11/14：デバッグ設定追加
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#define WINVER 0x0A00			//バージョン指定(Win10)
#include <sdkddkver.h>

//使用しない機能の制限
//#define WIN32_LEAN_AND_MEAN	//FBX読込対策
#define NOGDICAPMASKS
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
//#define NOCTLMGR				//IMGUI対策
#define NODRAWTEXT
#define NOKERNEL
//#define NONLS					//テクスチャ読込対策
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE
#include <Windows.h>

//デバッグ機能
#ifdef _DEBUG

#include <debugapi.h>	//デバッグ表示
#include <crtdbg.h>		//メモリリーク検出

#define PRINT_D(wStr) OutputDebugString(wStr)	//デバッグ出力用

#endif // _DEBUG
