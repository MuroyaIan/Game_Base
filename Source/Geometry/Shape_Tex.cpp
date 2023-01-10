
//===== インクルード部 =====
#include <Geometry/Shape_Tex.h>
#include <GraphicApp/Binder/BinderRef.h>

#include <Tool/Rand.h>

namespace dx = DirectX;

//===== クラス実装 =====
SHAPE_TEX::SHAPE_TEX(GFX_PACK& Gfx, VSD_MAKER::SHAPE Type, TEXTURE_MGR::TEX_ID Tex) :
	DRAWER(Gfx.m_DX), m_Gfx(Gfx), m_Type(Type), m_InstanceNum(0), m_aMtxWorld(m_InstanceNum), m_Tex(Tex), m_aMtxData(m_InstanceNum)
{
	//頂点情報作成
	VS_DATA<VERTEX_T> Model = VSD_MAKER::MakeData_Tex(m_Type);
	dx::XMFLOAT4X4 mtx{};
	dx::XMStoreFloat4x4(&mtx, dx::XMMatrixScaling(1.0f, 1.0f, 1.0f));
	Model.SetVertexPos(mtx);
	AddBind(std::make_unique<VERTEX_BUFFER>(m_Gfx.m_DX, Model.m_Vertices, m_aMtxWorld));

	//インデックス情報作成
	AddBind(std::make_unique<INDEX_BUFFER>(m_Gfx.m_DX, Model.m_Indices));

	//テクスチャ初期化
	m_Gfx.m_TextureMgr.SetTextureOn(m_Tex);

	//行列情報初期化
	for (auto& md : m_aMtxData) {
		md.dt = 0.002f;
		md.m_r = RAND_MAKER::MakeRand_Float(6.0f, 20.0f);
		md.m_roll = 0.0f;
		md.m_pitch = 0.0f;
		md.m_yaw = 0.0f;
		md.m_theta	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 2.0f);
		md.m_phi	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 2.0f);
		md.m_chi	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 2.0f);
		md.m_droll	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 2.0f);
		md.m_dpitch	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 2.0f);
		md.m_dyaw	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 2.0f);
		md.m_dtheta	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 1.0f);
		md.m_dphi	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 1.0f);
		md.m_dchi	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 1.0f);
	}
}

SHAPE_TEX::~SHAPE_TEX() noexcept
{
	//テクスチャ終了
	m_Gfx.m_TextureMgr.SetTextureOff(m_Tex);
}

//更新処理
void SHAPE_TEX::Update() noexcept
{
	//ワールド行列更新
	for (int i = 0; i < m_InstanceNum; i++) {

		//自転更新
		m_aMtxData[i].m_roll += m_aMtxData[i].m_droll * m_aMtxData[i].dt;
		m_aMtxData[i].m_pitch += m_aMtxData[i].m_dpitch * m_aMtxData[i].dt;
		m_aMtxData[i].m_yaw += m_aMtxData[i].m_dyaw * m_aMtxData[i].dt;

		//公転更新
		m_aMtxData[i].m_theta += m_aMtxData[i].m_dtheta * m_aMtxData[i].dt;
		m_aMtxData[i].m_phi += m_aMtxData[i].m_dphi * m_aMtxData[i].dt;
		m_aMtxData[i].m_chi += m_aMtxData[i].m_dchi * m_aMtxData[i].dt;

		//行列更新
		dx::XMMATRIX mtx = dx::XMMatrixRotationRollPitchYaw(m_aMtxData[i].m_roll, m_aMtxData[i].m_pitch, m_aMtxData[i].m_yaw)	//自転
			* dx::XMMatrixTranslation(m_aMtxData[i].m_r, 0.0f, 0.0f)															//公転半径更新
			* dx::XMMatrixRotationRollPitchYaw(m_aMtxData[i].m_theta, m_aMtxData[i].m_phi, m_aMtxData[i].m_chi);				//公転
		dx::XMStoreFloat4x4(&m_aMtxWorld[i], mtx);
	}
}

//書込み処理
void SHAPE_TEX::Draw(int InstanceNum) const noexcept
{
	//例外処理
	(void)InstanceNum;
	if (m_InstanceNum < 1)
		return;

	//インスタンス更新
	std::vector<DirectX::XMFLOAT4X4> aMtxWorld = m_aMtxWorld;
	for (auto& i : aMtxWorld)
		gMath::MtxTranspose4x4_SSE(&i._11);
	GetVertexBuffer().UpdateBuffer(m_Gfx.m_DX, aMtxWorld, VERTEX_BUFFER::VB_TYPE::INSTANCE);

	//インスタンス描画
	m_Gfx.m_TextureMgr.Bind(m_Tex);
	m_Gfx.m_ShaderMgr.Bind_Instance_Texture();
	DRAWER::Draw(m_InstanceNum);
}

//インスタンス追加
int SHAPE_TEX::AddInstance()
{
	//配列追加
	m_InstanceNum++;
	DirectX::XMFLOAT4X4 mtx{};
	dx::XMStoreFloat4x4(&mtx, dx::XMMatrixIdentity());
	m_aMtxWorld.push_back(mtx);
	MTX_DATA md{};
	md.dt = 0.002f;
	md.m_r = RAND_MAKER::MakeRand_Float(6.0f, 20.0f);
	md.m_roll = 0.0f;
	md.m_pitch = 0.0f;
	md.m_yaw = 0.0f;
	md.m_theta	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 2.0f);
	md.m_phi	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 2.0f);
	md.m_chi	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 2.0f);
	md.m_droll	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 2.0f);
	md.m_dpitch	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 2.0f);
	md.m_dyaw	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 2.0f);
	md.m_dtheta	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 1.0f);
	md.m_dphi	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 1.0f);
	md.m_dchi	= RAND_MAKER::MakeRand_Float(0.0f, 3.1415f * 1.0f);
	m_aMtxData.push_back(md);

	//インスタンスバッファ再設定
	GetVertexBuffer().ResetInstanceBuffer(m_Gfx.m_DX, m_aMtxWorld);
	GetVertexBuffer().UpdateBuffer(m_Gfx.m_DX, m_aMtxWorld, VERTEX_BUFFER::VB_TYPE::INSTANCE);

	//インスタンス数更新
	SetInstanceNum(m_InstanceNum);

	return m_InstanceNum - 1;
}
