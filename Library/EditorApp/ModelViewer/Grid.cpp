
//===== �C���N���[�h�� =====
#include <EditorApp/ModelViewer/Grid.h>
#include <GraphicApp/Binder/BinderRef.h>
#include <GraphicApp/Drawer/vsdGrid.h>

namespace dx = DirectX;

//===== �N���X���� =====
GRID::GRID(GRAPHIC& Gfx, SHADER_MGR& ShaderMgr) :
	DRAWER(Gfx), m_ShaderMgr(ShaderMgr), m_mtxWorld(), m_Size(dx::XMFLOAT2(20.0f, 20.0f))
{
	//���_���쐬
	VS_DATA<VERTEX> Model = std::move(VSD_GRID::MakeData<VERTEX>(20, 20));
	dx::XMFLOAT4X4 mtx{};
	dx::XMStoreFloat4x4(&mtx, dx::XMMatrixScaling(m_Size.x, m_Size.y, 1.0f));
	Model.SetVertexPos(mtx);
	AddBind(std::make_unique<VERTEX_BUFFER>(Gfx, Model.m_Vertices));

	//�C���f�b�N�X���쐬
	AddBind(std::make_unique<INDEX_BUFFER>(Gfx, Model.m_Indices));

	//VS�萔�o�b�t�@�쐬�i�ϊ��s��j
	CB_PTR cbData;
	AddBind(std::make_unique<CB_MTX_WVP>(Gfx, &cbData, *this));

	//PS�萔�o�b�t�@�쐬�i�|���S���F�j
	const dx::XMFLOAT4 cbColor(0.5f, 0.5f, 0.5f, 1.0f);
	AddBind(std::make_unique<CONSTANT_BUFFER<dx::XMFLOAT4>>(Gfx, cbColor, &cbData, false, true));

	//�萔�o�b�t�@Mgr�쐬
	AddBind(std::make_unique<CBUFF_MGR>(cbData));

	//�s�񏉊���
	dx::XMMATRIX mtxW = dx::XMMatrixRotationRollPitchYaw(gMath::GetRad(90), 0.0f, 0.0f);	//�����p��
	dx::XMStoreFloat4x4(&m_mtxWorld, mtxW);
}

GRID::~GRID() noexcept
{
}

//�X�V����
void GRID::Update() noexcept
{
}

//�����ݏ���
void GRID::Draw(int InstanceNum) const noexcept
{
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::VS_Default);
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::IL_Default);
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PT_Line);
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PS_OneColor);
	DRAWER::Draw(InstanceNum);
}
