
//===== インクルード部 =====
#include <Light/LightMgr.h>
#include <GraphicApp/Binder/BinderRef.h>

//===== クラス実装 =====
LIGHT_MGR::LIGHT_MGR(APP& App) noexcept :
	m_Gfx(App.GetGfxPack()), m_LightData(), m_UsedData(0), m_UsedData_backup(m_UsedData), m_pcBuff()
{
	//定数バッファ作成
	m_pcBuff = std::make_unique<PS_CBUFFER<LIGHT_PACK>>(m_Gfx.m_DX, static_cast<UINT>(CB_SLOT_PS::Light));
}

LIGHT_MGR::~LIGHT_MGR() noexcept
{
}

//書込み処理
void LIGHT_MGR::Draw() noexcept
{
	//定数バッファ更新
	m_pcBuff->Update(m_Gfx.m_DX, m_LightData);
	m_pcBuff->Bind(m_Gfx.m_DX);

	//ライト登録リセット
	m_UsedData_backup = m_UsedData;
	m_UsedData = 0;
}

//ポイントライト登録
void LIGHT_MGR::AddPointLight(LIGHT::LIGHT_POINT Data) noexcept
{
	//例外処理
	if (m_UsedData >= LIGHT_NUM)
		return;

	//データ更新
	m_LightData.PointLight[m_UsedData] = Data;
	m_UsedData++;
}
