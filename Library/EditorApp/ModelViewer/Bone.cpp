
//===== インクルード部 =====
#include <EditorApp/ModelViewer/Bone.h>
#include <GraphicApp/Binder/BinderRef.h>
#include <EditorApp/ModelViewer/Viewer.h>

#include <Tool/Rand.h>

namespace dx = DirectX;

//===== クラス実装 =====
BONE::BONE(GFX_PACK& Gfx, VIEWER& Viewer, FBX_LOADER& Loader, INPUT_MGR& Input) :
	DRAWER(Gfx.m_DX), m_Gfx(Gfx), m_InstanceNum(0), m_Loader(Loader),
	m_aMtxLocal(m_InstanceNum), m_bDrawAnimation(Viewer.GetFlag_DrawAnimation()), m_AnimationID(Viewer.GetAnimationID()), m_AnimFrame(Viewer.GetAnimationFrame()), m_Scale(1.0f),
	m_MtxLocal(), m_MtxWorld(), m_ModelScale(Viewer.GetModelScale()),
	m_Input(Input), m_Rot()
{
	//頂点情報作成
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
	dx::XMStoreFloat4x4(&mtx, dx::XMMatrixScaling(1.0f, 1.0f, 1.0f) * dx::XMMatrixRotationRollPitchYaw(gMath::GetRad(90), 0.0f, 0.0f));	//Z軸が前向き
	Model_C.SetVertexPos(mtx);
	AddBind(std::make_unique<VERTEX_BUFFER>(m_Gfx.m_DX, Model_C.m_Vertices, m_aMtxLocal));

	//インデックス情報作成
	AddBind(std::make_unique<INDEX_BUFFER>(m_Gfx.m_DX, Model_C.m_Indices));

	//VS定数バッファ作成（変換行列）
	CB_PTR cbData;
	AddBind(std::make_unique<CB_MTX_LWVP>(m_Gfx.m_DX, &cbData, *this, m_MtxLocal));

	//定数バッファMgr作成
	AddBind(std::make_unique<CBUFF_MGR>(cbData));

	//ローカル行列初期化
	dx::XMStoreFloat4x4(&m_MtxLocal, dx::XMMatrixIdentity());

	//ワールド行列初期化
	dx::XMStoreFloat4x4(&m_MtxWorld, dx::XMMatrixIdentity());
}

BONE::~BONE() noexcept
{
}

//更新処理
void BONE::Update() noexcept
{
	//回転制御
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

	//ワールド行列更新
	dx::XMStoreFloat4x4(&m_MtxWorld, dx::XMMatrixScaling(m_ModelScale, m_ModelScale, m_ModelScale)
		* dx::XMMatrixRotationRollPitchYaw(m_Rot.x, m_Rot.y, m_Rot.z));

	//ローカル行列更新
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

//書込み処理
void BONE::Draw(int InstanceNum) const noexcept
{
	//例外処理
	(void)InstanceNum;
	if (m_InstanceNum < 1)
		return;

	//インスタンス更新
	std::vector<DirectX::XMFLOAT4X4> aMtxLocal = m_aMtxLocal;
	for (auto& i : aMtxLocal)
		gMath::MtxTranspose4x4_SSE(&i._11);
	GetVertexBuffer().UpdateBuffer(m_Gfx.m_DX, aMtxLocal, VERTEX_BUFFER::VB_TYPE::Instance);

	//インスタンス描画
	m_Gfx.m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::VS_MODEL_INSTANCE_VTX_BLEND);
	m_Gfx.m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::IL_MODEL_INSTANCE_VTX_BLEND);
	m_Gfx.m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PT_TRI);
	m_Gfx.m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PS_VTX_BLEND);
	DRAWER::Draw(m_InstanceNum);
}

//インスタンス追加
int BONE::AddInstance()
{
	//配列追加
	m_InstanceNum++;
	DirectX::XMFLOAT4X4 mtx{};
	dx::XMStoreFloat4x4(&mtx, dx::XMMatrixIdentity());
	m_aMtxLocal.emplace_back(mtx);

	//インスタンスバッファ再設定
	GetVertexBuffer().ResetInstanceBuffer(m_Gfx.m_DX, m_aMtxLocal);
	GetVertexBuffer().UpdateBuffer(m_Gfx.m_DX, m_aMtxLocal, VERTEX_BUFFER::VB_TYPE::Instance);

	//インスタンス数更新
	SetInstanceNum(m_InstanceNum);

	return m_InstanceNum - 1;
}

//インスタンスクリア
void BONE::ClearInstance()
{
	m_InstanceNum = 0;
	m_aMtxLocal.clear();
	m_Scale = 1.0f;
	dx::XMStoreFloat4x4(&m_MtxWorld, dx::XMMatrixIdentity());
	m_Rot = { 0.0f, 0.0f, 0.0f };

	//インスタンスバッファ再設定
	GetVertexBuffer().ResetInstanceBuffer(m_Gfx.m_DX, m_aMtxLocal);

	//インスタンス数更新
	SetInstanceNum(m_InstanceNum);
}
