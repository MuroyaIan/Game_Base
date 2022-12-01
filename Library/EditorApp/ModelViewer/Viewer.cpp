
//===== インクルード部 =====
#include <EditorApp/ModelViewer/Viewer.h>
#include <EditorApp/ModelViewer/Grid.h>
#include <filesystem>
#include <EditorApp/ModelViewer/Viewer_Model.h>
#include <EditorApp/ModelViewer/Normal.h>
#include <EditorApp/ModelViewer/Surface.h>
#include <EditorApp/ModelViewer/Bone.h>
#include <EditorApp/ModelViewer/BoneLine.h>

//===== クラス実装 =====
VIEWER::VIEWER(APP& App) :
	m_App(App), m_Gfx(App.GetGfxPack()), m_aDrawer(), m_Camera(App),
	m_pLoader(), m_Scale(1.0f), m_bDrawNormal(false), m_bDrawSurface(false),
	m_bDrawAnimation(false), m_AnimationID(0), m_AnimFrame(0), m_bAnimPause(false),
	m_Drawer_Bone(), m_Drawer_BoneLine(), m_bDrawBone(false)
{
	//モデルローダ初期化
	m_pLoader = std::make_unique<FBX_LOADER>();

	//グリッド初期化
	m_aDrawer.push_back(std::make_unique<GRID>(m_Gfx.m_DX, m_Gfx.m_ShaderMgr));

	//骨描画用メッシュ初期化
	m_Drawer_Bone = std::make_unique<BONE>(m_Gfx, *this, *m_pLoader, m_App.GetInputMgr());
}

VIEWER::~VIEWER() noexcept
{
	if (!m_aDrawer.empty()) {
		for (auto& d : m_aDrawer)
			d.reset();
	}
	if (m_Drawer_Bone != nullptr)
		m_Drawer_Bone.reset();
	if (m_Drawer_BoneLine != nullptr)
		m_Drawer_BoneLine.reset();
}

//更新処理
void VIEWER::Update() noexcept
{
	for (auto& d : m_aDrawer)
		d->Update();
	m_Drawer_Bone->Update();
	if (m_Drawer_BoneLine != nullptr)
		m_Drawer_BoneLine->Update();

	//カメラ更新
	m_Camera.Update();
}

//描画処理
void VIEWER::Draw() const
{
	if (m_aDrawer.size() > 1) {

		//グリッド
		m_aDrawer[0]->Draw();
		if (!m_bDrawBone) {
			for (size_t i = 0, Cnt = m_pLoader->GetMeshCount(); i < Cnt; i++) {

				//モデル（ポリゴン）
				if (m_bDrawSurface)
					m_aDrawer[3 + i * 3]->Draw();
				else
					m_aDrawer[1 + i * 3]->Draw();

				//法線
				if (m_bDrawNormal)
					m_aDrawer[2 + i * 3]->Draw();
			}
		}
		else {

			//骨描画
			m_Drawer_Bone->Draw();
			m_Drawer_BoneLine->Draw();
		}
	}
	else {
		for (auto& d : m_aDrawer)
			d->Draw();
	}

#ifdef IMGUI

	m_pLoader->Draw();

#endif // IMGUI

}

//ポリゴン数取得
int VIEWER::GetPolygonNum() const noexcept
{
	if (m_aDrawer.size() > 1) {
		int Num{0};
		for (size_t i = 0, Cnt = (m_aDrawer.size() - 1) / 3; i < Cnt; i++)
			Num += m_aDrawer[1 + i * 3]->GetPolygonNum();
		return Num;
	}
	else
		return 0;
}

//モデル読込
void VIEWER::LoadModel(bool bAnimOnly)
{
	//カレントディレクトリを保存
	std::filesystem::path p = std::filesystem::current_path().c_str();

	//「ファイルを開く」ダイアログの表示
	std::wstring strFileName{};
	strFileName.reserve(MAX_PATH);
	strFileName = p.c_str();
	strFileName += L"\\Asset\\Model";

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFile = (LPWSTR)strFileName.c_str();			//「開く」で選択したファイルのパスを格納
	ofn.nMaxFile = (DWORD)strFileName.capacity();
	ofn.lpstrFilter = L"All Files\0*.*\0.fbx\0*.fbx\0";		//ファイル拡張子のフィルタ設定
	ofn.nFilterIndex = 2;									//最初に指定しているフィルタ
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	//ダイアログを表示
	if (TRUE == GetOpenFileName(&ofn)) {

		//カレントディレクトリ再設定
		std::filesystem::current_path(p);

		//FBXファイルの読込
		if (!bAnimOnly) {

			//今のモデルを解放
			while (m_aDrawer.size() > 1)
				m_aDrawer.erase(m_aDrawer.begin() + 1);
			if (m_Drawer_Bone->GetPolygonNum() > 0)
				m_Drawer_Bone->ClearInstance();
			if (m_Drawer_BoneLine != nullptr)
				m_Drawer_BoneLine.reset();
			m_bDrawAnimation = false;
			m_AnimationID = 0;
			m_AnimFrame = 0;
			m_bAnimPause = false;

			//ファイル読込
			m_pLoader->Load(strFileName.c_str());

			//モデル作成
			for (size_t i = 0, Cnt = m_pLoader->GetMeshCount(); i < Cnt; i++) {
				m_aDrawer.push_back(std::make_unique<VIEWER_MODEL>(m_Gfx.m_DX, m_Gfx.m_ShaderMgr, *this, *m_pLoader, static_cast<int>(i), m_App.GetInputMgr()));
				m_aDrawer.push_back(std::make_unique<NORMAL>(m_Gfx.m_DX, m_Gfx.m_ShaderMgr, *m_pLoader, static_cast<int>(i), *m_aDrawer[1 + 3 * i]));
				m_aDrawer.push_back(std::make_unique<SURFACE>(m_Gfx.m_DX, m_Gfx.m_ShaderMgr, *m_pLoader, static_cast<int>(i), *m_aDrawer[1 + 3 * i]));
			}

			//骨描画用メッシュ追加
			if (m_pLoader->GetSkeleton().size() > 0) {
				for (size_t i = 0, Cnt = m_pLoader->GetSkeleton().size(); i < Cnt; i++)
					m_Drawer_Bone->AddInstance();
				m_Drawer_BoneLine = std::make_unique<BONE_LINE>(m_Gfx.m_DX, m_Gfx.m_ShaderMgr, *this, *m_pLoader, *m_Drawer_Bone);
			}
		}
		else {

			//メッシュデータがない場合は除外
			if (m_aDrawer.size() <= 1)
				return;

			//アニメーションデータのみ読込
			m_pLoader->Load(strFileName.c_str(), true);
		}
	}
}

//骨メッシュ参照
BONE& VIEWER::GetBone() const noexcept
{
	BONE* a = dynamic_cast<BONE*>(m_Drawer_Bone.get());
	return *a;
}
