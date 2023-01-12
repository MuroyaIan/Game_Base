
//===== �C���N���[�h�� =====
#include <EditorApp/ModelViewer/Bone.h>
#include <GraphicApp/Binder/BinderRef.h>
#include <EditorApp/ModelViewer/Viewer.h>

#include <Tool/Rand.h>

namespace dx = DirectX;

//===== �N���X���� =====
BONE::BONE(GFX_PACK& Gfx, VIEWER& Viewer, FBX_LOADER& Loader, INPUT_MGR& Input) :
	DRAWER(Gfx.m_DX), m_Gfx(Gfx), m_InstanceNum(0), m_Loader(Loader),
	m_aMtxLocal(m_InstanceNum), m_bDrawAnimation(Viewer.GetFlag_DrawAnimation()), m_AnimationID(Viewer.GetAnimationID()), m_AnimFrame(Viewer.GetAnimationFrame()), m_Scale(1.0f),
	m_MtxLocal(), m_MtxWorld(), m_ModelScale(Viewer.GetModelScale()),
	m_Input(Input), m_Rot()
{
	//���_���쐬
	VS_DATA<VERTEX> Model = VSD_MAKER::MakeData_Default(VSD_MAKER::SHAPE::Pyramid);
	VS_DATA<VERTEX_C> Model_C;
	Model_C.m_Indices = Model.m_Indices;
	Model_C.m_Vertices.resize(Model.m_Vertices.size());
	auto pVtxTo = &Model_C.m_Vertices[0];
	auto pVtxFrom = &Model.m_Vertices[0];
	for (size_t i = 0, Cnt = Model.m_Vertices.size(); i < Cnt; i++) {
		pVtxTo->m_Pos = pVtxFrom->m_Pos;
		pVtxTo++;
		pVtxFrom++;
	}
	Model_C.m_Vertices[0].m_Color = VTX_COLOR(255, 127, 127, 255);
	Model_C.m_Vertices[1].m_Color = VTX_COLOR(255, 127, 127, 255);
	Model_C.m_Vertices[2].m_Color = VTX_COLOR(255, 127, 127, 255);
	Model_C.m_Vertices[3].m_Color = VTX_COLOR(255, 127, 127, 255);
	Model_C.m_Vertices[4].m_Color = VTX_COLOR(255,   0, 255, 255);
	Model_C.m_Vertices[5].m_Color = VTX_COLOR(255, 255,   0, 255);
	dx::XMFLOAT4X4 mtx{};
	dx::XMStoreFloat4x4(&mtx, dx::XMMatrixScaling(1.0f, 1.0f, 1.0f) * dx::XMMatrixRotationRollPitchYaw(gMath::GetRad(90), 0.0f, 0.0f));	//Z�����O����
	Model_C.SetVertexPos(mtx);
	AddBind(std::make_unique<VERTEX_BUFFER>(m_Gfx.m_DX, Model_C.m_Vertices, m_aMtxLocal));

	//�C���f�b�N�X���쐬
	AddBind(std::make_unique<INDEX_BUFFER>(m_Gfx.m_DX, Model_C.m_Indices));

	//VS�萔�o�b�t�@�쐬�i�ϊ��s��j
	CB_PTR cbData;
	AddBind(std::make_unique<CB_MTX_LWVP>(m_Gfx.m_DX, &cbData, *this, m_MtxLocal));

	//�萔�o�b�t�@Mgr�쐬
	AddBind(std::make_unique<CBUFF_MGR>(cbData));

	//���[�J���s�񏉊���
	dx::XMStoreFloat4x4(&m_MtxLocal, dx::XMMatrixIdentity());

	//���[���h�s�񏉊���
	dx::XMStoreFloat4x4(&m_MtxWorld, dx::XMMatrixIdentity());
}

BONE::~BONE() noexcept
{
}

//�X�V����
void BONE::Update() noexcept
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
	dx::XMStoreFloat4x4(&m_MtxWorld, dx::XMMatrixScaling(m_ModelScale, m_ModelScale, m_ModelScale)
		* dx::XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z));

	//���[�J���s��X�V
	if (m_InstanceNum == 0)
		return;
	auto pMtxLocal = &m_aMtxLocal[0];
	auto pMtxInit = &m_Loader.GetSkeleton()[0];
	for (size_t i = 0, Cnt = m_InstanceNum; i < Cnt; i++) {
		if (!m_bDrawAnimation) {
			dx::XMMATRIX mtxL = dx::XMMatrixScaling(m_Scale, m_Scale, m_Scale) * dx::XMLoadFloat4x4(&pMtxInit->InitMatrix);
			dx::XMStoreFloat4x4(pMtxLocal, mtxL);
		}
		else {
			dx::XMMATRIX mtxL{};
			if (m_Loader.GetSkin()[m_AnimationID].aFrameData[i].aBoneMatrix.size() > 0)
				mtxL = dx::XMMatrixScaling(m_Scale, m_Scale, m_Scale) * dx::XMLoadFloat4x4(&m_Loader.GetSkin()[m_AnimationID].aFrameData[i].aBoneMatrix[m_AnimFrame]);
			else
				mtxL = dx::XMMatrixScaling(m_Scale, m_Scale, m_Scale);
			dx::XMStoreFloat4x4(pMtxLocal, mtxL);
		}
		pMtxLocal++;
		pMtxInit++;
	}
}

//�����ݏ���
void BONE::Draw(int InstanceNum) const noexcept
{
	//��O����
	(void)InstanceNum;
	if (m_InstanceNum < 1)
		return;

	//�C���X�^���X�X�V
	std::vector<DirectX::XMFLOAT4X4> aMtxLocal = m_aMtxLocal;
	for (auto& i : aMtxLocal)
		gMath::MtxTranspose4x4_SSE(&i._11);
	GetVertexBuffer().UpdateBuffer(m_Gfx.m_DX, aMtxLocal, VERTEX_BUFFER::VB_TYPE::Instance);

	//�C���X�^���X�`��
	m_Gfx.m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::VS_MODEL_INSTANCE_VTX_BLEND);
	m_Gfx.m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::IL_MODEL_INSTANCE_VTX_BLEND);
	m_Gfx.m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PT_TRI);
	m_Gfx.m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PS_VTX_BLEND);
	DRAWER::Draw(m_InstanceNum);
}

//�C���X�^���X�ǉ�
int BONE::AddInstance()
{
	//�z��ǉ�
	m_InstanceNum++;
	DirectX::XMFLOAT4X4 mtx{};
	dx::XMStoreFloat4x4(&mtx, dx::XMMatrixIdentity());
	m_aMtxLocal.emplace_back(mtx);

	//�C���X�^���X�o�b�t�@�Đݒ�
	GetVertexBuffer().ResetInstanceBuffer(m_Gfx.m_DX, m_aMtxLocal);
	GetVertexBuffer().UpdateBuffer(m_Gfx.m_DX, m_aMtxLocal, VERTEX_BUFFER::VB_TYPE::Instance);

	//�C���X�^���X���X�V
	SetInstanceNum(m_InstanceNum);

	return m_InstanceNum - 1;
}

//�C���X�^���X�N���A
void BONE::ClearInstance()
{
	m_InstanceNum = 0;
	m_aMtxLocal.clear();
	m_Scale = 1.0f;
	dx::XMStoreFloat4x4(&m_MtxWorld, dx::XMMatrixIdentity());
	m_Rot = { 0.0f, 0.0f, 0.0f };

	//�C���X�^���X�o�b�t�@�Đݒ�
	GetVertexBuffer().ResetInstanceBuffer(m_Gfx.m_DX, m_aMtxLocal);

	//�C���X�^���X���X�V
	SetInstanceNum(m_InstanceNum);
}
