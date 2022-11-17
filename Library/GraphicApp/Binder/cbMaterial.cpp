
//===== インクルード部 =====
#include <GraphicApp/Binder/cbMaterial.h>
#include <GraphicApp/Drawer/vsdRef.h>

using pcbMta = PIXEL_CBUFFER<MATERIAL_DATA>;

//===== 静的メンバ変数 =====
std::unique_ptr<pcbMta> CB_MATERIAL::m_pPcBuff = nullptr;
int CB_MATERIAL::m_RefCount = 0;

//===== クラス実装 =====
CB_MATERIAL::CB_MATERIAL(GRAPHIC& Gfx, const MATERIAL_DATA& Material) :
	BINDER(), m_Material(Material)
{
	//定数バッファ初期化
	if (!m_pPcBuff)
		m_pPcBuff = std::make_unique<pcbMta>(Gfx, static_cast<UINT>(CB_SLOT_PS::MATERIAL));
	m_RefCount++;
}

CB_MATERIAL::~CB_MATERIAL() noexcept
{
	//バッファ解放
	m_RefCount--;
	if (m_RefCount == 0)
		m_pPcBuff.reset();
}

//バインド処理
void CB_MATERIAL::Bind(GRAPHIC& Gfx) noexcept
{
	//バッファ更新
	m_pPcBuff->Update(Gfx, m_Material);

	//バインド処理
	m_pPcBuff->Bind(Gfx);
}
