
//===== インクルード部 =====
#include <EditorApp/ModelViewer/Surface.h>
#include <GraphicApp/Binder/BinderRef.h>
#include <EditorApp/ModelViewer/ModelLoader.h>

//===== クラス実装 =====
SURFACE::SURFACE(GRAPHIC& Gfx, SHADER_MGR& ShaderMgr, FBX_LOADER& Loader, int MeshIndex, DRAWER& ModelIn) :
	DRAWER(Gfx), m_ShaderMgr(ShaderMgr), m_Loader(Loader), m_MeshIndex(MeshIndex), m_mtxWorld(), m_Model(ModelIn)
{
	//頂点情報作成
	VS_DATA<VERTEX> Model = MakeData_VS();
	AddBind(std::make_unique<VERTEX_BUFFER>(Gfx, Model.m_Vertices));

	//インデックス情報作成
	AddBind(std::make_unique<INDEX_BUFFER>(Gfx, Model.m_Indices));

	//定数バッファ作成（変換行列）
	AddBind(std::make_unique<CB_MTX_T>(Gfx, *this));
}

SURFACE::~SURFACE() noexcept
{
}

//更新処理
void SURFACE::Update() noexcept
{
	//行列更新
	m_mtxWorld = m_Model.GetWorldMatrix();
}

void SURFACE::Draw(int InstanceNum) const noexcept
{
	m_ShaderMgr.Bind_Default();
	DRAWER::Draw(InstanceNum);
}

//頂点情報作成
VS_DATA<VERTEX> SURFACE::MakeData_VS() const
{
	//データ取得
	VS_DATA<VERTEX_M> vsData = m_Loader.GetMesh(m_MeshIndex).vsData;

	//頂点作成
	VS_DATA<VERTEX> vsDataOut;
	for (auto& d : vsData.m_Vertices) {
		VERTEX vtx = d.m_Pos;
		vtx.m_Pos.x *= -1.0f;
		vsDataOut.m_Vertices.emplace_back(std::move(vtx));
	}

	//インデックス作成
	for (auto& d : vsData.m_Indices)
		vsDataOut.m_Indices.emplace_back(d);

	return VS_DATA<VERTEX>(std::move(vsDataOut));
}
