
//===== �C���N���[�h�� =====
#include <EditorApp/ModelViewer/Viewer.h>
#include <Light/LightMgr.h>
#include <EditorApp/ModelViewer/Grid.h>
#include <filesystem>
#include <EditorApp/ModelViewer/Viewer_Model.h>
#include <EditorApp/ModelViewer/Normal.h>
#include <EditorApp/ModelViewer/Surface.h>
#include <EditorApp/ModelViewer/Bone.h>
#include <EditorApp/ModelViewer/BoneLine.h>

//===== �N���X���� =====
VIEWER::VIEWER(APP& App) :
	m_Gfx(App.GetGfxPack()), m_Input(App.GetInputMgr()), m_aDrawer(), m_Camera(App), m_LightMgr(App.GetLightMgr()),
	m_pLoader(), m_Scale(1.0f), m_RotY(0.0f), m_bDrawNormal(false), m_bDrawSurface(false),
	m_bDrawAnimation(false), m_AnimationID(0), m_AnimFrame(0), m_bAnimPause(false),
	m_Drawer_Bone(), m_Drawer_BoneLine(), m_bDrawBone(false),
	m_LightPos(-1.0f, 1.0f, -1.0f), m_bUseSpecular(true), m_bUseNormalMap(true), m_bUseDispMap(true), m_DispCoef(1.0f, 2.0f, 0.0f)
{
	//���f�����[�_������
	m_pLoader = std::make_unique<FBX_LOADER>();

	//�O���b�h������
	m_aDrawer.push_back(std::make_unique<GRID>(m_Gfx.m_DX, m_Gfx.m_ShaderMgr));

	//���`��p���b�V��������
	m_Drawer_Bone = std::make_unique<BONE>(m_Gfx, *this, *m_pLoader, m_Input);
}

VIEWER::~VIEWER() noexcept
{
	//���������
	if (!m_aDrawer.empty()) {
		for (auto& d : m_aDrawer)
			d.reset();
	}
	if (m_Drawer_Bone != nullptr)
		m_Drawer_Bone.reset();
	if (m_Drawer_BoneLine != nullptr)
		m_Drawer_BoneLine.reset();
}

//�X�V����
void VIEWER::Update() noexcept
{
	//�J�����X�V
	m_Camera.Update();

	//���f����]����
	if (m_aDrawer.size() > 1) {
		if (m_Input.m_KB.GetPress(VK_A))
			m_RotY -= gMath::GetRad(2);
		else if (m_Input.m_KB.GetPress(VK_D))
			m_RotY += gMath::GetRad(2);
		if (m_Input.m_KB.GetPress(VK_R))
			m_RotY = 0.0f;

		//��]�͈͐���
		if (m_RotY > gMath::GetRad(180))
			m_RotY -= gMath::GetRad(360);
		else if (m_RotY < -gMath::GetRad(180))
			m_RotY += gMath::GetRad(360);
	}

	//���f���X�V
	for (auto& d : m_aDrawer)
		d->Update();
	m_Drawer_Bone->Update();
	if (m_Drawer_BoneLine != nullptr)
		m_Drawer_BoneLine->Update();

	//���C�g�}�l�[�W���X�V
	m_LightMgr.Update();
}

//�`�揈��
void VIEWER::Draw() const noexcept
{
	if (m_aDrawer.size() > 1) {

		//�O���b�h
		m_aDrawer[0]->Draw();
		if (!m_bDrawBone) {
			for (size_t i = 0, Cnt = m_pLoader->GetMeshCount(); i < Cnt; i++) {

				//���f���i�|���S���j
				if (m_bDrawSurface)
					m_aDrawer[3 + i * 3]->Draw();
				else
					m_aDrawer[1 + i * 3]->Draw();

				//�@��
				if (m_bDrawNormal)
					m_aDrawer[2 + i * 3]->Draw();
			}
		}
		else {

			//���`��
			m_Drawer_Bone->Draw();
			m_Drawer_BoneLine->Draw();
		}
	}
	else {
		for (auto& d : m_aDrawer)
			d->Draw();
	}

#ifdef IMGUI

	if (m_Gfx.m_DX.IsImGuiEnabled())
		m_pLoader->Draw();

#endif // IMGUI

}

//�|���S�����擾
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

//���f���Ǎ�
void VIEWER::LoadModel(bool bAnimOnly)
{
	//�J�����g�f�B���N�g����ۑ�
	std::filesystem::path p = std::filesystem::current_path().c_str();

	//�u�t�@�C�����J���v�_�C�A���O�̕\��
	std::wstring strFileName{};
	strFileName.reserve(MAX_PATH);
	strFileName = p.c_str();
	strFileName += L"\\Asset\\Model\\Input\\Folder";

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFile = (LPWSTR)strFileName.c_str();			//�u�J���v�őI�������t�@�C���̃p�X���i�[
	ofn.nMaxFile = (DWORD)strFileName.capacity();
	ofn.lpstrFilter = L"All Files\0*.*\0.fbx\0*.fbx\0";		//�t�@�C���g���q�̃t�B���^�ݒ�
	ofn.nFilterIndex = 2;									//�ŏ��Ɏw�肵�Ă���t�B���^
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	//�_�C�A���O��\��
	if (TRUE == GetOpenFileName(&ofn)) {

		//�J�����g�f�B���N�g���Đݒ�
		std::filesystem::current_path(p);

		//FBX�t�@�C���̓Ǎ�
		if (!bAnimOnly) {

			//���̃��f�������
			while (m_aDrawer.size() > 1)
				m_aDrawer.erase(m_aDrawer.begin() + 1);		//�O���b�h�����c��
			if (m_Drawer_Bone->GetPolygonNum() > 0)
				m_Drawer_Bone->ClearInstance();
			if (m_Drawer_BoneLine != nullptr)
				m_Drawer_BoneLine.reset();
			m_Scale = 1.0f;
			m_RotY = 0.0f;
			m_bDrawAnimation = false;
			m_AnimationID = 0;
			m_AnimFrame = 0;
			m_bAnimPause = false;

			//�t�@�C���Ǎ�
			m_pLoader->Load(strFileName.c_str());

			//���f���쐬
			for (size_t i = 0, Cnt = m_pLoader->GetMeshCount(); i < Cnt; i++) {
				m_aDrawer.push_back(std::make_unique<VIEWER_MODEL>(m_Gfx, *this, *m_pLoader, static_cast<int>(i)));
				m_aDrawer.push_back(std::make_unique<NORMAL>(m_Gfx.m_DX, m_Gfx.m_ShaderMgr, *m_pLoader, static_cast<int>(i), *m_aDrawer[1 + 3 * i]));
				m_aDrawer.push_back(std::make_unique<SURFACE>(m_Gfx.m_DX, m_Gfx.m_ShaderMgr, *m_pLoader, static_cast<int>(i), *m_aDrawer[1 + 3 * i]));
			}

			//���`��p���b�V���ǉ�
			if (m_pLoader->GetSkeleton().size() > 0) {
				for (size_t i = 0, Cnt = m_pLoader->GetSkeleton().size(); i < Cnt; i++)
					m_Drawer_Bone->AddInstance();
				m_Drawer_BoneLine = std::make_unique<BONE_LINE>(m_Gfx.m_DX, m_Gfx.m_ShaderMgr, *this, *m_pLoader, *m_Drawer_Bone);
			}
		}
		else {

			//���b�V���f�[�^���Ȃ��ꍇ�͏��O
			if (m_aDrawer.size() <= 1)
				return;

			//�A�j���[�V�����f�[�^�̂ݓǍ�
			m_pLoader->Load(strFileName.c_str(), true);
		}
	}
}

//�����b�V���Q��
BONE& VIEWER::GetBone() const noexcept
{
	return dynamic_cast<BONE&>(*m_Drawer_Bone);
}
