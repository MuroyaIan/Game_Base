
//===== インクルード部 =====
#include <GraphicApp/Binder/cbMaterial.h>
#include <GraphicApp/Drawer/vsdRef.h>

using cb = CONSTANT_BUFFER<CBD_MATERIAL>;

//===== 静的メンバ変数 =====
std::unique_ptr<cb> CB_MATERIAL::m_pCBuff{};
int CB_MATERIAL::m_RefCount = 0;

//===== クラス実装 =====
CB_MATERIAL::CB_MATERIAL(const CT_GRAPHIC& Gfx, CB_PTR* cbPtr, const CBD_MATERIAL& Material) :
	CT_BINDER(), m_Material(Material)
{
	//定数バッファ初期化
	if (!m_pCBuff)
		m_pCBuff = std::make_unique<cb>(Gfx, cbPtr, false, true);
	else
		m_pCBuff->SetBuffPtr(cbPtr);
	m_RefCount++;
}

CB_MATERIAL::~CB_MATERIAL() noexcept
{
	//バッファ解放
	m_RefCount--;
	if (m_RefCount == 0)
		m_pCBuff.reset();
}

//バインド処理(データ更新)
void CB_MATERIAL::Bind(const CT_GRAPHIC& Gfx) const
{
	//バッファ更新
	m_pCBuff->Update(Gfx, m_Material);
}
