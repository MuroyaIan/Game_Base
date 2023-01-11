
//===== インクルード部 =====
#include <GraphicApp/Binder/cbMaterial.h>
#include <GraphicApp/Drawer/vsdRef.h>

using pscb = PS_CBUFFER<CBD_MATERIAL>;

//===== 静的メンバ変数 =====
std::unique_ptr<pscb> CB_MATERIAL::m_pCBuffPS{};
int CB_MATERIAL::m_RefCount = 0;

//===== クラス実装 =====
CB_MATERIAL::CB_MATERIAL(const GRAPHIC& Gfx, const CBD_MATERIAL& Material) :
	BINDER(), m_Material(Material)
{
	//定数バッファ初期化
	if (!m_pCBuffPS)
		m_pCBuffPS = std::make_unique<pscb>(Gfx, static_cast<UINT>(CB_SLOT_PS::Material));
	m_RefCount++;
}

CB_MATERIAL::~CB_MATERIAL() noexcept
{
	//バッファ解放
	m_RefCount--;
	if (m_RefCount == 0)
		m_pCBuffPS.reset();
}

//バインド処理
void CB_MATERIAL::Bind(const GRAPHIC& Gfx) const noexcept
{
	//バッファ更新
	m_pCBuffPS->Update(Gfx, m_Material);

	//バインド処理
	m_pCBuffPS->Bind(Gfx);
}
