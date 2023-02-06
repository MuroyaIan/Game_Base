
//===== インクルード部 =====
#include <Light/LightMgr.h>
#include <GraphicApp/Binder/BinderRef.h>

//===== クラス実装 =====
LIGHT_MGR::LIGHT_MGR(APP& App) noexcept :
	m_Gfx(App.GetGfxPack()), m_cBuffRef(dynamic_cast<CONSTANT_BUFFER<LIGHT_PACK>&>(m_Gfx.m_ShaderMgr.GetBinder(SHADER_MGR::BINDER_ID::CB_PS_LIGHT))),
	m_LightData(), m_UsedData(0), m_UsedData_backup(m_UsedData)
{
}

LIGHT_MGR::~LIGHT_MGR() noexcept
{
}

//更新処理
void LIGHT_MGR::Update()
{
	//定数バッファ更新
	m_cBuffRef.Update(m_Gfx.m_DX, m_LightData);

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
