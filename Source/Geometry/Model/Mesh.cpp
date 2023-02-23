
//===== �C���N���[�h�� =====
#include <Geometry/Model/Mesh.h>
#include <GraphicApp/Binder/BinderRef.h>
#include <Light/LightMgr.h>
#include <Tool/Rand.h>

namespace dx = DirectX;

//===== �N���X���� =====
MESH::MESH(MODEL& ModelRef, int MeshIdx) :
	DRAWER(ModelRef.m_Gfx.m_DX), m_Gfx(ModelRef.m_Gfx),
	m_InstanceNum(ModelRef.m_InstanceNum), m_aInstanceData(ModelRef.m_aInstanceData), m_Material(),
	m_ModelRef(ModelRef), m_FileData(ModelRef.m_FileData), m_MeshIdx(MeshIdx), m_bStatic(ModelRef.m_bStatic),
	m_pLocalData(), m_AnimID(ModelRef.m_AnimID), m_AnimFrame(ModelRef.m_AnimFrame)
{
	//���b�V�����m�F
	ModelRef::MESH_PACK& Mesh = m_FileData.aMesh[m_MeshIdx];

	//���_���쐬
	VS_DATA<VERTEX_MNB> vsdModel = Mesh.vsData;
	VS_DATA<VERTEX_MB> Model;
	Model.m_Indices = vsdModel.m_Indices;
	Model.m_Vertices.resize(Model.m_Indices.size());
	for (size_t i = 0, Cnt = Model.m_Vertices.size(); i < Cnt; i++) {
		Model.m_Vertices[i].m_Pos = vsdModel.m_Vertices[i].m_Pos;
		Model.m_Vertices[i].m_UV = vsdModel.m_Vertices[i].m_UV;
		Model.m_Vertices[i].m_Normal = vsdModel.m_Vertices[i].m_Normal;
		for (int j = 0; j < 4; j++) {
			Model.m_Vertices[i].m_BoneID[j] = vsdModel.m_Vertices[i].m_BoneID[j];
			Model.m_Vertices[i].m_BoneWeight[j] = vsdModel.m_Vertices[i].m_BoneWeight[j];
		}
	}
	AddBind(std::make_unique<VERTEX_BUFFER>(m_Gfx.m_DX, Model.m_Vertices, m_aInstanceData));

	//�C���f�b�N�X���쐬
	AddBind(std::make_unique<INDEX_BUFFER>(m_Gfx.m_DX, Model.m_Indices));

	//VS�萔�o�b�t�@�쐬�i�J�����j
	CB_PTR cbData;
	m_Gfx.m_ShaderMgr.SetConstBufferPtr(SHADER_MGR::BINDER_ID::CB_VS_MtxVP, &cbData);

	//PS�萔�o�b�t�@�쐬�i�}�e���A���j
	m_Material = Mesh.MaterialData;
	AddBind(std::make_unique<CB_MATERIAL>(m_Gfx.m_DX, &cbData, m_Material));

	//VS�EPS�萔�o�b�t�@�쐬�i���C�g�j
	m_Gfx.m_ShaderMgr.SetConstBufferPtr(SHADER_MGR::BINDER_ID::CB_Light, &cbData);

	//VS�萔�o�b�t�@�쐬(�����)
	if (!m_bStatic)
		dynamic_cast<CB_BONE*>(ModelRef.m_pBoneBuffer.get())->SetBuffPtr(&cbData);

	//VS�萔�o�b�t�@�쐬(���[�J�����)
	if (!m_bStatic) {
		m_pLocalData = std::make_unique<CBD_MTX_LOCAL>();
		AddBind(std::make_unique<CB_LOCAL>(m_Gfx.m_DX, &cbData, *m_pLocalData));
	}

	//�萔�o�b�t�@Mgr�쐬
	AddBind(std::make_unique<CBUFF_MGR>(cbData));

	//�e�N�X�`���o�b�t�@�쐬
	SRV_PTR SrvData;
	if (Mesh.Tex_D.size() > 0)
		SrvData.m_aSrvPtrPS.push_back(m_Gfx.m_ModelMgr.SetTextureOn(m_ModelRef.m_ID, Mesh.Tex_D));		//Diffuse�e�N�X�`��
	else
		SrvData.m_aSrvPtrPS.push_back(m_Gfx.m_TextureMgr.SetTextureOn(TEXTURE_MGR::TEX_ID::TEX_Null));	//��摜
	SrvData.m_aSrvPtrPS.push_back(m_Gfx.m_TextureMgr.SetTextureOn(TEXTURE_MGR::TEX_ID::TEX_Null));
	SrvData.m_aSrvPtrPS.push_back(m_Gfx.m_TextureMgr.SetTextureOn(TEXTURE_MGR::TEX_ID::TEX_Null));
	SrvData.m_aSrvPtrPS.push_back(m_Gfx.m_TextureMgr.SetTextureOn(TEXTURE_MGR::TEX_ID::TEX_Null));
	AddBind(std::make_unique<SRV_MGR>(SrvData));
}

MESH::~MESH() noexcept
{
	//�e�N�X�`���o�b�t�@���
	ModelRef::MESH_PACK& Mesh = m_FileData.aMesh[m_MeshIdx];
	if (Mesh.Tex_D.size() > 0)
		m_Gfx.m_ModelMgr.SetTextureOff(m_ModelRef.m_ID, Mesh.Tex_D);
	else
		m_Gfx.m_TextureMgr.SetTextureOff(TEXTURE_MGR::TEX_ID::TEX_Null);
	m_Gfx.m_TextureMgr.SetTextureOff(TEXTURE_MGR::TEX_ID::TEX_Null);
	m_Gfx.m_TextureMgr.SetTextureOff(TEXTURE_MGR::TEX_ID::TEX_Null);
	m_Gfx.m_TextureMgr.SetTextureOff(TEXTURE_MGR::TEX_ID::TEX_Null);
}

//�X�V����
void MESH::Update() noexcept
{
	//��O����
	if (m_InstanceNum < 1)
		return;

	//���[�J�����X�V
	if (!m_bStatic) {
		if (m_FileData.aMesh[m_MeshIdx].aNoSkin.size() > 0)
			m_pLocalData->mtxSkin = m_FileData.aMesh[m_MeshIdx].aNoSkin[m_AnimID].aMatrix[m_AnimFrame];
	}
}

//�����ݏ���
void MESH::Draw(int InstanceNum) const noexcept
{
	//��O����
	(void)InstanceNum;
	if (m_InstanceNum < 1)
		return;

	//�C���X�^���X�X�V
	std::vector<VSD_INSTANCE> aInstData = m_aInstanceData;
	for (auto& i : aInstData)
		gMath::MtxTranspose4x4_SSE(&i.MtxWorld._11);
	GetVertexBuffer().UpdateBuffer(m_Gfx.m_DX, aInstData, VERTEX_BUFFER::VB_TYPE::Instance);

	//�C���X�^���X�`��
	if (m_bStatic)
		m_Gfx.m_ShaderMgr.Bind_Instance_Phong();
	else
		m_Gfx.m_ShaderMgr.Bind_Instance_Phong_Anim();
	DRAWER::Draw(m_InstanceNum);
}

//�C���X�^���X�ǉ�
int MESH::AddInstance()
{
	//�C���X�^���X�o�b�t�@�Đݒ�
	GetVertexBuffer().ResetInstanceBuffer(m_Gfx.m_DX, m_aInstanceData);

	//�C���X�^���X���X�V
	SetInstanceNum(m_InstanceNum);

	return 0;
}
