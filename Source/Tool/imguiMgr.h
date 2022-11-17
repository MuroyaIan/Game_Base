/**
 * @file imguiMgr.h
 * @brief ImGuiマネージャ
 * @author 室谷イアン
 * @date 2022/07/18
 * @履歴 2022/07/18：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <3rd_Party/ImGui/imgui.h>

//===== 定数・マクロ定義 =====
#define U8(str) reinterpret_cast<const char*>(str)

//===== クラス宣言 =====

//***** ImGuiマネージャ *****
class IMGUI_MGR
{
public:

	//プロトタイプ宣言
	IMGUI_MGR() noexcept;
	~IMGUI_MGR() noexcept;
	void DrawDemo() const noexcept;		//DEMOウィンドウ描画
};
