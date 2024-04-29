
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
	SRV_PTR SrvData;
	for (int i = 0, Cnt = static_cast<int>(TEXTURE_MODEL::TEX_TYPE::MaxType); i < Cnt; i++)
		SrvData.m_aSrvPtrPS.push_back(m_Gfx.m_TextureMgr.SetTextureOn(TEXTURE_MGR::TEX_ID::TEX_Null));	//��摜
	AddBind(std::make_unique<SRV_MGR>(SrvData));

	//�}�e���A����񏉊���
	m_Material.ms_Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_Material.ms_Specular = { 0.6f, 0.6f, 0.6f, 1.0f };
	m_Material.ms_Shininess = 30.0f;
}

SHAPE_MODEL::~SHAPE_MODEL() noexcept
{
	//�e�N�X�`���o�b�t�@���
	for (int i = 0, Cnt = static_cast<int>(TEXTURE_MODEL::TEX_TYPE::MaxType); i < Cnt; i++)
		m_Gfx.m_TextureMgr.SetTextureOff(TEXTURE_MGR::TEX_ID::TEX_Null);
}

//�X�V����
void SHAPE_MODEL::Update()
{
	//��O����
	if (m_InstanceNum < 1)
		return;

	//�C���X�^���X�X�V
	std::vector<VSD_INSTANCE> aInstData = m_aInstanceData;
	for (auto& i : aInstData)
		gMath::MtxTranspose4x4_SSE(&i.MtxWorld._11);
	GetVertexBuffer().UpdateBuffer(m_Gfx.m_DX, aInstData, VERTEX_BUFFER::VB_TYPE::Instance);
}

//�`�揈��
void SHAPE_MODEL::Draw(int InstanceNum) noexcept
{
	//��O����
	(void)InstanceNum;
	if (m_InstanceNum < 1)
		return;

	//�C���X�^���X�`��
	m_Gfx.m_ShaderMgr.Bind_Instance_Phong();
	DRAWER::Draw(m_InstanceNum);
}

//�C���X�^���X�ǉ�
int SHAPE_MODEL::AddInstance()
{
	//�z��ǉ�
	m_InstanceNum++;
	m_aInstanceData.push_back(VSD_INSTANCE{});

	//�C���X�^���X�o�b�t�@�Đݒ�
	GetVertexBuffer().ResetInstanceBuffer(m_Gfx.m_DX, m_aInstanceData);

	//�C���X�^���X���X�V
	SetInstanceNum(m_InstanceNum);

	return m_InstanceNum - 1;
}
