
//===== �C���N���[�h�� =====
#include <Geometry/Debug/Mesh_Light.h>
#include <GraphicApp/Binder/BinderRef.h>
#include <GraphicApp/Drawer/vsdSphere.h>

namespace dx = DirectX;

//===== �N���X���� =====
MESH_LIGHT::MESH_LIGHT(GFX_PACK& Gfx) :
	DRAWER(Gfx.m_DX), m_Gfx(Gfx), m_InstanceNum(0), m_aMtxWorld(m_InstanceNum), m_aMtxData(m_InstanceNum)
{
	//���_���쐬
	VS_DATA<VERTEX> Model = std::move(VSD_SPHERE::MakeData<VERTEX>());
	AddBind(std::make_unique<VERTEX_BUFFER>(m_Gfx.m_DX, Model.m_Vertices, m_aMtxWorld));

	//�C���f�b�N�X���쐬
	AddBind(std::make_unique<INDEX_BUFFER>(m_Gfx.m_DX, Model.m_Indices));

	//VS�萔�o�b�t�@�쐬�i�J�����j
	CB_PTR cbData;
	dynamic_cast<CB_MTX_VP*>(m_Gfx.m_ShaderMgr.GetBinder(SHADER_MGR::BINDER_ID::CB_VS_MTX_VP))->SetBuffPtr(&cbData);

	//PS�萔�o�b�t�@�쐬�i�|���S���F�j
	const dx::XMFLOAT4 cbColor(1.0f, 1.0f, 0.0f, 0.0f);
	AddBind(std::make_unique<CONSTANT_BUFFER<dx::XMFLOAT4>>(m_Gfx.m_DX, cbColor, &cbData, false, true));

	//�萔�o�b�t�@Mgr�쐬
	AddBind(std::make_unique<CBUFF_MGR>(cbData));
}

MESH_LIGHT::~MESH_LIGHT() noexcept
{
}

//�X�V����
void MESH_LIGHT::Update() noexcept
{
	//���[���h�s��X�V
	for (int i = 0; i < m_InstanceNum; i++){
		dx::XMMATRIX mtx = dx::XMMatrixScaling(m_aMtxData[i].m_Scale.x, m_aMtxData[i].m_Scale.y, m_aMtxData[i].m_Scale.z)
			* dx::XMMatrixTranslation(m_aMtxData[i].m_Pos.x, m_aMtxData[i].m_Pos.y, m_aMtxData[i].m_Pos.z);
		dx::XMStoreFloat4x4(&m_aMtxWorld[i], mtx);
	}
}

//�����ݏ���
void MESH_LIGHT::Draw(int InstanceNum) const noexcept
{
	//��O����
	(void)InstanceNum;
	if (m_InstanceNum < 1)
		return;

	//�C���X�^���X�X�V
	std::vector<DirectX::XMFLOAT4X4> aMtxWorld = m_aMtxWorld;
	for (auto& i : aMtxWorld)
		gMath::MtxTranspose4x4_SSE(&i._11);
	GetVertexBuffer().UpdateBuffer(m_Gfx.m_DX, aMtxWorld, VERTEX_BUFFER::VB_TYPE::Instance);

	//�C���X�^���X�`��
	m_Gfx.m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::VS_INSTANCE);
	m_Gfx.m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::IL_INSTANCE);
	m_Gfx.m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PT_TRI);
	m_Gfx.m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PS_ONE_COLOR);
	DRAWER::Draw(m_InstanceNum);
}

//�C���X�^���X�ǉ�
int MESH_LIGHT::AddInstance()
{
	//�z��ǉ�
	m_InstanceNum++;
	DirectX::XMFLOAT4X4 mtx{};
	dx::XMStoreFloat4x4(&mtx, dx::XMMatrixIdentity());
	m_aMtxWorld.push_back(mtx);
	m_aMtxData.push_back(MTX_DATA());

	//�C���X�^���X�o�b�t�@�Đݒ�
	GetVertexBuffer().ResetInstanceBuffer(m_Gfx.m_DX, m_aMtxWorld);
	GetVertexBuffer().UpdateBuffer(m_Gfx.m_DX, m_aMtxWorld, VERTEX_BUFFER::VB_TYPE::Instance);

	//�C���X�^���X���X�V
	SetInstanceNum(m_InstanceNum);

	return m_InstanceNum - 1;
}
