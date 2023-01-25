
//===== インクルード部 =====
#include <EditorApp/Editor.h>
#include <EditorApp/ModelViewer/Viewer.h>
#include <EditorApp/ModelViewer/Bone.h>
#include <Tool/gMath.h>

namespace dx = DirectX;

//===== クラス実装 =====
EDITOR::EDITOR(APP& App) noexcept :
	m_bEnable(RUN_EDITOR), m_App(App), m_bFBX_Viewer(false), m_pViewer()
{
}

EDITOR::~EDITOR() noexcept
{
}

//更新処理
void EDITOR::Update()
{
	//ビューワ処理
	if (m_bFBX_Viewer) {

		//ビューワ初期化
		if (m_pViewer == nullptr)
			m_pViewer = std::make_unique<VIEWER>(m_App);

		//ビューワ更新
		m_pViewer->Update();
	}
	else {

		//ビューワ終了
		if (m_pViewer != nullptr)
			m_pViewer.reset();
	}
}

//描画処理
void EDITOR::Draw()
{
	//ビューワ描画
	if (m_bFBX_Viewer)
		m_pViewer->Draw();

#ifdef IMGUI

	//開始メニュー
	if (m_App.m_pDX->IsImGuiEnabled()) {

		//テストメニュー
		if (ImGui::Begin("Game Editor Ver0.5"))
		{
			//エディタ終了
			if (ImGui::Button(U8(u8"ゲームモード開始")))
				m_bEnable = false;

			//性能計測
			static float FpsHist[10]{ 0.0f };																//計測履歴
			static unsigned short HistSlot = 0;																//計測スロット
			static unsigned short TgtFPS = static_cast<unsigned short>(m_App.m_Time.GetTargetFPS() / 2);	//計測時刻（フレーム単位）
			HistSlot++;
			if (HistSlot >= _countof(FpsHist) * TgtFPS)						//スロット更新
				HistSlot = 0;
			float fFps = static_cast<float>(m_App.m_Time.GetFPS());
			if (HistSlot % TgtFPS == 0) {
				for (size_t i = 1, Cnt = _countof(FpsHist); i < Cnt; i++)	//データシフト
					FpsHist[i - 1] = FpsHist[i];
				FpsHist[_countof(FpsHist) - 1] = fFps;						//履歴追加
			}
			ImGui::Text(U8(u8"FPS計測")); ImGui::SameLine();
			ImGui::Text(": %.3f ms/frame (%.0f)", 1000.0f / fFps, fFps);
			ImGui::PlotLines(U8(u8"RT図"), FpsHist, IM_ARRAYSIZE(FpsHist), 0, 0, 0.0f, 60.0f);

			//FBX Viewer
			ImGui::NewLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.5f, 1.0f), "FBX Viewer  "); ImGui::SameLine();
			ImGui::Checkbox(U8(u8"有効"), &m_bFBX_Viewer);
			if (m_bFBX_Viewer && m_pViewer != nullptr) {
				ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 0), ImGuiWindowFlags_NoTitleBar);

				//モデル読込
				if (ImGui::Button(U8(u8"モデル読込")))
					m_pViewer->LoadModel();

				//アニメーション読込
				ImGui::SameLine();
				if (ImGui::Button(U8(u8"アニメーション読込"))) {
					if (m_pViewer->GetDrawerNum() > 1)
						m_pViewer->LoadModel(true);
				}
				if (m_pViewer->GetDrawerNum() <= 1) {
					ImGui::Text(U8(u8"注：モデル無しの場合、"));
					ImGui::Text(U8(u8"アニメーション読込は無効。"));
					ImGui::NewLine();
				}

				//ポリゴン数表示
				UINT PolyNum = m_pViewer->GetPolygonNum();
				ImGui::Text(U8(u8"ポリゴン数（三角形）")); ImGui::SameLine(); ImGui::Text(": %d", PolyNum);

				//法線表示切替
				if (PolyNum > 0 && !m_pViewer->GetFlag_DrawAnimation())
					ImGui::Checkbox(U8(u8"法線表示"), &m_pViewer->GetFlag_DrawNormal());

				//ポリゴン表示切替
				if (PolyNum > 0 && !m_pViewer->GetFlag_DrawAnimation()) {
					ImGui::SameLine(); ImGui::Checkbox(U8(u8"ポリゴン（面）表示"), &m_pViewer->GetFlag_DrawSurface());
				}

				//骨表示切替
				if (PolyNum > 0 && m_pViewer->GetLoader().GetSkeleton().size() > 0) {
					ImGui::Checkbox(U8(u8"骨表示  "), &m_pViewer->GetFlag_DrawBone()); ImGui::SameLine();
				}

				//アニメーション再生
				if (PolyNum > 0 && m_pViewer->GetLoader().GetAnimation().size() > 0) {
					ImGui::Checkbox(U8(u8"アニメーション再生"), &m_pViewer->GetFlag_DrawAnimation());
					if (m_pViewer->GetFlag_DrawAnimation()) {
						ImGui::NewLine();
						ImGui::Text(U8(u8"アニメーションID")); ImGui::SameLine(); ImGui::Text(": %d", m_pViewer->GetAnimationID() + 1); ImGui::SameLine();
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

						if (ImGui::Button(U8(u8"再生切替")))
							m_pViewer->GetFlag_AnimPause() = !m_pViewer->GetFlag_AnimPause();
						ImGui::SameLine(); ImGui::Text(U8(u8"状態")); ImGui::SameLine();
						if (m_pViewer->GetFlag_AnimPause()) {
							ImGui::Text(":"); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), U8(u8"一時停止"));
							auto& AnimData = m_pViewer->GetLoader().GetAnimation()[m_pViewer->GetAnimationID()];
							ImGui::SliderInt(U8(u8"フレーム数"), &m_pViewer->GetAnimationFrame(), AnimData.StartFrame, AnimData.StopFrame - 1);
						}
						else {
							ImGui::Text(":"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), U8(u8"再生中"));
							ImGui::Text(U8(u8"フレーム数")); ImGui::SameLine(); ImGui::Text(": %d", m_pViewer->GetAnimationFrame());
						}
					}
				}
				ImGui::NewLine();

				//ファイル書出し
				if (PolyNum > 0) {
					if (ImGui::TreeNode(U8(u8"ファイル書出し"))) {

						static char ModelName[MAX_PATH]{ '\0' };
						ImGui::Text(U8(u8"モデル名"));
						ImGui::InputText("mIn", ModelName, sizeof(ModelName));
						if (ImGui::Button(U8(u8"モデル書出し"))) {
							std::string Name = ModelName;
							if (Name.size() > 0)
								m_pViewer->GetLoader().SaveModelData(Name.c_str());
						}

						static char AnimName[MAX_PATH]{ '\0' };
						ImGui::Text(U8(u8"アニメーション名"));
						ImGui::InputText("aIn", AnimName, sizeof(AnimName));
						if (ImGui::Button(U8(u8"アニメーション書出し"))) {
							std::string Name = AnimName;
							if (Name.size() > 0)
								m_pViewer->GetLoader().SaveAnimData(Name.c_str(), m_pViewer->GetAnimationID());
						}

						ImGui::TreePop();
					}
				}

				//モデル制御
				if (PolyNum > 0) {
					if (ImGui::TreeNode(U8(u8"モデル"))) {

						//メッシュスケール
						float Scale = m_pViewer->GetModelScale();
						ImGui::SliderFloat(U8(u8"スケール"), &Scale, 0.0f, 1.0f, "%.2f");
						m_pViewer->GetModelScale() = Scale;

						//メッシュ回転(Y軸)
						float RotY = gMath::GetDegree_f(m_pViewer->GetModelRotation());
						ImGui::SliderFloat(U8(u8"Y軸回転"), &RotY, -180.0f, 180.0f, "%.f");
						m_pViewer->GetModelRotation() = gMath::GetRad(RotY);

						//マテリアル制御
						ImGui::Checkbox(U8(u8"鏡面反射光"), &m_pViewer->GetSpecularFlag()); ImGui::SameLine();
						ImGui::Checkbox(U8(u8"法線マップ"), &m_pViewer->GetNormalMapFlag());
						if (m_pViewer->GetNormalMapFlag())
							ImGui::Checkbox(U8(u8"視差マップ"), &m_pViewer->GetDispMapFlag());
						else
							m_pViewer->GetDispMapFlag() = false;

						if (m_pViewer->GetDispMapFlag()) {
							if (ImGui::TreeNode(U8(u8"視差マップ調整"))) {
								ImGui::SliderFloat(U8(u8"最小サンプリング数"), &m_pViewer->GetDispCoef().x, 1.0f, 32.0f, "%.0f");
								ImGui::SliderFloat(U8(u8"最大サンプリング数"), &m_pViewer->GetDispCoef().y, 2.0f, 64.0f, "%.0f");
								ImGui::SliderFloat(U8(u8"視差深さ"), &m_pViewer->GetDispCoef().z, 0.0f, 1.0f, "%.2f");

								ImGui::TreePop();
							}
						}

						ImGui::TreePop();
					}
				}

				//骨制御
				if (PolyNum > 0) {
					if (ImGui::TreeNode(U8(u8"ボーン"))) {

						//骨スケール
						float Scale = m_pViewer->GetBone().GetScale();
						ImGui::SliderFloat(U8(u8"スケール"), &Scale, 0.0f, 5.0f, "%.1f");
							m_pViewer->GetBone().GetScale() = Scale;

						ImGui::TreePop();
					}
				}

				//ライト制御
				if (PolyNum > 0) {
					if (ImGui::TreeNode(U8(u8"ライト"))) {

						//ライト座標
						dx::XMFLOAT3& LightPos = m_pViewer->GetLightPos();
						ImGui::SliderFloat(U8(u8"座標X"), &LightPos.x, -1.0f, 1.0f, "%.1f");
						ImGui::SliderFloat(U8(u8"座標Y"), &LightPos.y, -1.0f, 1.0f, "%.1f");
						ImGui::SliderFloat(U8(u8"座標Z"), &LightPos.z, -1.0f, 1.0f, "%.1f");

						ImGui::TreePop();
					}
				}

				//カメラ情報
				if (ImGui::TreeNode(U8(u8"カメラ"))) {

					//出力処理
					dx::XMFLOAT3 CamRot = m_pViewer->GetCamera().GetRotation();
					ImGui::Text(U8(u8"　回転（公転）角度"));
					ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "X"); ImGui::SameLine(); ImGui::Text(": %d deg. ", static_cast<int>(CamRot.x / fPI * 180)); ImGui::SameLine();
					ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Y"); ImGui::SameLine(); ImGui::Text(": %d deg.", static_cast<int>(CamRot.y / fPI * 180));
					ImGui::Text(U8(u8"　中心距離")); ImGui::SameLine(); ImGui::Text(": %.1ff", m_pViewer->GetCamera().GetCenterDistance());
					ImGui::SliderInt(U8(u8"ズーム速度"), &m_pViewer->GetCamera().GetZoomSpeed(), 1, 10);
					dx::XMFLOAT3 CamPos = m_pViewer->GetCamera().GetPosition();
					ImGui::Text(U8(u8"　位置"));
					ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "X"); ImGui::SameLine(); ImGui::Text(": %.1ff ", CamPos.x); ImGui::SameLine();
					ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Y"); ImGui::SameLine(); ImGui::Text(": %.1ff ", CamPos.y); ImGui::SameLine();
					ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f), "Z"); ImGui::SameLine(); ImGui::Text(": %.1ff", CamPos.z);

					//カメラリセット
					if (ImGui::Button(U8(u8"カメラリセット")))
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
