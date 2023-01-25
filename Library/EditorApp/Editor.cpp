
//===== �C���N���[�h�� =====
#include <EditorApp/Editor.h>
#include <EditorApp/ModelViewer/Viewer.h>
#include <EditorApp/ModelViewer/Bone.h>
#include <Tool/gMath.h>

namespace dx = DirectX;

//===== �N���X���� =====
EDITOR::EDITOR(APP& App) noexcept :
	m_bEnable(RUN_EDITOR), m_App(App), m_bFBX_Viewer(false), m_pViewer()
{
}

EDITOR::~EDITOR() noexcept
{
}

//�X�V����
void EDITOR::Update()
{
	//�r���[������
	if (m_bFBX_Viewer) {

		//�r���[��������
		if (m_pViewer == nullptr)
			m_pViewer = std::make_unique<VIEWER>(m_App);

		//�r���[���X�V
		m_pViewer->Update();
	}
	else {

		//�r���[���I��
		if (m_pViewer != nullptr)
			m_pViewer.reset();
	}
}

//�`�揈��
void EDITOR::Draw()
{
	//�r���[���`��
	if (m_bFBX_Viewer)
		m_pViewer->Draw();

#ifdef IMGUI

	//�J�n���j���[
	if (m_App.m_pDX->IsImGuiEnabled()) {

		//�e�X�g���j���[
		if (ImGui::Begin("Game Editor Ver0.5"))
		{
			//�G�f�B�^�I��
			if (ImGui::Button(U8(u8"�Q�[�����[�h�J�n")))
				m_bEnable = false;

			//���\�v��
			static float FpsHist[10]{ 0.0f };																//�v������
			static unsigned short HistSlot = 0;																//�v���X���b�g
			static unsigned short TgtFPS = static_cast<unsigned short>(m_App.m_Time.GetTargetFPS() / 2);	//�v�������i�t���[���P�ʁj
			HistSlot++;
			if (HistSlot >= _countof(FpsHist) * TgtFPS)						//�X���b�g�X�V
				HistSlot = 0;
			float fFps = static_cast<float>(m_App.m_Time.GetFPS());
			if (HistSlot % TgtFPS == 0) {
				for (size_t i = 1, Cnt = _countof(FpsHist); i < Cnt; i++)	//�f�[�^�V�t�g
					FpsHist[i - 1] = FpsHist[i];
				FpsHist[_countof(FpsHist) - 1] = fFps;						//����ǉ�
			}
			ImGui::Text(U8(u8"FPS�v��")); ImGui::SameLine();
			ImGui::Text(": %.3f ms/frame (%.0f)", 1000.0f / fFps, fFps);
			ImGui::PlotLines(U8(u8"RT�}"), FpsHist, IM_ARRAYSIZE(FpsHist), 0, 0, 0.0f, 60.0f);

			//FBX Viewer
			ImGui::NewLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.5f, 1.0f), "FBX Viewer  "); ImGui::SameLine();
			ImGui::Checkbox(U8(u8"�L��"), &m_bFBX_Viewer);
			if (m_bFBX_Viewer && m_pViewer != nullptr) {
				ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 0), ImGuiWindowFlags_NoTitleBar);

				//���f���Ǎ�
				if (ImGui::Button(U8(u8"���f���Ǎ�")))
					m_pViewer->LoadModel();

				//�A�j���[�V�����Ǎ�
				ImGui::SameLine();
				if (ImGui::Button(U8(u8"�A�j���[�V�����Ǎ�"))) {
					if (m_pViewer->GetDrawerNum() > 1)
						m_pViewer->LoadModel(true);
				}
				if (m_pViewer->GetDrawerNum() <= 1) {
					ImGui::Text(U8(u8"���F���f�������̏ꍇ�A"));
					ImGui::Text(U8(u8"�A�j���[�V�����Ǎ��͖����B"));
					ImGui::NewLine();
				}

				//�|���S�����\��
				UINT PolyNum = m_pViewer->GetPolygonNum();
				ImGui::Text(U8(u8"�|���S�����i�O�p�`�j")); ImGui::SameLine(); ImGui::Text(": %d", PolyNum);

				//�@���\���ؑ�
				if (PolyNum > 0 && !m_pViewer->GetFlag_DrawAnimation())
					ImGui::Checkbox(U8(u8"�@���\��"), &m_pViewer->GetFlag_DrawNormal());

				//�|���S���\���ؑ�
				if (PolyNum > 0 && !m_pViewer->GetFlag_DrawAnimation()) {
					ImGui::SameLine(); ImGui::Checkbox(U8(u8"�|���S���i�ʁj�\��"), &m_pViewer->GetFlag_DrawSurface());
				}

				//���\���ؑ�
				if (PolyNum > 0 && m_pViewer->GetLoader().GetSkeleton().size() > 0) {
					ImGui::Checkbox(U8(u8"���\��  "), &m_pViewer->GetFlag_DrawBone()); ImGui::SameLine();
				}

				//�A�j���[�V�����Đ�
				if (PolyNum > 0 && m_pViewer->GetLoader().GetAnimation().size() > 0) {
					ImGui::Checkbox(U8(u8"�A�j���[�V�����Đ�"), &m_pViewer->GetFlag_DrawAnimation());
					if (m_pViewer->GetFlag_DrawAnimation()) {
						ImGui::NewLine();
						ImGui::Text(U8(u8"�A�j���[�V����ID")); ImGui::SameLine(); ImGui::Text(": %d", m_pViewer->GetAnimationID() + 1); ImGui::SameLine();
						if (ImGui::Button("-")) {
							m_pViewer->GetAnimationID()--;
							if (m_pViewer->GetAnimationID() < 0)
								m_pViewer->GetAnimationID() = 0;
						}
						ImGui::SameLine();
						if (ImGui::Button("+")) {
							m_pViewer->GetAnimationID()++;
							if (m_pViewer->GetAnimationID() > m_pViewer->GetLoader().GetAnimation().size() - 1)
								m_pViewer->GetAnimationID() = static_cast<int>(m_pViewer->GetLoader().GetAnimation().size()) - 1;
						}

						if (ImGui::Button(U8(u8"�Đ��ؑ�")))
							m_pViewer->GetFlag_AnimPause() = !m_pViewer->GetFlag_AnimPause();
						ImGui::SameLine(); ImGui::Text(U8(u8"���")); ImGui::SameLine();
						if (m_pViewer->GetFlag_AnimPause()) {
							ImGui::Text(":"); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), U8(u8"�ꎞ��~"));
							auto& AnimData = m_pViewer->GetLoader().GetAnimation()[m_pViewer->GetAnimationID()];
							ImGui::SliderInt(U8(u8"�t���[����"), &m_pViewer->GetAnimationFrame(), AnimData.StartFrame, AnimData.StopFrame - 1);
						}
						else {
							ImGui::Text(":"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), U8(u8"�Đ���"));
							ImGui::Text(U8(u8"�t���[����")); ImGui::SameLine(); ImGui::Text(": %d", m_pViewer->GetAnimationFrame());
						}
					}
				}
				ImGui::NewLine();

				//�t�@�C�����o��
				if (PolyNum > 0) {
					if (ImGui::TreeNode(U8(u8"�t�@�C�����o��"))) {

						static char ModelName[MAX_PATH]{ '\0' };
						ImGui::Text(U8(u8"���f����"));
						ImGui::InputText("mIn", ModelName, sizeof(ModelName));
						if (ImGui::Button(U8(u8"���f�����o��"))) {
							std::string Name = ModelName;
							if (Name.size() > 0)
								m_pViewer->GetLoader().SaveModelData(Name.c_str());
						}

						static char AnimName[MAX_PATH]{ '\0' };
						ImGui::Text(U8(u8"�A�j���[�V������"));
						ImGui::InputText("aIn", AnimName, sizeof(AnimName));
						if (ImGui::Button(U8(u8"�A�j���[�V�������o��"))) {
							std::string Name = AnimName;
							if (Name.size() > 0)
								m_pViewer->GetLoader().SaveAnimData(Name.c_str(), m_pViewer->GetAnimationID());
						}

						ImGui::TreePop();
					}
				}

				//���f������
				if (PolyNum > 0) {
					if (ImGui::TreeNode(U8(u8"���f��"))) {

						//���b�V���X�P�[��
						float Scale = m_pViewer->GetModelScale();
						ImGui::SliderFloat(U8(u8"�X�P�[��"), &Scale, 0.0f, 1.0f, "%.2f");
						m_pViewer->GetModelScale() = Scale;

						//���b�V����](Y��)
						float RotY = gMath::GetDegree_f(m_pViewer->GetModelRotation());
						ImGui::SliderFloat(U8(u8"Y����]"), &RotY, -180.0f, 180.0f, "%.f");
						m_pViewer->GetModelRotation() = gMath::GetRad(RotY);

						//�}�e���A������
						ImGui::Checkbox(U8(u8"���ʔ��ˌ�"), &m_pViewer->GetSpecularFlag()); ImGui::SameLine();
						ImGui::Checkbox(U8(u8"�@���}�b�v"), &m_pViewer->GetNormalMapFlag());
						if (m_pViewer->GetNormalMapFlag())
							ImGui::Checkbox(U8(u8"�����}�b�v"), &m_pViewer->GetDispMapFlag());
						else
							m_pViewer->GetDispMapFlag() = false;

						if (m_pViewer->GetDispMapFlag()) {
							if (ImGui::TreeNode(U8(u8"�����}�b�v����"))) {
								ImGui::SliderFloat(U8(u8"�ŏ��T���v�����O��"), &m_pViewer->GetDispCoef().x, 1.0f, 32.0f, "%.0f");
								ImGui::SliderFloat(U8(u8"�ő�T���v�����O��"), &m_pViewer->GetDispCoef().y, 2.0f, 64.0f, "%.0f");
								ImGui::SliderFloat(U8(u8"�����[��"), &m_pViewer->GetDispCoef().z, 0.0f, 1.0f, "%.2f");

								ImGui::TreePop();
							}
						}

						ImGui::TreePop();
					}
				}

				//������
				if (PolyNum > 0) {
					if (ImGui::TreeNode(U8(u8"�{�[��"))) {

						//���X�P�[��
						float Scale = m_pViewer->GetBone().GetScale();
						ImGui::SliderFloat(U8(u8"�X�P�[��"), &Scale, 0.0f, 5.0f, "%.1f");
							m_pViewer->GetBone().GetScale() = Scale;

						ImGui::TreePop();
					}
				}

				//���C�g����
				if (PolyNum > 0) {
					if (ImGui::TreeNode(U8(u8"���C�g"))) {

						//���C�g���W
						dx::XMFLOAT3& LightPos = m_pViewer->GetLightPos();
						ImGui::SliderFloat(U8(u8"���WX"), &LightPos.x, -1.0f, 1.0f, "%.1f");
						ImGui::SliderFloat(U8(u8"���WY"), &LightPos.y, -1.0f, 1.0f, "%.1f");
						ImGui::SliderFloat(U8(u8"���WZ"), &LightPos.z, -1.0f, 1.0f, "%.1f");

						ImGui::TreePop();
					}
				}

				//�J�������
				if (ImGui::TreeNode(U8(u8"�J����"))) {

					//�o�͏���
					dx::XMFLOAT3 CamRot = m_pViewer->GetCamera().GetRotation();
					ImGui::Text(U8(u8"�@��]�i���]�j�p�x"));
					ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "X"); ImGui::SameLine(); ImGui::Text(": %d deg. ", static_cast<int>(CamRot.x / fPI * 180)); ImGui::SameLine();
					ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Y"); ImGui::SameLine(); ImGui::Text(": %d deg.", static_cast<int>(CamRot.y / fPI * 180));
					ImGui::Text(U8(u8"�@���S����")); ImGui::SameLine(); ImGui::Text(": %.1ff", m_pViewer->GetCamera().GetCenterDistance());
					ImGui::SliderInt(U8(u8"�Y�[�����x"), &m_pViewer->GetCamera().GetZoomSpeed(), 1, 10);
					dx::XMFLOAT3 CamPos = m_pViewer->GetCamera().GetPosition();
					ImGui::Text(U8(u8"�@�ʒu"));
					ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "X"); ImGui::SameLine(); ImGui::Text(": %.1ff ", CamPos.x); ImGui::SameLine();
					ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Y"); ImGui::SameLine(); ImGui::Text(": %.1ff ", CamPos.y); ImGui::SameLine();
					ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f), "Z"); ImGui::SameLine(); ImGui::Text(": %.1ff", CamPos.z);

					//�J�������Z�b�g
					if (ImGui::Button(U8(u8"�J�������Z�b�g")))
						m_pViewer->GetCamera().ResetCamera();

					ImGui::TreePop();
				}
				ImGui::EndChild();
			}
		}
		ImGui::End();
	}

#endif // IMGUI

}
