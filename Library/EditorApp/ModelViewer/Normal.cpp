
//===== �C���N���[�h�� =====
#include <EditorApp/ModelViewer/Normal.h>
#include <GraphicApp/Binder/BinderRef.h>
#include <EditorApp/ModelViewer/ModelLoader.h>

namespace dx = DirectX;

//===== �N���X���� =====
NORMAL::NORMAL(GRAPHIC& Gfx, SHADER_MGR& ShaderMgr, FBX_LOADER& Loader, int MeshIndex, DRAWER& ModelIn) :
	DRAWER(Gfx), m_ShaderMgr(ShaderMgr), m_Loader(Loader), m_MeshIndex(MeshIndex), m_mtxWorld(), m_Model(ModelIn)
{
	//���_���쐬
	VS_DATA<VERTEX> Model = MakeData_VS();
	AddBind(std::make_unique<VERTEX_BUFFER>(Gfx, Model.m_Vertices));

	//�C���f�b�N�X���쐬
	AddBind(std::make_unique<INDEX_BUFFER>(Gfx, Model.m_Indices));

	//VS�萔�o�b�t�@�쐬�i�ϊ��s��j
	CB_PTR cbData;
	AddBind(std::make_unique<CB_MTX_T>(Gfx, &cbData, *this));

	//PS�萔�o�b�t�@�쐬�i�@���̐F�j
	const dx::XMFLOAT4 cbNormalColor(1.0f, 1.0f, 0.5f, 1.0f);
	AddBind(std::make_unique<CONSTANT_BUFFER<dx::XMFLOAT4>>(Gfx, cbNormalColor, &cbData, false, true));

	//�萔�o�b�t�@Mgr�쐬
	AddBind(std::make_unique<CBUFF_MGR>(cbData));
}

NORMAL::~NORMAL() noexcept
{
}

//�X�V����
void NORMAL::Update() noexcept
{
	//�s��X�V
	m_mtxWorld = m_Model.GetWorldMatrix();
}

void NORMAL::Draw(int InstanceNum) const noexcept
{
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::VS_DEFAULT);
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::IL_DEFAULT);
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PT_LINE);
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PS_ONE_COLOR);
	DRAWER::Draw(InstanceNum);
}

//���_���쐬
VS_DATA<VERTEX> NORMAL::MakeData_VS() const
{
	//�f�[�^�擾
	VS_DATA<VERTEX_M> vsData = m_Loader.GetMesh(m_MeshIndex).vsData;

	//���_�쐬
	VS_DATA<VERTEX> vsDataOut;
	for (auto& d : vsData.m_Indices) {

		//�n�_
		VERTEX vtx = VERTEX{ vsData.m_Vertices[d].m_Pos };
		vtx.m_Pos.x *= -1.0f;
		vsDataOut.m_Vertices.emplace_back(std::move(vtx));

		//�I�_
		DirectX::XMFLOAT3 Pos{};
		Pos.x = -(vsData.m_Vertices[d].m_Pos.x + vsData.m_Vertices[d].m_Normal.x);
		Pos.y = vsData.m_Vertices[d].m_Pos.y + vsData.m_Vertices[d].m_Normal.y;
		Pos.z = vsData.m_Vertices[d].m_Pos.z + vsData.m_Vertices[d].m_Normal.z;
		vsDataOut.m_Vertices.emplace_back(VERTEX(Pos));
	}

	//�C���f�b�N�X�쐬
	for (size_t i = 0, Cnt = vsDataOut.m_Vertices.size(); i < Cnt; i++)
		vsDataOut.m_Indices.push_back(static_cast<unsigned short>(i));

	return VS_DATA<VERTEX>(std::move(vsDataOut));
}
