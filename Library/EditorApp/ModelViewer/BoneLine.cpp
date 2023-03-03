
//===== インクルード部 =====
#include <EditorApp/ModelViewer/BoneLine.h>
#include <GraphicApp/Binder/BinderRef.h>
#include <EditorApp/ModelViewer/Viewer.h>

namespace dx = DirectX;

//===== プロトタイプ宣言 =====
void SetVertex(VS_DATA<VERTEX_C>& vsData, FBX_LOADER::BONE_DATA& Bone) noexcept;														//頂点設定
void SetVertexAnimation(VS_DATA<VERTEX_C>& vsData, FBX_LOADER::BONE_DATA& Bone, FBX_LOADER::SKIN_DATA& Skin, int& AnimFrame) noexcept;	//頂点設定（アニメーション再生時）

//===== クラス実装 =====
BONE_LINE::BONE_LINE(GRAPHIC& Gfx, SHADER_MGR& ShaderMgr, VIEWER& Viewer, FBX_LOADER& Loader, DRAWER& BoneIn) :
	DRAWER(Gfx), m_Gfx(Gfx), m_ShaderMgr(ShaderMgr), m_Loader(Loader), m_mtxWorld(), m_Bone(BoneIn),
	m_bDrawAnimation(Viewer.GetFlag_DrawAnimation()), m_AnimationID(Viewer.GetAnimationID()), m_AnimFrame(Viewer.GetAnimationFrame())
{

	//頂点情報作成
	VS_DATA<VERTEX_C> Model = MakeData_VS();
	AddBind(std::make_unique<VERTEX_BUFFER>(Gfx, Model.m_Vertices, true));

	//インデックス情報作成
	AddBind(std::make_unique<INDEX_BUFFER>(Gfx, Model.m_Indices));

	//VS定数バッファ作成（変換行列）
	CB_PTR cbData;
	AddBind(std::make_unique<CB_MTX_WVP>(Gfx, &cbData, *this));

	//定数バッファMgr作成
	AddBind(std::make_unique<CBUFF_MGR>(cbData));
}

BONE_LINE::~BONE_LINE() noexcept
{
}

//更新処理
void BONE_LINE::Update()
{
	//頂点更新
	VS_DATA<VERTEX_C> vsData;
	std::vector<FBX_LOADER::BONE_DATA>& BoneData = m_Loader.GetSkeleton();
	if (m_bDrawAnimation) {

		//アニメーション再生中⇒指定フレーム時の骨座標取得
		for (auto& b : BoneData) {
			if (b.IsRoot)
				SetVertexAnimation(vsData, b, m_Loader.GetSkin()[m_AnimationID], m_AnimFrame);
		}
		GetVertexBuffer().UpdateBuffer(m_Gfx, vsData.m_Vertices, VERTEX_BUFFER::VB_TYPE::Vertex);
	}
	else {

		//アニメーション再生なし⇒初期姿勢の骨座標取得
		for (auto& b : BoneData) {
			if (b.IsRoot)
				SetVertex(vsData, b);
		}
		GetVertexBuffer().UpdateBuffer(m_Gfx, vsData.m_Vertices, VERTEX_BUFFER::VB_TYPE::Vertex);
	}

	//ワールド行列更新
	m_mtxWorld = m_Bone.GetWorldMatrix();
}

//描画処理
void BONE_LINE::Draw(int InstanceNum) noexcept
{
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::VS_VtxBlend);
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::IL_VtxBlend);
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PT_Line);
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PS_VtxBlend);
	DRAWER::Draw(InstanceNum);
}

//頂点情報作成
VS_DATA<VERTEX_C> BONE_LINE::MakeData_VS() const
{
	//データ取得
	VS_DATA<VERTEX_C> vsData;
	std::vector<FBX_LOADER::BONE_DATA>& BoneData = m_Loader.GetSkeleton();
	for (auto& b : BoneData) {
		if (b.IsRoot)
			SetVertex(vsData, b);
	}

	//インデックス作成
	for (size_t i = 0, Cnt = vsData.m_Vertices.size(); i < Cnt; i++)
		vsData.m_Indices.push_back(static_cast<UINT>(i));

	return VS_DATA<VERTEX_C>(std::move(vsData));
}

//頂点設定
void SetVertex(VS_DATA<VERTEX_C>& vsData, FBX_LOADER::BONE_DATA& Bone) noexcept
{
	for (auto& b : Bone.aChildBone) {

		//親の座標
		dx::XMFLOAT3 Pos{ -Bone.InitMatrix._41, Bone.InitMatrix._42, Bone.InitMatrix._43 };
		vsData.m_Vertices.push_back(VERTEX_C{ Pos, VTX_COLOR(255, 63, 63, 255) });

		//子の座標
		Pos = { -b->InitMatrix._41, b->InitMatrix._42, b->InitMatrix._43 };
		vsData.m_Vertices.push_back(VERTEX_C{ Pos, VTX_COLOR(127, 255, 255, 255) });

		//子ノードの設定へ続く
		SetVertex(vsData, *b);
	}
}

//頂点設定（アニメーション再生時）
void SetVertexAnimation(VS_DATA<VERTEX_C>& vsData, FBX_LOADER::BONE_DATA& Bone, FBX_LOADER::SKIN_DATA& Skin, int& AnimFrame) noexcept
{
	for (auto& b : Bone.aChildBone) {

		//親の座標
		if (Skin.aFrameData[Bone.Index].aBoneMatrix.size() > 0) {									//フレーム情報がある場合
			dx::XMFLOAT4X4& FrameData = Skin.aFrameData[Bone.Index].aBoneMatrix[AnimFrame];
			dx::XMFLOAT3 Pos{ -(FrameData._41), FrameData._42, FrameData._43 };
			vsData.m_Vertices.push_back(VERTEX_C{ Pos, VTX_COLOR(255, 63, 63, 255) });
		}
		else {																						//フレーム情報がない場合（初期姿勢）
			dx::XMFLOAT3 Pos{ -Bone.InitMatrix._41, Bone.InitMatrix._42, Bone.InitMatrix._43 };
			vsData.m_Vertices.push_back(VERTEX_C{ Pos, VTX_COLOR(255, 63, 63, 255) });
		}

		//子の座標
		if (Skin.aFrameData[b->Index].aBoneMatrix.size() > 0) {
			dx::XMFLOAT4X4& FrameDataChild = Skin.aFrameData[b->Index].aBoneMatrix[AnimFrame];
			dx::XMFLOAT3 Pos = { -(FrameDataChild._41), FrameDataChild._42, FrameDataChild._43 };
			vsData.m_Vertices.push_back(VERTEX_C{ Pos, VTX_COLOR(127, 255, 255, 255) });
		}
		else {
			dx::XMFLOAT3 Pos = { -b->InitMatrix._41, b->InitMatrix._42, b->InitMatrix._43 };
			vsData.m_Vertices.push_back(VERTEX_C{ Pos, VTX_COLOR(127, 255, 255, 255) });
		}

		//子ノードの設定へ続く
		SetVertexAnimation(vsData, *b, Skin, AnimFrame);
	}
}
