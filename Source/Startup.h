/**
 * @file Startup.h
 * @brief スタートアップ処理
 * @author 室谷イアン
 * @date 2022/04/30
 * @履歴 2022/04/30：ウィンドウ作成、WndProc作成
 *		 2022/06/09：ウィンドウ処理ライブラリ化
 */

//===== インクルードガード =====
#pragma once

//===== 定数・マクロ定義 =====
#define APP_WIN64					//プラットフォーム指定
#define DIRECTX_11					//dx11
//#define DIRECTX_12				//dx12

//デバッグ判定
#ifdef _DEBUG

constexpr bool IS_DEBUG = true;		//デバッグモード
#define IMGUI						//ImGui使用
constexpr bool RUN_EDITOR = true;	//エディタモード

#else

constexpr bool IS_DEBUG = false;
constexpr bool RUN_EDITOR = false;

#endif // _DEBUG
