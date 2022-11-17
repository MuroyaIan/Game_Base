
//===== インクルード部 =====
#include <EditorApp/ModelViewer/Grid.h>
#include <GraphicApp/Binder/BinderRef.h>
#include <GraphicApp/Drawer/vsdGrid.h>

namespace dx = DirectX;

//===== クラス実装 =====
GRID::GRID(GRAPHIC& Gfx, SHADER_MGR& ShaderMgr) :
	DRAWER_EX(), m_ShaderMgr(ShaderMgr), m_mtxWorld(), m_Size(dx::XMFLOAT2(20.0f, 20.0f))
{
	//静的初期化が済んだ場合
	if (!StaticIsInit()) {

		//頂点情報作成
		VS_DATA<VERTEX> Model = std::move(VSD_GRID::MakeData<VERTEX>(20, 20));
		dx::XMFLOAT4X4 mtx{};
		dx::XMStoreFloat4x4(&mtx, dx::XMMatrixScaling(m_Size.x, m_Size.y, 1.0f));
		Model.InitSize(mtx);
		AddStaticBind(std::make_unique<VERTEX_BUFFER>(Gfx, Model.m_Vertices));

		//インデックス情報作成
		AddStaticBind(std::make_unique<INDEX_BUFFER>(Gfx, Model.m_Indices));

		//定数バッファ作成（ポリゴン色）
		const dx::XMFLOAT4 cbColor(0.5f, 0.5f, 0.5f, 1.0f);
		AddStaticBind(std::make_unique<PIXEL_CBUFFER<dx::XMFLOAT4>>(Gfx, cbColor));
	}

	//定数バッファ作成（変換行列）
	AddBind(std::make_unique<CB_MTX_T>(Gfx, *this));
}

GRID::~GRID() noexcept
{
}

//更新処理
void GRID::Update() noexcept
{
	//行列更新
	dx::XMMATRIX mtx = dx::XMMatrixRotationRollPitchYaw(gMath::GetRad(90), 0.0f, 0.0f);		//初期姿勢
	dx::XMStoreFloat4x4(&m_mtxWorld, mtx);
}

//書込み処理
void GRID::Draw(GRAPHIC& Gfx, bool bDrawInstance) const noexcept(!IS_DEBUG)
{
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::VS_DEFAULT);
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::IL_DEFAULT);
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PT_LINE);
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PS_ONE_COLOR);
	DRAWER::Draw(Gfx, bDrawInstance);
}
