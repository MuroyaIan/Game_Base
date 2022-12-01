
//===== �C���N���[�h�� =====
#include <EditorApp/ModelViewer/Viewer_Model.h>
#include <GraphicApp/Binder/BinderRef.h>
#include <Tool/gMath.h>
#include <EditorApp/ModelViewer/Viewer.h>
#include <Tool/Input/InputMgr.h>

namespace dx = DirectX;

//===== �N���X���� =====
VIEWER_MODEL::VIEWER_MODEL(GRAPHIC& Gfx, SHADER_MGR& ShaderMgr, VIEWER& Viewer, FBX_LOADER& Loader, int MeshIndex, INPUT_MGR& Input) :
	DRAWER_EX(Gfx), m_ShaderMgr(ShaderMgr), m_Viewer(Viewer), m_Loader(Loader), m_MeshIndex(MeshIndex), m_MtxLocal(), m_MtxWorld(), m_Material(), m_bNoBone(false),
	m_pMtxBone(), m_bDrawAnimation(m_Viewer.GetFlag_DrawAnimation()), m_AnimationID(m_Viewer.GetAnimationID()), m_AnimFrame(0), m_FrameCnt(0), m_AnimPause(m_Viewer.GetFlag_AnimPause()),
	m_Input(Input), m_Scale(Viewer.GetModelScale()), m_Rot()
{
	//���_���쐬
	VS_DATA<VERTEX_MB> Model = MakeData_VS();
	AddBind(std::make_unique<VERTEX_BUFFER>(Gfx, Model.m_Vertices));

	//�C���f�b�N�X���쐬
	AddBind(std::make_unique<INDEX_BUFFER>(Gfx, Model.m_Indices));

	//�萔�o�b�t�@�쐬�i�ϊ��s��j
	AddBind(std::make_unique<CB_MTX_LWVP>(Gfx, *this, m_MtxLocal));

	//�萔�o�b�t�@�쐬�i�����j
	m_pMtxBone = std::make_unique<CBD_BONE>();
	AddBind(std::make_unique<CB_BONE>(Gfx, *m_pMtxBone, true));

	//�萔�o�b�t�@�쐬�i�}�e���A���j
	AddBind(std::make_unique<CB_MATERIAL>(Gfx, m_Material));

	//�e�N�X�`���o�b�t�@�쐬
	auto& MeshData = m_Loader.GetMesh(m_MeshIndex);
	std::vector<TEX_LOADER::TEX_DATA> aData(static_cast<int>(TEXTURE_MODEL::TEX_TYPE::MAX_TYPE));
	if (MeshData.aTex_Diffuse.size() > 0) {
		std::string Path = m_Loader.GetFilePath();
		Path += MeshData.aTex_Diffuse[0];
		aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::DIFFUSE)] = TEX_LOADER::LoadTexture(Path.c_str());
	}
	else
		aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::DIFFUSE)] = TEX_LOADER::LoadTexture("Asset/Texture/null.png");
	if (MeshData.aTex_Specular.size() > 0) {
		std::string Path = m_Loader.GetFilePath();
		Path += MeshData.aTex_Specular[0];
		aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::SPECULAR)] = TEX_LOADER::LoadTexture(Path.c_str());
	}
	else
		aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::SPECULAR)] = TEX_LOADER::LoadTexture("Asset/Texture/null.png");
	aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::NORMAL)] = TEX_LOADER::LoadTexture("Asset/Texture/null.png");

	AddBind(std::make_unique<TEXTURE_MODEL>(Gfx, aData));

	for (auto& d : aData)
		TEX_LOADER::ReleaseTexture(d.pImageData);

	//���[�J���s�񏉊���
	dx::XMStoreFloat4x4(&m_MtxLocal, dx::XMMatrixIdentity());

	//���[���h�s�񏉊���
	dx::XMStoreFloat4x4(&m_MtxWorld, dx::XMMatrixIdentity());

	//�}�e���A����񏉊���
	m_Material = MeshData.MaterialData;

	//���b�V����񏉊���
	if (MeshData.aNoSkinData.size() > 0)
		m_bNoBone = true;
}

VIEWER_MODEL::~VIEWER_MODEL() noexcept
{
}

//�X�V����
void VIEWER_MODEL::Update() noexcept
{
	//��]����
	if (m_Input.m_KB.GetPress(VK_A))
		m_Rot.y += gMath::GetRad(2);
	else if (m_Input.m_KB.GetPress(VK_D))
		m_Rot.y -= gMath::GetRad(2);
	if (m_Input.m_KB.GetPress(VK_W))
		m_Rot.x += gMath::GetRad(2);
	else if (m_Input.m_KB.GetPress(VK_S))
		m_Rot.x -= gMath::GetRad(2);
	if (m_Input.m_KB.GetPress(VK_R)) {
		m_Rot.x = 0.0f;
		m_Rot.y = 0.0f;
	}

	//���[���h�s��X�V
	dx::XMMATRIX mtx = dx::XMMatrixScaling(m_Scale, m_Scale, m_Scale)
		* dx::XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z);
	dx::XMStoreFloat4x4(&m_MtxWorld, mtx);

	//�����X�V
	if (m_bDrawAnimation)
		UpdateBoneData(m_AnimationID);
	else {

		//����񃊃Z�b�g
		for (auto& m : m_pMtxBone->mtxBone)
			dx::XMStoreFloat4x4(&m, dx::XMMatrixIdentity());

		//���[�J���s�񃊃Z�b�g
		dx::XMStoreFloat4x4(&m_MtxLocal, dx::XMMatrixIdentity());
	}
}

void VIEWER_MODEL::Draw(int InstanceNum) const noexcept
{
	m_ShaderMgr.Bind_Model();
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::SAMPLER);
	DRAWER::Draw(InstanceNum);
}

//���_���쐬
VS_DATA<VERTEX_MB> VIEWER_MODEL::MakeData_VS() const noexcept
{
	VS_DATA<VERTEX_M>& DataRef = m_Loader.GetMesh(m_MeshIndex).vsData;
	VS_DATA<VERTEX_MB> vsData;
	vsData.m_Indices = DataRef.m_Indices;		//�C���f�b�N�X
	for (auto& v : DataRef.m_Vertices) {		//���_���

		//���_���
		VERTEX_MB Vertex;
		Vertex.m_Pos = v.m_Pos;
		Vertex.m_Normal = v.m_Normal;
		Vertex.m_UV = v.m_UV;
		vsData.m_Vertices.emplace_back(Vertex);
	}

	//����d
	for (auto& idx : m_Loader.GetMesh(m_MeshIndex).aIndexBuffer) {	//���_�o�b�t�@�C���f�b�N�X�z��

		//�Ή����钸�_�֔��f
		size_t BoneNum = idx.aBoneID.size();	//���_���Ƃ̍��̐�
		for (auto& idxNum : idx.aIndex) {

			//�����e�����Ȃ��ꍇ
			if (BoneNum == 0) {
				vsData.m_Vertices[idxNum].m_BoneWeight[0] = 1.0f;
				continue;
			}

			//����d���i�[
			for (size_t k = 0; k < 4; k++) {	//4�ȏ�̍���j��
				if (k >= BoneNum)				//��O�����i����4�����̏ꍇ�j
					break;
				vsData.m_Vertices[idxNum].m_BoneID[k] = idx.aBoneID[k];
				vsData.m_Vertices[idxNum].m_BoneWeight[k] = idx.aWeight[k];
			}
		}
	}
	return VS_DATA<VERTEX_MB>(std::move(vsData));
}

//�����X�V
void VIEWER_MODEL::UpdateBoneData(int AnimID) noexcept
{
	//�Đ��t���[�����X�V
	auto pAnimData = &m_Loader.GetAnimation()[AnimID];
	if (!m_AnimPause) {
		if (!pAnimData->bIsFPS_30) {

			//60FPS
			m_AnimFrame++;

			//�t���[���J�E���g���Z�b�g
			if (m_FrameCnt > 0)
				m_FrameCnt = 0;
		}
		else {

			//30FPS
			m_FrameCnt++;
			if (m_FrameCnt > 1) {
				m_FrameCnt = 0;

				//60FPS�Ɠ�������
				m_AnimFrame++;
			}
		}
	}
	else
		m_AnimFrame = m_Viewer.GetAnimationFrame();
	if (m_AnimFrame < pAnimData->StartFrame)
		m_AnimFrame = pAnimData->StartFrame;
	if (m_AnimFrame >= pAnimData->StopFrame)
		m_AnimFrame = pAnimData->StartFrame;
	m_Viewer.GetAnimationFrame() = m_AnimFrame;			//�r���[�����֔��f

	//���s��X�V
	auto pMtxBone = &m_pMtxBone->mtxBone[0];
	auto pMtxBoneRef = &m_Loader.GetSkin()[AnimID].aFrameData[0];
	for (size_t i = 0, Cnt = m_Loader.GetSkin()[AnimID].aFrameData.size(); i < Cnt; i++) {
		if (pMtxBoneRef->aMatrix.size() > 0)
			*pMtxBone = pMtxBoneRef->aMatrix[m_AnimFrame];
		pMtxBone++;
		pMtxBoneRef++;
	}

	//���[�J���s��X�V�i���Ȃ����b�V���j
	if (m_bNoBone)
		m_MtxLocal = m_Loader.GetMesh(m_MeshIndex).aNoSkinData[AnimID].aMatrix[m_AnimFrame];
}
