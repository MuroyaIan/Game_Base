
//===== インクルード部 =====
#include <Tool/imguiMgr.h>
#include <WinApp/WinConfig.h>

//===== クラス実装 =====
IMGUI_MGR::IMGUI_MGR() noexcept
{
	//初期化処理
	IMGUI_CHECKVERSION();
	if (ImGui::CreateContext() == nullptr) {

#ifdef _DEBUG

		PRINT_D(L"Error : IMGUI初期化失敗!\n");

#endif // _DEBUG

	}

	//スタイル設定
	ImGui::StyleColorsDark();
	ImGuiStyle& Style = ImGui::GetStyle();
	Style.Colors[ImGuiCol_TitleBg]          = ImVec4(0.0f, 0.5f, 0.5f, 0.5f);
	Style.Colors[ImGuiCol_TitleBgActive]    = ImVec4(0.0f, 0.5f, 0.5f, 1.0f);
	Style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.2f, 0.2f, 0.2f, 0.5f);

	//フォント読込
	ImGuiIO& io = ImGui::GetIO();
	ImFont* Font = io.Fonts->AddFontFromFileTTF("Asset/Font/UDEVGothicJPDOC-Regular.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	IM_ASSERT(Font != NULL);
	(void)Font;
}

IMGUI_MGR::~IMGUI_MGR() noexcept
{
	ImGui::DestroyContext();
}

//DEMOウィンドウ描画
void IMGUI_MGR::DrawDemo() const noexcept
{
	static bool bShowDemo = true;
	if (bShowDemo)
		ImGui::ShowDemoWindow(&bShowDemo);
}
