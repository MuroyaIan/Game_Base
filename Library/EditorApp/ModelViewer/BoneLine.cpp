
//===== インクルード部 =====
#include <EditorApp/ModelViewer/BoneLine.h>
#include <GraphicApp/Binder/BinderRef.h>
#include <EditorApp/ModelViewer/Viewer.h>

namespace dx = DirectX;

//===== プロトタイプ宣言 =====
void SetVertex(VS_DATA<VERTEX>& vsData, FBX_LOADER::BONE_DATA& Bone);															//頂点設定
void SetVertexAnimation(VS_DATA<VERTEX>& vsData, FBX_LOADER::BONE_DATA& Bone, FBX_LOADER::SKIN_DATA& Skin, int& AnimFrame);		//頂点設定（アニメーション再生時）

//===== クラス実装 =====
BONE_LINE::BONE_LINE(GRAPHIC& Gfx, SHADER_MGR& ShaderMgr, VIEWER& Viewer, FBX_LOADER& Loader, DRAWER& BoneIn) :
	DRAWER(Gfx), m_Gfx(Gfx), m_ShaderMgr(ShaderMgr), m_Loader(Loader), m_mtxWorld(), m_Bone(BoneIn),
	m_bDrawAnimation(Viewer.GetFlag_DrawAnimation()), m_AnimationID(Viewer.GetAnimationID()), m_AnimFrame(Viewer.GetAnimationFrame())
{

	//頂点情報作成
	VS_DATA<VERTEX> Model = MakeData_VS();
	AddBind(std::make_unique<VERTEX_BUFFER>(Gfx, Model.m_Vertices, true));

	//インデックス情報作成
	AddBind(std::make_unique<INDEX_BUFFER>(Gfx, Model.m_Indices));

	//VS定数バッファ作成（変換行列）
	CB_PTR cbData;
	AddBind(std::make_unique<CB_MTX_WVP>(Gfx, &cbData, *this));

	//PS定数バッファ作成（法線の色）
	const dx::XMFLOAT4 cbNormalColor(0.5f, 1.0f, 1.0f, 1.0f);
	AddBind(std::make_unique<CONSTANT_BUFFER<dx::XMFLOAT4>>(Gfx, cbNormalColor, &cbData, false, true));

	//定数バッファMgr作成
	AddBind(std::make_unique<CBUFF_MGR>(cbData));
}

BONE_LINE::~BONE_LINE() noexcept
{
}

//更新処理
void BONE_LINE::Update() noexcept
{
	//頂点更新
	VS_DATA<VERTEX> vsData;
	std::vector<FBX_LOADER::BONE_DATA>& BoneData = m_Loader.GetSkeleton();
	if (m_bDrawAnimation) {
		for (auto& b : BoneData) {
			if (b.IsRoot)
				SetVertexAnimation(vsData, b, m_Loader.GetSkin()[m_AnimationID], m_AnimFrame);
		}
		GetVertexBuffer().UpdateBuffer(m_Gfx, vsData.m_Vertices, VERTEX_BUFFER::VB_TYPE::Vertex);
	}
	else {
		for (auto& b : BoneData) {
			if (b.IsRoot)
				SetVertex(vsData, b);
		}
		GetVertexBuffer().UpdateBuffer(m_Gfx, vsData.m_Vertices, VERTEX_BUFFER::VB_TYPE::Vertex);
	}

	//ワールド行列更新
	m_mtxWorld = m_Bone.GetWorldMatrix();
}

void BONE_LINE::Draw(int InstanceNum) const noexcept
{
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::VS_DEFAULT);
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::IL_DEFAULT);
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PT_LINE);
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PS_ONE_COLOR);
	DRAWER::Draw(InstanceNum);
}

//頂点情報作成
VS_DATA<VERTEX> BONE_LINE::MakeData_VS() const
{
	//データ取得
	VS_DATA<VERTEX> vsData;
	std::vector<FBX_LOADER::BONE_DATA>& BoneData = m_Loader.GetSkeleton();
	for (auto& b : BoneData) {
		if (b.IsRoot)
			SetVertex(vsData, b);
	}

	//インデックス作成
	for (size_t i = 0, Cnt = vsData.m_Vertices.size(); i < Cnt; i++)
		vsData.m_Indices.emplace_back(static_cast<unsigned short>(i));

	return VS_DATA<VERTEX>(std::move(vsData));
}

//頂点設定
void SetVertex(VS_DATA<VERTEX>& vsData, FBX_LOADER::BONE_DATA& Bone)
{
	for (auto& b : Bone.aChildBone) {

		//親の座標
		dx::XMFLOAT3 Pos{ -Bone.InitMatrix._41, Bone.InitMatrix._42, Bone.InitMatrix._43 };
		vsData.m_Vertices.emplace_back(Pos);

		//子の座標
		Pos = { -b->InitMatrix._41, b->InitMatrix._42, b->InitMatrix._43 };
		vsData.m_Vertices.emplace_back(Pos);

		//子ノードの設定へ続く
		SetVertex(vsData, *b);
	}
}

//頂点設定（アニメーション再生時）
void SetVertexAnimation(VS_DATA<VERTEX>& vsData, FBX_LOADER::BONE_DATA& Bone, FBX_LOADER::SKIN_DATA& Skin, int& AnimFrame)
{
	for (auto& b : Bone.aChildBone) {

		//親の座標
		if (Skin.aFrameData[Bone.Index].aBoneMatrix.size() > 0) {									//フレーム情報がある場合
			dx::XMFLOAT4X4& FrameData = Skin.aFrameData[Bone.Index].aBoneMatrix[AnimFrame];
			dx::XMFLOAT3 Pos{ -(FrameData._41), FrameData._42, FrameData._43 };
			vsData.m_Vertices.emplace_back(Pos);
		}
		else {																						//フレーム情報がない場合（初期姿勢）
			dx::XMFLOAT3 Pos{ -Bone.InitMatrix._41, Bone.InitMatrix._42, Bone.InitMatrix._43 };
			vsData.m_Vertices.emplace_back(Pos);
		}

		//子の座標
		if (Skin.aFrameData[b->Index].aBoneMatrix.size() > 0) {
			dx::XMFLOAT4X4& FrameDataChild = Skin.aFrameData[b->Index].aBoneMatrix[AnimFrame];
			dx::XMFLOAT3 Pos = { -(FrameDataChild._41), FrameDataChild._42, FrameDataChild._43 };
			vsData.m_Vertices.emplace_back(Pos);
		}
		else {
			dx::XMFLOAT3 Pos = { -b->InitMatrix._41, b->InitMatrix._42, b->InitMatrix._43 };
			vsData.m_Vertices.emplace_back(Pos);
		}

		//子ノードの設定へ続く
		SetVertexAnimation(vsData, *b, Skin, AnimFrame);
	}
}
