
//===== �C���N���[�h�� =====
#include <Geometry/Shape_Tex.h>
#include <GraphicApp/Binder/BinderRef.h>

#include <Tool/Rand.h>

namespace dx = DirectX;

//===== �N���X���� =====
SHAPE_TEX::SHAPE_TEX(GFX_PACK& Gfx, VSD_MAKER::SHAPE Type, TEXTURE_MGR::TEX_ID Tex) :
	DRAWER(Gfx.m_DX), m_Gfx(Gfx), m_Type(Type), m_InstanceNum(0), m_aMtxWorld(m_InstanceNum), m_Tex(Tex), m_aMtxData(m_InstanceNum)
{
	//���_���쐬
	VS_DATA<VERTEX_T> Model = VSD_MAKER::MakeData_Tex(m_Type);
	dx::XMFLOAT4X4 mtx{};
	dx::XMStoreFloat4x4(&mtx, dx::XMMatrixScaling(1.0f, 1.0f, 1.0f));
	Model.SetVertexPos(mtx);
	AddBind(std::make_unique<VERTEX_BUFFER>(m_Gfx.m_DX, Model.m_Vertices, m_aMtxWorld));

	//�C���f�b�N�X���쐬
	AddBind(std::make_unique<INDEX_BUFFER>(m_Gfx.m_DX, Model.m_Indices));

	//�e�N�X�`��������
	m_Gfx.m_TextureMgr.SetTextureOn(m_Tex);

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

SHAPE_TEX::~SHAPE_TEX() noexcept
{
	//�e�N�X�`���I��
	m_Gfx.m_TextureMgr.SetTextureOff(m_Tex);
}

//�X�V����
void SHAPE_TEX::Update() noexcept
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
		dx::XMStoreFloat4x4(&m_aMtxWorld[i], mtx);
	}
}

//�����ݏ���
void SHAPE_TEX::Draw(int InstanceNum) const noexcept
{
	//��O����
	(void)InstanceNum;
	if (m_InstanceNum < 1)
		return;

	//�C���X�^���X�X�V
	std::vector<DirectX::XMFLOAT4X4> aMtxWorld = m_aMtxWorld;
	for (auto& i : aMtxWorld)
		gMath::MtxTranspose4x4_SSE(&i._11);
	GetVertexBuffer().UpdateBuffer(m_Gfx.m_DX, aMtxWorld, VERTEX_BUFFER::VB_TYPE::INSTANCE);

	//�C���X�^���X�`��
	m_Gfx.m_TextureMgr.Bind(m_Tex);
	m_Gfx.m_ShaderMgr.Bind_Instance_Texture();
	DRAWER::Draw(m_InstanceNum);
}

//�C���X�^���X�ǉ�
int SHAPE_TEX::AddInstance()
{
	//�z��ǉ�
	m_InstanceNum++;
	DirectX::XMFLOAT4X4 mtx{};
	dx::XMStoreFloat4x4(&mtx, dx::XMMatrixIdentity());
	m_aMtxWorld.push_back(mtx);
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
	GetVertexBuffer().ResetInstanceBuffer(m_Gfx.m_DX, m_aMtxWorld);
	GetVertexBuffer().UpdateBuffer(m_Gfx.m_DX, m_aMtxWorld, VERTEX_BUFFER::VB_TYPE::INSTANCE);

	//�C���X�^���X���X�V
	SetInstanceNum(m_InstanceNum);

	return m_InstanceNum - 1;
}
