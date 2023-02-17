
//===== インクルード部 =====
#include <EditorApp/ModelViewer/Grid.h>
#include <GraphicApp/Binder/BinderRef.h>
#include <GraphicApp/Drawer/vsdGrid.h>

namespace dx = DirectX;

//===== クラス実装 =====
GRID::GRID(GRAPHIC& Gfx, SHADER_MGR& ShaderMgr) :
	DRAWER(Gfx), m_ShaderMgr(ShaderMgr), m_mtxWorld(), m_Size(dx::XMFLOAT2(20.0f, 20.0f))
{
	//頂点情報作成
	VS_DATA<VERTEX> Model = std::move(VSD_GRID::MakeData<VERTEX>(20, 20));
	dx::XMFLOAT4X4 mtx{};
	dx::XMStoreFloat4x4(&mtx, dx::XMMatrixScaling(m_Size.x, m_Size.y, 1.0f));
	Model.SetVertexPos(mtx);
	AddBind(std::make_unique<VERTEX_BUFFER>(Gfx, Model.m_Vertices));

	//インデックス情報作成
	AddBind(std::make_unique<INDEX_BUFFER>(Gfx, Model.m_Indices));

	//VS定数バッファ作成（変換行列）
	CB_PTR cbData;
	AddBind(std::make_unique<CB_MTX_WVP>(Gfx, &cbData, *this));

	//PS定数バッファ作成（ポリゴン色）
	const dx::XMFLOAT4 cbColor(0.5f, 0.5f, 0.5f, 1.0f);
	AddBind(std::make_unique<CONSTANT_BUFFER<dx::XMFLOAT4>>(Gfx, cbColor, &cbData, false, true));

	//定数バッファMgr作成
	AddBind(std::make_unique<CBUFF_MGR>(cbData));

	//行列初期化
	dx::XMMATRIX mtxW = dx::XMMatrixRotationRollPitchYaw(gMath::GetRad(90), 0.0f, 0.0f);	//初期姿勢
	dx::XMStoreFloat4x4(&m_mtxWorld, mtxW);
}

GRID::~GRID() noexcept
{
}

//更新処理
void GRID::Update() noexcept
{
}

//書込み処理
void GRID::Draw(int InstanceNum) const noexcept
{
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::VS_Default);
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::IL_Default);
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PT_Line);
	m_ShaderMgr.Bind(SHADER_MGR::BINDER_ID::PS_OneColor);
	DRAWER::Draw(InstanceNum);
}
