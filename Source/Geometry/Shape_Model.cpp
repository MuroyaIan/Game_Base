
//===== �C���N���[�h�� =====
#include <Geometry/Shape_Model.h>
#include <GraphicApp/Binder/BinderRef.h>

#include <Tool/Rand.h>

namespace dx = DirectX;

//===== �N���X���� =====
SHAPE_MODEL::SHAPE_MODEL(GFX_PACK& Gfx, VSD_MAKER::SHAPE Type) :
	DRAWER(Gfx.m_DX), m_Gfx(Gfx), m_Type(Type), m_InstanceNum(0), m_aInstanceData(m_InstanceNum), m_Material(), m_aMtxData(m_InstanceNum)
{
	//���_���쐬
	VS_DATA<VERTEX_M> Model = VSD_MAKER::MakeData_Model(m_Type);
	dx::XMFLOAT4X4 mtx{};
	dx::XMStoreFloat4x4(&mtx, dx::XMMatrixScaling(1.0f, 1.0f, 1.0f));
	Model.SetVertexPos(mtx);
	for (auto& v : Model.m_Vertices) {	//���W�n�ϊ�
		v.m_Pos.x *= -1.0f;
		v.m_Normal.x *= -1.0f;
	}
	AddBind(std::make_unique<VERTEX_BUFFER>(m_Gfx.m_DX, Model.m_Vertices, m_aInstanceData));

	//�C���f�b�N�X���쐬
	AddBind(std::make_unique<INDEX_BUFFER>(m_Gfx.m_DX, Model.m_Indices));

	//�萔�o�b�t�@�쐬�i�}�e���A���j
	AddBind(std::make_unique<CB_MATERIAL>(m_Gfx.m_DX, nullptr, m_Material));

	//�e�N�X�`���o�b�t�@�쐬
	std::vector<TEX_LOADER::TEX_DATA> aData(static_cast<int>(TEXTURE_MODEL::TEX_TYPE::MaxType));
	//aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Diffuse)] = TEX_LOADER::LoadTexture("Asset/Texture/test2.png");
	aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Diffuse)] = TEX_LOADER::LoadTexture("Asset/Texture/null.png");
	aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Specular)] = TEX_LOADER::LoadTexture("Asset/Texture/null.png");
	aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Normal)] = TEX_LOADER::LoadTexture("Asset/Texture/null.png");
	AddBind(std::make_unique<TEXTURE_MODEL>(m_Gfx.m_DX, aData));
	for (auto& d : aData)
		TEX_LOADER::ReleaseTexture(d.pImageData);

	//�}�e���A����񏉊���
	m_Material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_Material.Specular = { 0.6f, 0.6f, 0.6f, 1.0f };
	m_Material.Shininess = 30.0f;

	//�s���񏉊���
	for (auto& md : m_aMtxData) {
		md.dt = 0.002f;
		md.m_r = RAND_MAKER::MakeRand_Float(6.0f, 20.0f);
		md.m_roll = 0.0f;
		md.m_pitch = 0.0f;
		md.m_yaw = 0.0f;
		md.m_theta	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 2.0f);
		md.m_phi	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 2.0f);
		md.m_chi	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 2.0f);
		md.m_droll	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 2.0f);
		md.m_dpitch	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 2.0f);
		md.m_dyaw	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 2.0f);
		md.m_dtheta	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 1.0f);
		md.m_dphi	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 1.0f);
		md.m_dchi	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 1.0f);
	}
}

SHAPE_MODEL::~SHAPE_MODEL() noexcept
{
}

//�X�V����
void SHAPE_MODEL::Update() noexcept
{
	//���[���h�s��X�V
	for (int i = 0; i < m_InstanceNum; i++) {

		//���]�X�V
		m_aMtxData[i].m_roll += m_aMtxData[i].m_droll * m_aMtxData[i].dt;
		m_aMtxData[i].m_pitch += m_aMtxData[i].m_dpitch * m_aMtxData[i].dt;
		m_aMtxData[i].m_yaw += m_aMtxData[i].m_dyaw * m_aMtxData[i].dt;

		//���]�X�V
		m_aMtxData[i].m_theta += m_aMtxData[i].m_dtheta * m_aMtxData[i].dt;
		m_aMtxData[i].m_phi += m_aMtxData[i].m_dphi * m_aMtxData[i].dt;
		m_aMtxData[i].m_chi += m_aMtxData[i].m_dchi * m_aMtxData[i].dt;

		//�s��X�V
		dx::XMMATRIX mtx = dx::XMMatrixRotationRollPitchYaw(m_aMtxData[i].m_roll, m_aMtxData[i].m_pitch, m_aMtxData[i].m_yaw)	//���]
			* dx::XMMatrixTranslation(m_aMtxData[i].m_r, 0.0f, 0.0f)															//���]���a�X�V
			* dx::XMMatrixRotationRollPitchYaw(m_aMtxData[i].m_theta, m_aMtxData[i].m_phi, m_aMtxData[i].m_chi);				//���]
		dx::XMStoreFloat4x4(&m_aInstanceData[i].MtxWorld, mtx);
	}
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
	m_aInstanceData.push_back(InstData);
	MTX_DATA md{};
	md.dt = 0.002f;
	md.m_r = RAND_MAKER::MakeRand_Float(6.0f, 20.0f);
	md.m_roll = 0.0f;
	md.m_pitch = 0.0f;
	md.m_yaw = 0.0f;
	md.m_theta	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 2.0f);
	md.m_phi	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 2.0f);
	md.m_chi	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 2.0f);
	md.m_droll	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 2.0f);
	md.m_dpitch	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 2.0f);
	md.m_dyaw	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 2.0f);
	md.m_dtheta	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 1.0f);
	md.m_dphi	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 1.0f);
	md.m_dchi	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 1.0f);
	m_aMtxData.push_back(md);

	//�C���X�^���X�o�b�t�@�Đݒ�
	GetVertexBuffer().ResetInstanceBuffer(m_Gfx.m_DX, m_aInstanceData);
	GetVertexBuffer().UpdateBuffer(m_Gfx.m_DX, m_aInstanceData, VERTEX_BUFFER::VB_TYPE::Instance);

	//�C���X�^���X���X�V
	SetInstanceNum(m_InstanceNum);

	return m_InstanceNum - 1;
}
