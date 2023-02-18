
//===== �C���N���[�h�� =====
#include <Geometry/Base/Shape_Model.h>
#include <GraphicApp/Binder/BinderRef.h>
#include <Light/LightMgr.h>
#include <Tool/Rand.h>

namespace dx = DirectX;

//===== �N���X���� =====
SHAPE_MODEL::SHAPE_MODEL(GFX_PACK& Gfx, VSD_MAKER::SHAPE Type) :
	DRAWER(Gfx.m_DX), m_Gfx(Gfx), m_Type(Type), m_InstanceNum(0), m_aInstanceData(m_InstanceNum), m_Material()
{
	//���_���쐬
	VS_DATA<VERTEX_M> Model = VSD_MAKER::MakeData_Model(m_Type);
	for (auto& v : Model.m_Vertices) {	//���W�n�ϊ��i�E��n�ցj
		v.m_Pos.x *= -1.0f;
		v.m_Normal.x *= -1.0f;
	}
	AddBind(std::make_unique<VERTEX_BUFFER>(m_Gfx.m_DX, Model.m_Vertices, m_aInstanceData));

	//�C���f�b�N�X���쐬
	AddBind(std::make_unique<INDEX_BUFFER>(m_Gfx.m_DX, Model.m_Indices));

	//VS�萔�o�b�t�@�쐬�i�J�����j
	CB_PTR cbData;
	m_Gfx.m_ShaderMgr.SetConstBufferPtr(SHADER_MGR::BINDER_ID::CB_VS_MtxVP, &cbData);

	//PS�萔�o�b�t�@�쐬�i�}�e���A���j
	AddBind(std::make_unique<CB_MATERIAL>(m_Gfx.m_DX, &cbData, m_Material));

	//VS�EPS�萔�o�b�t�@�쐬�i���C�g�j
	m_Gfx.m_ShaderMgr.SetConstBufferPtr(SHADER_MGR::BINDER_ID::CB_Light, &cbData);

	//�萔�o�b�t�@Mgr�쐬
	AddBind(std::make_unique<CBUFF_MGR>(cbData));

	//�e�N�X�`���o�b�t�@�쐬
	std::vector<TEX_LOADER::TEX_DATA> aData(static_cast<int>(TEXTURE_MODEL::TEX_TYPE::MaxType));
	TEX_LOADER::TEX_DATA& NullImage = m_Gfx.m_TextureMgr.GetTexPack(TEXTURE_MGR::TEX_ID::TEX_Null).TexData;		//��摜
	aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Diffuse)] = NullImage;
	aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Specular)] = NullImage;
	aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Normal)] = NullImage;
	aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Displacement)] = NullImage;
	AddBind(std::make_unique<TEXTURE_MODEL>(m_Gfx.m_DX, aData));
	for (auto& d : aData)
		d.pImageData = nullptr;

	//�}�e���A����񏉊���
	m_Material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_Material.Specular = { 0.6f, 0.6f, 0.6f, 1.0f };
	m_Material.Shininess = 30.0f;
}

SHAPE_MODEL::~SHAPE_MODEL() noexcept
{
}

//�X�V����
void SHAPE_MODEL::Update() noexcept
{
}

//�����ݏ���
void SHAPE_MODEL::Draw(int InstanceNum) const noexcept
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
	m_Gfx.m_ShaderMgr.Bind_Instance_Phong();
	DRAWER::Draw(m_InstanceNum);
}

//�C���X�^���X�ǉ�
int SHAPE_MODEL::AddInstance()
{
	//�z��ǉ�
	m_InstanceNum++;
	VSD_INSTANCE InstData{};
	//InstData.MaterialColor = {
	//	RAND_MAKER::MakeRand_Float(0.0f, 1.0f),
	//	RAND_MAKER::MakeRand_Float(0.0f, 1.0f),
	//	RAND_MAKER::MakeRand_Float(0.0f, 1.0f),
	//	1.0f
	//};
	m_aInstanceData.push_back(std::move(InstData));

	//�C���X�^���X�o�b�t�@�Đݒ�
	GetVertexBuffer().ResetInstanceBuffer(m_Gfx.m_DX, m_aInstanceData);

	//�C���X�^���X���X�V
	SetInstanceNum(m_InstanceNum);

	return m_InstanceNum - 1;
}
