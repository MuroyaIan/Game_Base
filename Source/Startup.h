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

#ifdef _DEBUG

#define IMGUI						 //ImGui使用
constexpr bool gc_IS_DEBUG = true;   //デバッグモード
constexpr bool gc_RUN_EDITOR = true; //エディタモード

#else

constexpr bool gc_IS_DEBUG = false;
constexpr bool gc_RUN_EDITOR = false;

#endif // _DEBUG
