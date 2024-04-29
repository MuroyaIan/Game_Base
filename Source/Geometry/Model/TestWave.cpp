
//===== インクルード部 =====
#include <Geometry/Model/TestWave.h>
#include <GraphicApp/Binder/BinderRef.h>

#include <Tool/Rand.h>
#include <Tool/gMath.h>

namespace dx = DirectX;

//===== クラス実装 =====
WAVE::WAVE(APP& App) :
	DRAWER(App.GetGfxPack().m_DX), m_Gfx(App.GetGfxPack()), m_InstanceNum(22500), m_aInstanceData(m_InstanceNum), m_Material(),
	m_Input(App.GetInputMgr()), m_aInitPos(m_InstanceNum), m_WaveLength(0.5f), m_WaveCoef(1.0f)
{
	//頂点情報作成
	VS_DATA<VERTEX_M> Model = VSD_SPHERE::MakeData_Model<VERTEX_M>(8, 8);
	dx::XMFLOAT4X4 mtx{};
	dx::XMStoreFloat4x4(&mtx, dx::XMMatrixScaling(1.0f, 1.0f, 1.0f));
	Model.SetVertexPos(mtx);
	for (auto& v : Model.m_Vertices) {	//座標系変換
		v.m_Pos.x *= -1.0f;
		v.m_Normal.x *= -1.0f;
	}
	AddBind(std::make_unique<VERTEX_BUFFER>(m_Gfx.m_DX, Model.m_Vertices, m_aInstanceData));

	//インデックス情報作成
	AddBind(std::make_unique<INDEX_BUFFER>(m_Gfx.m_DX, Model.m_Indices));

	//定数バッファ作成（マテリアル）
	AddBind(std::make_unique<CB_MATERIAL>(m_Gfx.m_DX, nullptr, m_Material));

	//テクスチャバッファ作成
	std::vector<TEX_LOADER::TEX_DATA> aData(static_cast<int>(TEXTURE_MODEL::TEX_TYPE::MaxType));
	//aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Diffuse)] = TEX_LOADER::LoadTexture("Asset/Texture/test2.png");
	aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Diffuse)] = TEX_LOADER::LoadTexture("Asset/Texture/null.png");
	aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Specular)] = TEX_LOADER::LoadTexture("Asset/Texture/null.png");
	aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Normal)] = TEX_LOADER::LoadTexture("Asset/Texture/null.png");
	AddBind(std::make_unique<TEXTURE_MODEL>(m_Gfx.m_DX, aData));
	for (auto& d : aData)
		TEX_LOADER::ReleaseTexture(d.pImageData);

	//インスタンス数初期化
	SetInstanceNum(m_InstanceNum);

	//位置情報初期化
	int EdgeSize = static_cast<int>(std::sqrt(m_InstanceNum));	//波方陣の辺の長さ
	float EdgeCoef = 2.0f;										//辺の長さ用係数
	float EdgeHalfSize = EdgeSize * EdgeCoef * 0.5f;			//辺の長さの半分
	for (int i = 0; i < m_InstanceNum; i++) {
		dx::XMMATRIX mtxT = dx::XMMatrixTranslation(0.0f + EdgeCoef * (i % EdgeSize), 0.0f, 0.0f + EdgeCoef * static_cast<float>(i / EdgeSize))		//初期位置（XY平面）
			* dx::XMMatrixTranslation(-EdgeHalfSize, 0.0f, -EdgeHalfSize * 0.35f);																			//波の中心座標を修正
		dx::XMVECTOR vPos = dx::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		dx::XMStoreFloat3(&m_aInitPos[i], dx::XMVector3Transform(vPos, mtxT));
	}

	//マテリアル情報初期化
	m_Material.ms_Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_Material.ms_Specular = { 0.6f, 0.6f, 0.6f, 1.0f };
	m_Material.ms_Shininess = 30.0f;
}

WAVE::~WAVE() noexcept
{
}

//更新処理
void WAVE::Update() noexcept
{
	//タイマ制御(直進波)
	static float swTimer = 0.0f;
	static const float swTimeCoef = 0.01f;						//タイマ用係数
	swTimer += swTimeCoef;

	//タイマ制御(環状波)
	static float rwTimer = -1.0f;
	static const float rwTimeCoef = 0.1f;						//タイマ用係数
	static const float rwEndTime = rwTimeCoef * 60.0f * 5.0f;	//タイマ終了時間
	if (m_Input.m_KB.GetTrigger(VK_SPACE) &&
		rwTimer == -1.0f)
		rwTimer = 0.0f;											//波開始
	if (rwTimer >= 0.0f) {
		rwTimer += rwTimeCoef;

		//波終了
		if (rwTimer >= rwEndTime)
			rwTimer = -1.0f;
	}

	//ワールド行列更新
	for (int i = 0; i < m_InstanceNum; i++) {

		//直進波を計算
		m_WaveLength = 50.0f;
		m_WaveCoef = 0.5f;
		float swHeight = ClacStraightWave(VECTOR2{ 1.0f, 1.0f }, VECTOR2{ m_aInitPos[i].x, m_aInitPos[i].z }, swTimer);

		//環状波を計算
		m_WaveLength = 40.0f;
		m_WaveCoef = 4.0f;
		float rwHeight = ClacRingWave(VECTOR2{ 0.0f, 0.0f }, VECTOR2{ m_aInitPos[i].x, m_aInitPos[i].z }, rwTimer);

		//行列更新
		dx::XMMATRIX mtx = dx::XMMatrixTranslation(m_aInitPos[i].x, swHeight + rwHeight, m_aInitPos[i].z);	//波用変換行列
		dx::XMStoreFloat4x4(&m_aInstanceData[i].MtxWorld, mtx);
	}
}

//描画処理
void WAVE::Draw(int InstanceNum)
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
	m_Gfx.m_ShaderMgr.Bind_Instance_Phong();
	DRAWER::Draw(m_InstanceNum);
}

//インスタンス追加
int WAVE::AddInstance()
{
	//配列追加
	m_InstanceNum++;
	VSD_INSTANCE InstData{};
	m_aInstanceData.push_back(InstData);

	//インスタンスバッファ再設定
	GetVertexBuffer().ResetInstanceBuffer(m_Gfx.m_DX, m_aInstanceData);
	GetVertexBuffer().UpdateBuffer(m_Gfx.m_DX, m_aInstanceData, VERTEX_BUFFER::VB_TYPE::Instance);

	//インスタンス数更新
	SetInstanceNum(m_InstanceNum);

	return m_InstanceNum - 1;
}

//正弦波による高さ計算
float WAVE::ClacSineWave(float Dist, float Time, bool bLoop)
{
	float Height = 0.0f;												//波の高さ
	float Period = f2PI / m_WaveLength * Dist;							//周期
	float Phase = std::sqrtf(f2PI * fGravity / m_WaveLength) * Time;	//位相

	//無限波
	if (bLoop) {
		Height = m_WaveCoef * m_WaveLength / 14.0f * std::sinf(Period - Phase);
		return Height;
	}

	//波1個分
	if (Phase < Period ||
		Phase >= f2PI + Period)
		Height = 0.0f;
	else
		Height = m_WaveCoef * m_WaveLength / 14.0f * std::sinf(Period - Phase);

	return Height;
}

//環状波による高さ計算
float WAVE::ClacRingWave(VECTOR2 OriPos, VECTOR2 Pos, float Time)
{
	float Dist = gMath::VecLength(Pos - OriPos);
	return ClacSineWave(Dist, Time, false);
}

//直進波による高さ計算
float WAVE::ClacStraightWave(VECTOR2 vDirection, VECTOR2 vPos, float Time)
{
	float Dist = gMath::VecDot(gMath::VecNormalize(vDirection), vPos);
	return ClacSineWave(Dist, Time);
}
