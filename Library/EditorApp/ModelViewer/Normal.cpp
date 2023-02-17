
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
	VS_DATA<VERTEX_C> Model = MakeData_VS();
	AddBind(std::make_unique<VERTEX_BUFFER>(Gfx, Model.m_Vertices));

	//�C���f�b�N�X���쐬
	AddBind(std::make_unique<INDEX_BUFFER>(Gfx, Model.m_Indices));

	//VS�萔�o�b�t�@�쐬�i�ϊ��s��j
	CB_PTR cbData;
	AddBind(std::make_unique<CB_MTX_WVP>(Gfx, &cbData, *this));

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
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::VS_VtxBlend);
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::IL_VtxBlend);
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PT_Line);
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PS_VtxBlend);
	DRAWER::Draw(InstanceNum);
}

//���_���쐬
VS_DATA<VERTEX_C> NORMAL::MakeData_VS() const
{
	//�f�[�^�擾
	VS_DATA<VERTEX_M> vsData = m_Loader.GetMesh(m_MeshIndex).vsData;

	//���_�쐬
	VS_DATA<VERTEX_C> vsDataOut;
	for (auto& d : vsData.m_Indices) {

		//�n�_
		VERTEX_C vtx;
		vtx.m_Pos = vsData.m_Vertices[d].m_Pos;
		vtx.m_Pos.x *= -1.0f;
		vtx.m_Color = VTX_COLOR(255, 127, 127, 255);
		vsDataOut.m_Vertices.push_back(std::move(vtx));

		//�I�_
		VERTEX_C vtx2;
		vtx2.m_Pos.x = -(vsData.m_Vertices[d].m_Pos.x + vsData.m_Vertices[d].m_Normal.x);
		vtx2.m_Pos.y = vsData.m_Vertices[d].m_Pos.y + vsData.m_Vertices[d].m_Normal.y;
		vtx2.m_Pos.z = vsData.m_Vertices[d].m_Pos.z + vsData.m_Vertices[d].m_Normal.z;
		vtx2.m_Color = VTX_COLOR(255, 255, 127, 255);
		vsDataOut.m_Vertices.push_back(std::move(vtx2));
	}

	//�C���f�b�N�X�쐬
	for (size_t i = 0, Cnt = vsDataOut.m_Vertices.size(); i < Cnt; i++)
		vsDataOut.m_Indices.push_back(static_cast<UINT>(i));

	return VS_DATA<VERTEX_C>(std::move(vsDataOut));
}
