
//===== �C���N���[�h�� =====
#include <EditorApp/ModelViewer/Surface.h>
#include <GraphicApp/Binder/BinderRef.h>
#include <EditorApp/ModelViewer/ModelLoader.h>

//===== �N���X���� =====
SURFACE::SURFACE(GRAPHIC& Gfx, SHADER_MGR& ShaderMgr, FBX_LOADER& Loader, int MeshIndex, DRAWER& ModelIn) :
	DRAWER(Gfx), m_ShaderMgr(ShaderMgr), m_Loader(Loader), m_MeshIndex(MeshIndex), m_mtxWorld(), m_Model(ModelIn)
{
	//���_���쐬
	VS_DATA<VERTEX> Model = MakeData_VS();
	AddBind(std::make_unique<VERTEX_BUFFER>(Gfx, Model.m_Vertices));

	//�C���f�b�N�X���쐬
	AddBind(std::make_unique<INDEX_BUFFER>(Gfx, Model.m_Indices));

	//�萔�o�b�t�@�쐬�i�ϊ��s��j
	AddBind(std::make_unique<CB_MTX_T>(Gfx, *this));
}

SURFACE::~SURFACE() noexcept
{
}

//�X�V����
void SURFACE::Update() noexcept
{
	//�s��X�V
	m_mtxWorld = m_Model.GetWorldMatrix();
}

void SURFACE::Draw(int InstanceNum) const noexcept
{
	m_ShaderMgr.Bind_Default();
	DRAWER::Draw(InstanceNum);
}

//���_���쐬
VS_DATA<VERTEX> SURFACE::MakeData_VS() const
{
	//�f�[�^�擾
	VS_DATA<VERTEX_M> vsData = m_Loader.GetMesh(m_MeshIndex).vsData;

	//���_�쐬
	VS_DATA<VERTEX> vsDataOut;
	for (auto& d : vsData.m_Vertices) {
		VERTEX vtx = d.m_Pos;
		vtx.m_Pos.x *= -1.0f;
		vsDataOut.m_Vertices.emplace_back(std::move(vtx));
	}

	//�C���f�b�N�X�쐬
	for (auto& d : vsData.m_Indices)
		vsDataOut.m_Indices.emplace_back(d);

	return VS_DATA<VERTEX>(std::move(vsDataOut));
}
