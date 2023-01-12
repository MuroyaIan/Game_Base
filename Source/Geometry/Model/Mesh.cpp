
//===== �C���N���[�h�� =====
#include <Geometry/Model/Mesh.h>
#include <GraphicApp/Binder/BinderRef.h>
#include <Tool/Rand.h>

namespace dx = DirectX;

//===== �N���X���� =====
MESH::MESH(MODEL& ModelRef, int MeshIdx) :
	DRAWER(ModelRef.m_Gfx.m_DX), m_FileData(ModelRef.m_FileData), m_MeshIdx(MeshIdx),
	m_Gfx(ModelRef.m_Gfx), m_InstanceNum(ModelRef.m_InstanceNum), m_aInstanceData(ModelRef.m_aInstanceData), m_Material(),
	m_bStatic(ModelRef.m_bStatic), m_pLocalData(), m_AnimID(ModelRef.m_AnimID), m_AnimID_Backup(ModelRef.m_AnimID_Backup),
	m_AnimFrame(ModelRef.m_AnimFrame), m_AnimFrame_Backup(ModelRef.m_AnimFrame_Backup),
	m_bBlendAnim(ModelRef.m_bBlendAnim), m_BlendTimer(ModelRef.m_BlendTimer)
{
	//���b�V�����m�F
	ModelRef::MESH_PACK& Mesh = m_FileData.aMesh[m_MeshIdx];

	//���_���쐬
	VS_DATA<VERTEX_MB> Model = Mesh.vsData;
	dx::XMFLOAT4X4 mtx{};
	dx::XMStoreFloat4x4(&mtx, dx::XMMatrixScaling(1.0f, 1.0f, 1.0f));
	Model.SetVertexPos(mtx);
	AddBind(std::make_unique<VERTEX_BUFFER>(m_Gfx.m_DX, Model.m_Vertices, m_aInstanceData));

	//�C���f�b�N�X���쐬
	AddBind(std::make_unique<INDEX_BUFFER>(m_Gfx.m_DX, Model.m_Indices));

	//�}�e���A�����쐬
	m_Material = Mesh.MaterialData;
	AddBind(std::make_unique<CB_MATERIAL>(m_Gfx.m_DX, nullptr, m_Material));

	//�e�N�X�`���o�b�t�@�쐬
	std::vector<TEX_LOADER::TEX_DATA> aData(static_cast<int>(TEXTURE_MODEL::TEX_TYPE::MaxType));	//�o�b�t�@�p�z��
	TEX_LOADER::TEX_DATA NullImage = TEX_LOADER::LoadTexture("Asset/Texture/null.png");				//��摜
	std::string Tex_D = Mesh.Tex_D;																	//Diffuse�e�N�X�`����
	if (Tex_D.size() > 0) {
		for (auto& t : m_FileData.m_aTexPack) {
			if (t.Name == Tex_D) {
				aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Diffuse)] = t.TexData;
				break;
			}
		}
	}
	else
		aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Diffuse)] = NullImage;
	aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Specular)] = NullImage;
	aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Normal)] = NullImage;
	AddBind(std::make_unique<TEXTURE_MODEL>(m_Gfx.m_DX, aData));
	TEX_LOADER::ReleaseTexture(NullImage.pImageData);

	//���[�J�����쐬
	m_pLocalData = std::make_unique<CBD_MTX_LOCAL>();
	AddBind(std::make_unique<CB_LOCAL>(m_Gfx.m_DX, nullptr, *m_pLocalData));
}

MESH::~MESH() noexcept
{
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
	GetVertexBuffer().UpdateBuffer(m_Gfx.m_DX, m_aInstanceData, VERTEX_BUFFER::VB_TYPE::Instance);

	//�C���X�^���X���X�V
	SetInstanceNum(m_InstanceNum);

	return 0;
}
