
//===== �C���N���[�h�� =====
#include <App.h>

#ifdef _WIN64

#include <WinApp/WinWindow.h>

#endif // _WIN64

#include <Tool/Rand.h>
#include <EditorApp/Editor.h>
#include <Camera/CameraMgr.h>
#include <Light/LightMgr.h>

#ifdef IMGUI
#
#	include <Geometry/Debug/DebugMgr.h>
#
#endif // IMGUI



#include <Geometry/Base/Shape_Default.h>		//�y�`��e�X�g�z
#include <Geometry/Base/Shape_Tex.h>
#include <Geometry/Base/Shape_Model.h>
#include <Geometry/Model/Model.h>
#include <Light/DirectionalLight.h>
#include <Light/PointLight.h>
#include <Geometry/Model/TestPlayer.h>
//#include <Geometry/Model/TestWave.h>

namespace dx = DirectX;

//===== �O���[�o���萔�錾 =====
constexpr LPCWSTR WINDOW_NAME = L"Game_Base";	//Window��
constexpr float SCREEN_WIDTH = 1600.0f;
constexpr float SCREEN_HEIGHT = 900.0f;			//��ʉ𑜓x
constexpr int WND_POS_X = 150;
constexpr int WND_POS_Y = 50;					//Window������W

//===== �N���X���� =====
APP::APP() :
	m_pWindow(), m_Message(), m_Time(), m_pDX(), m_pShaderMgr(), m_pTextureMgr(), m_pModelMgr(),
	m_pGfx(), m_pInputMgr(), m_pEditor(), m_pCameraMgr(), m_pLightMgr(),
	m_aDrawer(0), m_aModel(0), m_pSunLight(), m_aLight(4), m_pPlayer()
{

#ifdef _WIN64

	//�E�B���h�E������
	m_pWindow = std::make_unique<WIN_WINDOW>(WINDOW_NAME, static_cast<int>(SCREEN_WIDTH), static_cast<int>(SCREEN_HEIGHT), WND_POS_X, WND_POS_Y);

	//DirectX������
	m_pDX = std::make_unique<GRAPHIC>(dynamic_cast<WIN_WINDOW*>(m_pWindow.get())->GetHandle(), SCREEN_WIDTH, SCREEN_HEIGHT);

#endif // _WIN64

	//�V�F�[�_Mgr������
	m_pShaderMgr = std::make_unique<SHADER_MGR>(*m_pDX);

	//�e�N�X�`��Mgr������
	m_pTextureMgr = std::make_unique<TEXTURE_MGR>(*m_pDX);

	//���f��Mgr������
	m_pModelMgr = std::make_unique<MODEL_MGR>();

	//�`��f�[�^������
	m_pGfx = std::make_unique<GFX_PACK>(*m_pDX, *m_pShaderMgr, *m_pTextureMgr, *m_pModelMgr);

	//���̓}�l�[�W��������
	m_pInputMgr = std::make_unique<INPUT_MGR>(*this);

	//����������
	RAND_MAKER::InitMaker();

	//�G�f�B�^������
	m_pEditor = std::make_unique<EDITOR>(*this);

	//�J�����}�l�[�W��������
	m_pCameraMgr = std::make_unique<CAMERA_MGR>(*this);

	//���C�g�}�l�[�W��������
	m_pLightMgr = std::make_unique<LIGHT_MGR>(*this);

#ifdef IMGUI

	//�f�o�b�O�}�l�[�W��������
	m_pDebugMgr = std::make_unique<DEBUG_MGR>(*this);

#endif // IMGUI



	//�y�`��e�X�g�z
	m_aDrawer.reserve(11);
	m_aDrawer.push_back(std::make_unique<SHAPE_DEFAULT>(*m_pGfx, VSD_MAKER::SHAPE::Box));
	m_aDrawer.push_back(std::make_unique<SHAPE_DEFAULT>(*m_pGfx, VSD_MAKER::SHAPE::Pyramid));
	m_aDrawer.push_back(std::make_unique<SHAPE_DEFAULT>(*m_pGfx, VSD_MAKER::SHAPE::Cone));
	m_aDrawer.push_back(std::make_unique<SHAPE_DEFAULT>(*m_pGfx, VSD_MAKER::SHAPE::Prism));
	m_aDrawer.push_back(std::make_unique<SHAPE_DEFAULT>(*m_pGfx, VSD_MAKER::SHAPE::Cylinder));
	m_aDrawer.push_back(std::make_unique<SHAPE_DEFAULT>(*m_pGfx, VSD_MAKER::SHAPE::Sphere));
	m_aDrawer.push_back(std::make_unique<SHAPE_DEFAULT>(*m_pGfx, VSD_MAKER::SHAPE::Plane));
	m_aDrawer.push_back(std::make_unique<SHAPE_TEX>(*m_pGfx, VSD_MAKER::SHAPE::Box, TEXTURE_MGR::TEX_ID::TEX_TestBox));
	m_aDrawer.push_back(std::make_unique<SHAPE_TEX>(*m_pGfx, VSD_MAKER::SHAPE::Plane, TEXTURE_MGR::TEX_ID::TEX_TestPlane));
	m_aDrawer.push_back(std::make_unique<SHAPE_MODEL>(*m_pGfx, VSD_MAKER::SHAPE::Box));
	m_aDrawer.push_back(std::make_unique<SHAPE_MODEL>(*m_pGfx, VSD_MAKER::SHAPE::Pyramid));
	m_aDrawer.push_back(std::make_unique<SHAPE_MODEL>(*m_pGfx, VSD_MAKER::SHAPE::Cone));
	m_aDrawer.push_back(std::make_unique<SHAPE_MODEL>(*m_pGfx, VSD_MAKER::SHAPE::Prism));
	m_aDrawer.push_back(std::make_unique<SHAPE_MODEL>(*m_pGfx, VSD_MAKER::SHAPE::Cylinder));
	m_aDrawer.push_back(std::make_unique<SHAPE_MODEL>(*m_pGfx, VSD_MAKER::SHAPE::Sphere));
	m_aDrawer.push_back(std::make_unique<SHAPE_MODEL>(*m_pGfx, VSD_MAKER::SHAPE::Plane));

	//�`�󐶐��p�����_��
	std::vector<std::unique_ptr<DRAWER>>& aDrawer = m_aDrawer;
	auto MakeGeom = [&aDrawer](int Idx)
	{
		enum class SHAPE
		{
			Box,
			Pyramid,
			Cone,
			Prism,
			Cylinder,
			Sphere,
			Plane,
			Box_Tex,
			Plane_Tex,
			Model_Box,
			Model_Pyramid,
			Model_Cone,
			Model_Prism,
			Model_Cylinder,
			Model_Sphere,
			Model_Plane,

			MaxNum
		};

		//�C���X�^���X�쐬
		SHAPE Shape = static_cast<SHAPE>(RAND_MAKER::MakeRand_Int(9, 15));
		int InstIdx = aDrawer[static_cast<int>(Shape)]->AddInstance();

		//���[���h�s��ݒ�
		dx::XMFLOAT4X4 mtxW{};
		dx::XMStoreFloat4x4(&mtxW, dx::XMMatrixTranslation(
			(Idx % 20) * 2.0f - (20.0f - 1.0f),
			0.0f,
			static_cast<float>(Idx / 20) * 2.0f - (20.0f - 1.0f))
		);
		aDrawer[static_cast<int>(Shape)]->SetWorldMatrix(mtxW, InstIdx);
		return;
	};

	//��������
	//for (int i = 0; i < nDrawNum; i++)
	//	MakeGeom(i);

	//�y���f���e�X�g�z
	m_aModel.reserve(1);
	m_aModel.push_back(std::make_unique<MODEL>(*this, MODEL_MGR::MODEL_ID::Tarantella));
	for (int i = 0; i < 1600; i++) {
		m_aModel[0]->AddInstance();

		//���[���h�s��ݒ�
		dx::XMFLOAT4X4 mtxW{};
		dx::XMStoreFloat4x4(&mtxW, dx::XMMatrixTranslation(
			(i % 10) * 10.0f - (50.0f - 5.0f),
			0.0f,
			static_cast<float>(i / 10) * 10.0f - (50.0f - 5.0f)
		)
		);
		m_aModel[0]->SetWorldMatrix(mtxW, i);
	}

	//���ʃe�X�g
	//m_aDrawer.push_back(std::make_unique<WAVE>(*this));



	//�J����������
	m_pCameraMgr->SetCamera(CAMERA_MGR::CAMERA_ID::FirstPerson);

	//���z��������
	m_pSunLight = std::make_unique<DIRECTIONAL_LIGHT>(*this);

	//�_����������
	for(auto& l : m_aLight)
		l = std::make_unique<POINT_LIGHT>(*this, 50.0f);
	m_aLight[0]->GetData().Color_D = dx::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	m_aLight[1]->GetData().Color_D = dx::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	m_aLight[2]->GetData().Color_D = dx::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	m_aLight[3]->GetData().Color_D = dx::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	m_aLight[0]->GetData().Pos = dx::XMFLOAT4(8.0f, 2.0f, 8.0f, 1.0f);
	m_aLight[1]->GetData().Pos = dx::XMFLOAT4(-8.0f, 2.0f, 8.0f, 1.0f);
	m_aLight[2]->GetData().Pos = dx::XMFLOAT4(8.0f, 2.0f, -8.0f, 1.0f);
	m_aLight[3]->GetData().Pos = dx::XMFLOAT4(-8.0f, 2.0f, -8.0f, 1.0f);



	//m_pPlayer = std::make_unique<PLAYER>(*this);
}

APP::~APP()
{
}

//�A�v���P�[�V�������s
int APP::Run()
{
	//�Q�[�����[�v
	while (true)
	{
		//���b�Z�[�W����
		if (PeekMessage(&m_Message, nullptr, 0, 0, PM_REMOVE)) {

			//�I������
			if (m_Message.message == WM_QUIT)
				return static_cast<int>(m_Message.wParam);

			//���̑�
			TranslateMessage(&m_Message);
			DispatchMessage(&m_Message);
		}

		//FPS����
		if (!m_Time.Update())
			continue;

		//�Q�[������
		Update();
		Draw();
	}
}

//�X�V����
void APP::Update()
{
	//���͏����X�V
	m_pInputMgr->Update();

	//�G�f�B�^�X�V
	if (m_pEditor->IsEnabled())		//���[�h�m�F
		m_pEditor->Update();
	else {

		//���z���X�V
		m_pSunLight->Update();

		//�_�����X�V
		for (auto& l : m_aLight)
			l->Update();

		//3D�X�V
		for (auto& d : m_aDrawer)
			d->Update();
		for (auto& m : m_aModel)
			m->Update();
		//m_pPlayer->Update();



		//�J�����}�l�[�W���X�V
		m_pCameraMgr->Update();

		//���C�g�}�l�[�W���X�V
		m_pLightMgr->Update();
	}

#ifdef IMGUI

	//�f�o�b�O�}�l�[�W���X�V
	m_pDebugMgr->Update();

	//ImGui�`�搧��
	if (m_pInputMgr->m_KB.GetTrigger(VK_TAB)) {
		if (m_pDX->IsImGuiEnabled())
			m_pDX->SetImGuiMode(false);
		else
			m_pDX->SetImGuiMode(true);
	}

	//�}�E�X�\���E��\��
	if (m_pInputMgr->m_KB.GetTrigger(VK_Q)) {
		if (m_pInputMgr->m_Mouse.IsDrawing()) {
			m_pInputMgr->m_Mouse.Hide();
			m_pInputMgr->m_Mouse.EnableRawInput();
		}
		else {
			m_pInputMgr->m_Mouse.Show();
			m_pInputMgr->m_Mouse.DisableRawInput();
		}
	}

#endif // IMGUI

}

//�`�揈��
void APP::Draw()
{
	//�`��J�n
	m_pDX->BeginFrame(0.0f, 0.0f, 0.0f);

	//static float fAngle{ 0.0f };	//�y2D�e�X�g�z��]�p�x
	//fAngle += 0.01f;
	//m_pDX->DrawTestTriangle(fAngle, (float)m_Window.m_Mouse.GetPosX() / 800.0f - 1.0f, -(float)m_Window.m_Mouse.GetPosY() / 450.0f + 1.0f);



	//���[�h�ؑ�
	if (m_pEditor->IsEnabled())
		m_pEditor->Draw();			//�G�f�B�^���[�h
	else {

		//3D�`��
		for (auto& d : m_aDrawer)
			d->Draw();
		for (auto& m : m_aModel)
			m->Draw();
		//m_pPlayer->Draw();



#ifdef IMGUI

		//�f�o�b�O�}�l�[�W���`��
		m_pDebugMgr->Draw();

		//ImGui�`��
		if (m_pDX->IsImGuiEnabled()) {

			//�e�X�g���j���[
			if (ImGui::Begin(U8(u8"�R���g���[���p�l��")))
			{
				//���\�v��
				static float FpsHist[10]{ 0.0f };														//�v������
				static unsigned short HistSlot = 0;														//�v���X���b�g
				static unsigned short TgtFPS = static_cast<unsigned short>(m_Time.GetTargetFPS() / 2);	//�v�������i�t���[���P��)
				HistSlot++;																				//�X���b�g�X�V
				if (HistSlot >= _countof(FpsHist) * TgtFPS)
					HistSlot = 0;
				float fFps = static_cast<float>(m_Time.GetFPS());
				if (HistSlot % TgtFPS == 0) {
					for (int i = 1; i < _countof(FpsHist); i++)				//�f�[�^�V�t�g
						FpsHist[i - 1] = FpsHist[i];
					FpsHist[_countof(FpsHist) - 1] = fFps;					//����ǉ�
				}
				ImGui::Text(U8(u8"FPS�v��"));
				ImGui::SameLine();
				ImGui::Text(": %.3f ms/frame (%.0f)", 1000.0f / fFps, fFps);
				ImGui::PlotLines(U8(u8"RT�}"), FpsHist, IM_ARRAYSIZE(FpsHist), 0, 0, 0.0f, 60.0f);

				//�|���S�����\��
				UINT PolyNum = 0;
				for (auto& d : m_aDrawer)
					PolyNum += d->GetPolygonNum();
				for (auto& m : m_aModel)
					PolyNum += m->GetPolygonNum();
				PolyNum += m_pDebugMgr->GetPolygonNum();
				ImGui::NewLine();
				ImGui::Text(U8(u8"�|���S�����i�O�p�`�j")); ImGui::SameLine(); ImGui::Text(": %d", PolyNum);

				//���͏��\��
				ImGui::NewLine();
				if (ImGui::TreeNode(U8(u8"���̓f�o�C�X"))) {

					//�}�E�X���
					ImGui::Text(U8(u8"�@�}�E�X"));

					dx::XMFLOAT2 MousePos = m_pInputMgr->m_Mouse.GetPos();
					ImGui::Text(U8(u8"���W")); ImGui::SameLine();
					ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), " X"); ImGui::SameLine(); ImGui::Text(": %d", static_cast<int>(MousePos.x)); ImGui::SameLine();
					ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), " Y"); ImGui::SameLine(); ImGui::Text(": %d", static_cast<int>(MousePos.y));

					ImGui::Text(U8(u8"�z�C�[���l")); ImGui::SameLine(); ImGui::Text(": %d", m_pInputMgr->m_Mouse.GetWheelVal());

					ImGui::Text(U8(u8"�}�E�X�\��")); ImGui::SameLine(); ImGui::Text(":"); ImGui::SameLine();
					if (m_pInputMgr->m_Mouse.IsDrawing())
						ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "True");
					else {
						ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "False");

						dx::XMINT2 MouseMove = m_pInputMgr->m_Mouse.GetMoveVal();
						ImGui::Text(U8(u8"�}�E�X�ړ���"));
						ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), " X"); ImGui::SameLine(); ImGui::Text(": %d", MouseMove.x); ImGui::SameLine();
						ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), " Y"); ImGui::SameLine(); ImGui::Text(": %d", MouseMove.y);
					}

					//�R���g���[�����
					ImGui::Text(U8(u8"�@�p�b�h"));
					ImGui::Text(U8(u8"�ڑ��ԍ�")); ImGui::SameLine(); ImGui::Text(":");
					if (m_pInputMgr->m_GamePad.CheckPad(PAD_1)) {
						ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", PAD_1 + 1);
					}
					else if (m_pInputMgr->m_GamePad.CheckPad(PAD_2)) {
						ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", PAD_2 + 1);
					}
					else if (m_pInputMgr->m_GamePad.CheckPad(PAD_3)) {
						ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", PAD_3 + 1);
					}
					else if (m_pInputMgr->m_GamePad.CheckPad(PAD_4)) {
						ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", PAD_4 + 1);
					}
					else {
						ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "None");
					}

					if (m_pInputMgr->m_GamePad.CheckPad(PAD_1)) {
						dx::XMFLOAT2 StickL3 = m_pInputMgr->m_GamePad.GetStickL3(PAD_1);
						dx::XMFLOAT2 StickR3 = m_pInputMgr->m_GamePad.GetStickR3(PAD_1);
						ImGui::Text(U8(u8"�X�e�B�b�N"));
						ImGui::Text("(L3)"); ImGui::SameLine();
						ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "X"); ImGui::SameLine(); ImGui::Text(": %.1f ", StickL3.x); ImGui::SameLine();
						ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Y"); ImGui::SameLine(); ImGui::Text(": %.1f", StickL3.y);
						ImGui::Text("(R3)"); ImGui::SameLine();
						ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "X"); ImGui::SameLine(); ImGui::Text(": %.1f ", StickR3.x); ImGui::SameLine();
						ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Y"); ImGui::SameLine(); ImGui::Text(": %.1f", StickR3.y);

						ImGui::Text(U8(u8"�g���K�["));
						ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "L2"); ImGui::SameLine(); ImGui::Text(": %.1f ", m_pInputMgr->m_GamePad.GetValL2(PAD_1)); ImGui::SameLine();
						ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "R2"); ImGui::SameLine(); ImGui::Text(": %.1f", m_pInputMgr->m_GamePad.GetValR2(PAD_1));

						ImGui::Text(U8(u8"�U��")); ImGui::SameLine(); ImGui::Text(":");
						if (m_pInputMgr->m_GamePad.GetVibeL(PAD_1) == 0.0f && m_pInputMgr->m_GamePad.GetVibeR(PAD_1) == 0.0f) {
							ImGui::SameLine(); ImGui::Text("Off");
						}
						else {
							ImGui::SameLine(); ImGui::Text("On");
						}
						ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "L"); ImGui::SameLine(); ImGui::Text(": %.1f ", m_pInputMgr->m_GamePad.GetVibeL(PAD_1)); ImGui::SameLine();
						ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "R"); ImGui::SameLine(); ImGui::Text(": %.1f", m_pInputMgr->m_GamePad.GetVibeR(PAD_1));
					}



					ImGui::TreePop();
				}

				//�J�������
				if (ImGui::TreeNode(U8(u8"�J�������"))) {

					//�o�͏���
					dx::XMFLOAT4X4 mtxW = m_pCameraMgr->GetWorldMtx();
					ImGui::Text(U8(u8"�@�ʒu")); ImGui::SameLine(); ImGui::Text("(cm)");
					ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "X"); ImGui::SameLine(); ImGui::Text(": %.1f ", mtxW._41); ImGui::SameLine();
					ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Y"); ImGui::SameLine(); ImGui::Text(": %.1f ", mtxW._42); ImGui::SameLine();
					ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f), "Z"); ImGui::SameLine(); ImGui::Text(": %.1f", mtxW._43);

					dx::XMFLOAT3 Rot = m_pCameraMgr->GetRotation();
					ImGui::Text(U8(u8"�@��]")); ImGui::SameLine(); ImGui::Text("(deg.)");
					ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "X"); ImGui::SameLine(); ImGui::Text(": %d ", gMath::GetDegree(Rot.x)); ImGui::SameLine();
					ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Y"); ImGui::SameLine(); ImGui::Text(": %d ", gMath::GetDegree(Rot.y)); ImGui::SameLine();
					ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f), "Z"); ImGui::SameLine(); ImGui::Text(": %d", gMath::GetDegree(Rot.z));

					ImGui::TreePop();
				}

				//���C�g���
				if (ImGui::TreeNode(U8(u8"���C�g���"))) {
					auto& LightDataD = m_pSunLight->GetData();
					auto& LightDataA = m_pLightMgr->GetData();

					//���s����
					ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), U8(u8"���s����"));

					ImGui::Text(U8(u8"���̌���"));
					ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "X"); ImGui::SameLine(); ImGui::Text(": %.1f ", LightDataD.Pos.x); ImGui::SameLine();
					ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Y"); ImGui::SameLine(); ImGui::Text(": %.1f ", LightDataD.Pos.y); ImGui::SameLine();
					ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f), "Z"); ImGui::SameLine(); ImGui::Text(": %.1f", LightDataD.Pos.z);

					ImGui::Text(U8(u8"�F���"));
					ImGui::ColorEdit3(U8(u8"�g�U�F"), &LightDataD.Color_D.x);
					ImGui::Text(U8(u8"�g�U�����x")); ImGui::SameLine(); ImGui::Text(": %.2f", LightDataD.Color_D.w);

					//����
					ImGui::NewLine();
					ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), U8(u8"����"));
					ImGui::ColorEdit3(U8(u8"���F"), &LightDataA.AmbientLight.x);
					ImGui::Text(U8(u8"�������x")); ImGui::SameLine(); ImGui::Text(": %.2f", LightDataA.AmbientLight.w);

					//�_����
					ImGui::NewLine();
					ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), U8(u8"�_����"));
					ImGui::Text(U8(u8"�L��������")); ImGui::SameLine(); ImGui::Text(": %d", m_pLightMgr->GetLightNum());

					ImGui::TreePop();
				}
			}
			ImGui::End();
		}

#endif // IMGUI

	}

	//�`��I��
	m_pDX->EndFrame();
}
