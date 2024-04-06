
//===== �C���N���[�h�� =====
#include <Tool/imguiMgr.h>
#include <WinApp/WinConfig.h>

//===== �N���X���� =====
IMGUI_MGR::IMGUI_MGR() noexcept
{
	//����������
	IMGUI_CHECKVERSION();
	if (ImGui::CreateContext() == nullptr) {

#ifdef _DEBUG

		PRINT_D(L"Error : IMGUI���������s!\n");

#endif // _DEBUG

	}

	//�X�^�C���ݒ�
	ImGui::StyleColorsDark();
	ImGuiStyle& Style = ImGui::GetStyle();
	Style.Colors[ImGuiCol_TitleBg]          = ImVec4(0.0f, 0.5f, 0.5f, 0.5f);
	Style.Colors[ImGuiCol_TitleBgActive]    = ImVec4(0.0f, 0.5f, 0.5f, 1.0f);
	Style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.2f, 0.2f, 0.2f, 0.5f);

	//�t�H���g�Ǎ�
	ImGuiIO& io = ImGui::GetIO();
	ImFont* Font = io.Fonts->AddFontFromFileTTF("Asset/Font/UDEVGothicJPDOC-Regular.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	IM_ASSERT(Font != NULL);
	(void)Font;
}

IMGUI_MGR::~IMGUI_MGR() noexcept
{
	ImGui::DestroyContext();
}

//DEMO�E�B���h�E�`��
void IMGUI_MGR::DrawDemo() const noexcept
{
	static bool bShowDemo = true;
	if (bShowDemo)
		ImGui::ShowDemoWindow(&bShowDemo);
}
