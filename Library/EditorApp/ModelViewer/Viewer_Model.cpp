
//===== �C���N���[�h�� =====
#include <EditorApp/ModelViewer/Viewer_Model.h>
#include <GraphicApp/Binder/BinderRef.h>
#include <EditorApp/ModelViewer/Viewer.h>

namespace dx = DirectX;

//===== �N���X���� =====
VIEWER_MODEL::VIEWER_MODEL(GFX_PACK& Gfx, VIEWER& Viewer, FBX_LOADER& Loader, int MeshIndex) :
	DRAWER(Gfx.m_DX), m_Gfx(Gfx.m_DX), m_ShaderMgr(Gfx.m_ShaderMgr), m_Viewer(Viewer), m_Loader(Loader), m_MeshIndex(MeshIndex), m_MtxLocal(), m_MtxWorld(), m_Material(), m_bNoBone(false),
	m_pMtxBone(), m_bDrawAnimation(m_Viewer.GetFlag_DrawAnimation()), m_AnimationID(m_Viewer.GetAnimationID()), m_AnimFrame(0), m_FrameCnt(0), m_AnimPause(m_Viewer.GetFlag_AnimPause()),
	m_Scale(Viewer.GetModelScale()), m_RotY(Viewer.GetModelRotation()), pcbLight(), m_LightPos(Viewer.GetLightPos()), bUseNormalMap(false), bUseDispMap(false)
{
	//���_���쐬
	VS_DATA<VERTEX_MB> Model = MakeData_VS();
	AddBind(std::make_unique<VERTEX_BUFFER>(m_Gfx, Model.m_Vertices));

	//�C���f�b�N�X���쐬
	AddBind(std::make_unique<INDEX_BUFFER>(m_Gfx, Model.m_Indices));

	//VS�萔�o�b�t�@�쐬�i�ϊ��s��j
	CB_PTR cbData;
	AddBind(std::make_unique<CB_MTX_LWVP>(m_Gfx, &cbData, *this, m_MtxLocal));

	//VS�萔�o�b�t�@�쐬�i�����j
	m_pMtxBone = std::make_unique<CBD_BONE>();
	AddBind(std::make_unique<CB_BONE>(m_Gfx, &cbData, *m_pMtxBone, true));

	//VS�萔�o�b�t�@�쐬�i��������j
	pcbLight = std::make_unique<CONSTANT_BUFFER<dx::XMFLOAT4>>(m_Gfx, &cbData, true);

	//PS�萔�o�b�t�@�쐬�i�}�e���A���j
	AddBind(std::make_unique<CB_MATERIAL>(m_Gfx, &cbData, m_Material));

	//�萔�o�b�t�@Mgr�쐬
	AddBind(std::make_unique<CBUFF_MGR>(cbData));

	//�e�N�X�`���f�[�^�Ǎ�
	auto& MeshData = m_Loader.GetMesh(m_MeshIndex);
	std::vector<TEX_LOADER::TEX_DATA> aData(static_cast<int>(TEXTURE_MODEL::TEX_TYPE::MaxType));

	//Diffuse�}�b�v
	if (MeshData.aTex_Diffuse.size() > 0) {
		std::string Path = m_Loader.GetFilePath();
		Path += MeshData.aTex_Diffuse[0];
		aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Diffuse)] = TEX_LOADER::LoadTexture(Path.c_str());
	}
	else
		aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Diffuse)] = TEX_LOADER::LoadTexture("Asset/Texture/null.png");

	//Specular�}�b�v
	if (MeshData.aTex_Specular.size() > 0) {
		std::string Path = m_Loader.GetFilePath();
		Path += MeshData.aTex_Specular[0];
		aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Specular)] = TEX_LOADER::LoadTexture(Path.c_str());
	}
	else
		aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Specular)] = TEX_LOADER::LoadTexture("Asset/Texture/null.png");

	//Normal�}�b�v
	if (MeshData.aTex_Normal.size() > 0) {
		bUseNormalMap = true;
		std::string Path = m_Loader.GetFilePath();
		Path += MeshData.aTex_Normal[0];
		aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Normal)] = TEX_LOADER::LoadTexture(Path.c_str());
	}
	else
		aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Normal)] = TEX_LOADER::LoadTexture("Asset/Texture/null.png");

	//Displacement�}�b�v
	if (MeshData.aTex_Displacement.size() > 0) {
		bUseDispMap = true;
		std::string Path = m_Loader.GetFilePath();
		Path += MeshData.aTex_Displacement[0];
		aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Displacement)] = TEX_LOADER::LoadTexture(Path.c_str());
	}
	else
		aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Displacement)] = TEX_LOADER::LoadTexture("Asset/Texture/null.png");

	//�e�N�X�`���o�b�t�@�쐬
	AddBind(std::make_unique<TEXTURE_MODEL>(m_Gfx, aData));
	for (auto& d : aData)
		TEX_LOADER::ReleaseTexture(d.pImageData);	//�f�[�^���

	//���[�J���s�񏉊���
	dx::XMStoreFloat4x4(&m_MtxLocal, dx::XMMatrixIdentity());

	//���[���h�s�񏉊���
	dx::XMStoreFloat4x4(&m_MtxWorld, dx::XMMatrixIdentity());

	//�}�e���A����񏉊���
	m_Material = MeshData.MaterialData;

	//���̉e�����󂯂邩�m�F
	if (MeshData.aNoSkinData.size() > 0)
		m_bNoBone = true;
}

VIEWER_MODEL::~VIEWER_MODEL() noexcept
{
}

//�X�V����
void VIEWER_MODEL::Update() noexcept
{
	//���[���h�s��X�V
	dx::XMMATRIX mtx = dx::XMMatrixScaling(m_Scale, m_Scale, m_Scale)
		* dx::XMMatrixRotationRollPitchYaw(0.0f, m_RotY, 0.0f);
	dx::XMStoreFloat4x4(&m_MtxWorld, mtx);

	//�����X�V
	if (m_bDrawAnimation)
		UpdateBoneData(m_AnimationID);	//�A�j���[�V�����Đ����ˏ��X�V
	else {

		//����񃊃Z�b�g
		for (auto& m : m_pMtxBone->mtxBone)
			dx::XMStoreFloat4x4(&m, dx::XMMatrixIdentity());

		//���[�J���s�񃊃Z�b�g
		dx::XMStoreFloat4x4(&m_MtxLocal, dx::XMMatrixIdentity());
	}

	//�}�e���A���X�V
	if (m_Viewer.GetSpecularFlag())
		m_Material.Specular = m_Loader.GetMesh(m_MeshIndex).MaterialData.Specular;
	else
		m_Material.Specular = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_Material.Disp_MinLayerNum = m_Viewer.GetDispCoef().x;
	m_Material.Disp_MaxLayerNum = m_Viewer.GetDispCoef().y;
	m_Material.Disp_DepthScale = m_Viewer.GetDispCoef().z;										//�����}�b�s���O�̒����l���X�V
	m_Loader.GetMesh(m_MeshIndex).MaterialData.Disp_MinLayerNum = m_Material.Disp_MinLayerNum;
	m_Loader.GetMesh(m_MeshIndex).MaterialData.Disp_MaxLayerNum = m_Material.Disp_MaxLayerNum;
	m_Loader.GetMesh(m_MeshIndex).MaterialData.Disp_DepthScale = m_Material.Disp_DepthScale;	//�f�[�^�����f�����ɂ��X�V
}

//�`�揈��
void VIEWER_MODEL::Draw(int InstanceNum) const noexcept
{
	//PS�萔�o�b�t�@�쐬�i��������j
	dx::XMFLOAT4 Offset = { m_LightPos.x, m_LightPos.y, m_LightPos.z, 0.0f };
	pcbLight->Update(m_Gfx, Offset);

	//���̑��̃o�C���h
	if (bUseNormalMap &&
		m_Viewer.GetNormalMapFlag()) {
		m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::VS_MODEL_NORMAL);
		m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::IL_MODEL_NORMAL);

		//�����}�b�v����E�Ȃ�
		if (bUseDispMap &&
			m_Viewer.GetDispMapFlag())
			m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PS_MODEL_DISP);
		else
			m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PS_MODEL_NORMAL);
	}
	else {

		//�m�[�}���}�b�v�Ȃ�
		m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::VS_MODEL);
		m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::IL_MODEL);
		m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PS_MODEL);
	}
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PT_TRI);
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::SAMPLER);

	//�`��
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
		Vertex.m_UV = v.m_UV;
		Vertex.m_Normal = v.m_Normal;
		Vertex.m_Binormal = v.m_Binormal;
		Vertex.m_Tangent = v.m_Tangent;
		vsData.m_Vertices.push_back(std::move(Vertex));
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

			//�t���[���J�E���g����
			if (m_FrameCnt > 0)
				m_FrameCnt = 0;
		}
		else {

			//�t���[���J�E���g����
			m_FrameCnt++;
			if (m_FrameCnt > 1) {
				m_FrameCnt = 0;

				//30FPS
				m_AnimFrame++;
			}
		}
	}
	else
		m_AnimFrame = m_Viewer.GetAnimationFrame();		//�A�j���[�V�����ꎞ��~�˃t���[���ʒu��ێ�

	//�Đ��t���[�����͈̔͐���
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
