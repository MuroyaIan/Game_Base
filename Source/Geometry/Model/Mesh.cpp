
//===== インクルード部 =====
#include <Geometry/Model/Mesh.h>
#include <GraphicApp/Binder/BinderRef.h>
#include <Light/LightMgr.h>
#include <Tool/Rand.h>

namespace dx = DirectX;

//===== クラス実装 =====
MESH::MESH(MODEL& ModelRef, int MeshIdx) :
	DRAWER(ModelRef.m_Gfx.m_DX), m_Gfx(ModelRef.m_Gfx),
	m_InstanceNum(ModelRef.m_InstanceNum), m_aInstanceData(ModelRef.m_aInstanceData), m_Material(),
	m_ModelRef(ModelRef), m_FileData(ModelRef.m_FileData), m_MeshIdx(MeshIdx), m_bStatic(ModelRef.m_bStatic),
	m_pLocalData(), m_AnimID(ModelRef.m_AnimID), m_AnimFrame(ModelRef.m_AnimFrame)
{
	//メッシュ情報確認
	ModelRef::MESH_PACK& Mesh = m_FileData.aMesh[m_MeshIdx];

	//頂点情報作成
	VS_DATA<VERTEX_MNB> vsdModel = Mesh.vsData;
	VS_DATA<VERTEX_MB> Model;
	Model.m_Indices = vsdModel.m_Indices;
	Model.m_Vertices.resize(Model.m_Indices.size());
	for (size_t i = 0, Cnt = Model.m_Vertices.size(); i < Cnt; i++) {
		Model.m_Vertices[i].m_Pos = vsdModel.m_Vertices[i].m_Pos;
		Model.m_Vertices[i].m_UV = vsdModel.m_Vertices[i].m_UV;
		Model.m_Vertices[i].m_Normal = vsdModel.m_Vertices[i].m_Normal;
		for (int j = 0; j < 4; j++) {
			Model.m_Vertices[i].m_BoneID[j] = vsdModel.m_Vertices[i].m_BoneID[j];
			Model.m_Vertices[i].m_BoneWeight[j] = vsdModel.m_Vertices[i].m_BoneWeight[j];
		}
	}
	AddBind(std::make_unique<VERTEX_BUFFER>(m_Gfx.m_DX, Model.m_Vertices, m_aInstanceData));

	//インデックス情報作成
	AddBind(std::make_unique<INDEX_BUFFER>(m_Gfx.m_DX, Model.m_Indices));

	//VS定数バッファ作成（カメラ）
	CB_PTR cbData;
	m_Gfx.m_ShaderMgr.SetConstBufferPtr(SHADER_MGR::BINDER_ID::CB_VS_MtxVP, &cbData);

	//PS定数バッファ作成（マテリアル）
	m_Material = Mesh.MaterialData;
	AddBind(std::make_unique<CB_MATERIAL>(m_Gfx.m_DX, &cbData, m_Material));

	//VS・PS定数バッファ作成（ライト）
	m_Gfx.m_ShaderMgr.SetConstBufferPtr(SHADER_MGR::BINDER_ID::CB_Light, &cbData);

	//VS定数バッファ作成(骨情報)
	if (!m_bStatic)
		dynamic_cast<CB_BONE*>(ModelRef.m_pBoneBuffer.get())->SetBuffPtr(&cbData);

	//VS定数バッファ作成(ローカル情報)
	if (!m_bStatic) {
		m_pLocalData = std::make_unique<CBD_MTX_LOCAL>();
		AddBind(std::make_unique<CB_LOCAL>(m_Gfx.m_DX, &cbData, *m_pLocalData));
	}

	//定数バッファMgr作成
	AddBind(std::make_unique<CBUFF_MGR>(cbData));

	//テクスチャバッファ作成
	SRV_PTR SrvData;
	if (Mesh.Tex_D.size() > 0)
		SrvData.m_aSrvPtrPS.push_back(m_Gfx.m_ModelMgr.SetTextureOn(m_ModelRef.m_ID, Mesh.Tex_D));		//Diffuseテクスチャ
	else
		SrvData.m_aSrvPtrPS.push_back(m_Gfx.m_TextureMgr.SetTextureOn(TEXTURE_MGR::TEX_ID::TEX_Null));	//空画像
	SrvData.m_aSrvPtrPS.push_back(m_Gfx.m_TextureMgr.SetTextureOn(TEXTURE_MGR::TEX_ID::TEX_Null));
	SrvData.m_aSrvPtrPS.push_back(m_Gfx.m_TextureMgr.SetTextureOn(TEXTURE_MGR::TEX_ID::TEX_Null));
	SrvData.m_aSrvPtrPS.push_back(m_Gfx.m_TextureMgr.SetTextureOn(TEXTURE_MGR::TEX_ID::TEX_Null));
	AddBind(std::make_unique<SRV_MGR>(SrvData));
}

MESH::~MESH() noexcept
{
	//テクスチャバッファ解放
	ModelRef::MESH_PACK& Mesh = m_FileData.aMesh[m_MeshIdx];
	if (Mesh.Tex_D.size() > 0)
		m_Gfx.m_ModelMgr.SetTextureOff(m_ModelRef.m_ID, Mesh.Tex_D);
	else
		m_Gfx.m_TextureMgr.SetTextureOff(TEXTURE_MGR::TEX_ID::TEX_Null);
	m_Gfx.m_TextureMgr.SetTextureOff(TEXTURE_MGR::TEX_ID::TEX_Null);
	m_Gfx.m_TextureMgr.SetTextureOff(TEXTURE_MGR::TEX_ID::TEX_Null);
	m_Gfx.m_TextureMgr.SetTextureOff(TEXTURE_MGR::TEX_ID::TEX_Null);
}

//更新処理
void MESH::Update() noexcept
{
	//例外処理
	if (m_InstanceNum < 1)
		return;

	//ローカル情報更新
	if (!m_bStatic) {
		if (m_FileData.aMesh[m_MeshIdx].aNoSkin.size() > 0)
			m_pLocalData->mtxSkin = m_FileData.aMesh[m_MeshIdx].aNoSkin[m_AnimID].aMatrix[m_AnimFrame];
	}
}

//書込み処理
void MESH::Draw(int InstanceNum) const noexcept
{
	//例外処理
	(void)InstanceNum;
	if (m_InstanceNum < 1)
		return;

	//インスタンス更新
	std::vector<VSD_INSTANCE> aInstData = m_aInstanceData;
	for (auto& i : aInstData)
		gMath::MtxTranspose4x4_SSE(&i.MtxWorld._11);
	GetVertexBuffer().UpdateBuffer(m_Gfx.m_DX, aInstData, VERTEX_BUFFER::VB_TYPE::Instance);

	//インスタンス描画
	if (m_bStatic)
		m_Gfx.m_ShaderMgr.Bind_Instance_Phong();
	else
		m_Gfx.m_ShaderMgr.Bind_Instance_Phong_Anim();
	DRAWER::Draw(m_InstanceNum);
}

//インスタンス追加
int MESH::AddInstance()
{
	//インスタンスバッファ再設定
	GetVertexBuffer().ResetInstanceBuffer(m_Gfx.m_DX, m_aInstanceData);

	//インスタンス数更新
	SetInstanceNum(m_InstanceNum);

	return 0;
}
