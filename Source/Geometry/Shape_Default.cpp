
//===== �C���N���[�h�� =====
#include <Geometry/Shape_Default.h>
#include <GraphicApp/Binder/BinderRef.h>

namespace dx = DirectX;

//===== �N���X���� =====
SHAPE_DEFAULT::SHAPE_DEFAULT(GFX_PACK& Gfx, VSD_MAKER::SHAPE Type) :
	DRAWER(Gfx.m_DX), m_Gfx(Gfx), m_Type(Type), m_InstanceNum(0), m_aMtxWorld(m_InstanceNum)
{
	//���_���쐬
	VS_DATA<VERTEX> Model = VSD_MAKER::MakeData_Default(m_Type);
	//dx::XMFLOAT4X4 mtx{};
	//dx::XMStoreFloat4x4(&mtx, dx::XMMatrixScaling(1.0f, 1.0f, 1.0f));
	//Model.SetVertexPos(mtx);
	AddBind(std::make_unique<VERTEX_BUFFER>(m_Gfx.m_DX, Model.m_Vertices, m_aMtxWorld));

	//�C���f�b�N�X���쐬
	AddBind(std::make_unique<INDEX_BUFFER>(m_Gfx.m_DX, Model.m_Indices));

	//VS�萔�o�b�t�@�쐬�i�J�����j
	CB_PTR cbData;
	m_Gfx.m_ShaderMgr.SetConstBufferPtr(SHADER_MGR::BINDER_ID::CB_VS_MtxVP, &cbData);

	//PS�萔�o�b�t�@�쐬�i�|���S���F�j
	m_Gfx.m_ShaderMgr.SetConstBufferPtr(SHADER_MGR::BINDER_ID::CB_PS_Default, &cbData);

	//�萔�o�b�t�@Mgr�쐬
	AddBind(std::make_unique<CBUFF_MGR>(cbData));
}

SHAPE_DEFAULT::~SHAPE_DEFAULT() noexcept
{
}

//�X�V����
void SHAPE_DEFAULT::Update() noexcept
{
}

//�����ݏ���
void SHAPE_DEFAULT::Draw(int InstanceNum) const noexcept
{
	//��O����
	(void)InstanceNum;
	if (m_InstanceNum < 1)
		return;

	//�C���X�^���X�X�V
	std::vector<dx::XMFLOAT4X4> aMtxWorld = m_aMtxWorld;
	for (auto& i : aMtxWorld)
		gMath::MtxTranspose4x4_SSE(&i._11);
	GetVertexBuffer().UpdateBuffer(m_Gfx.m_DX, aMtxWorld, VERTEX_BUFFER::VB_TYPE::Instance);

	//�C���X�^���X�`��
	m_Gfx.m_ShaderMgr.Bind_Instance();
	DRAWER::Draw(m_InstanceNum);
}

//�C���X�^���X�ǉ�
int SHAPE_DEFAULT::AddInstance()
{
	//�z��ǉ�
	m_InstanceNum++;
	dx::XMFLOAT4X4 mtx{};
	dx::XMStoreFloat4x4(&mtx, dx::XMMatrixIdentity());
	m_aMtxWorld.push_back(std::move(mtx));

	//�C���X�^���X�o�b�t�@�Đݒ�
	GetVertexBuffer().ResetInstanceBuffer(m_Gfx.m_DX, m_aMtxWorld);

	//�C���X�^���X���X�V
	SetInstanceNum(m_InstanceNum);

	return m_InstanceNum - 1;
}
