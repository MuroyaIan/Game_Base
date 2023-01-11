
//===== インクルード部 =====
#include <GraphicApp/Binder/cbBone.h>
#include <Tool/gMath.h>

using vscb = VS_CBUFFER<CBD_BONE>;

//===== 静的メンバ変数 =====
std::unique_ptr<vscb> CB_BONE::m_pCBuffVS{};
int CB_BONE::m_RefCount = 0;

//===== クラス実装 =====
CB_BONE::CB_BONE(const GRAPHIC& Gfx, const CBD_BONE& aMtxBone, bool Transpose) :
	BINDER(), m_aMtxBone(aMtxBone), m_bTransposeMatrix(Transpose)
{
	//定数バッファ初期化
	if (!m_pCBuffVS)
		m_pCBuffVS = std::make_unique<vscb>(Gfx, static_cast<UINT>(CB_SLOT_VS::Bone));
	m_RefCount++;
}

CB_BONE::~CB_BONE() noexcept
{
	//バッファ解放
	m_RefCount--;
	if (m_RefCount == 0)
		m_pCBuffVS.reset();
}

//バインド処理
void CB_BONE::Bind(const GRAPHIC& Gfx) const noexcept
{
	//バッファ更新
	if (m_bTransposeMatrix) {
		CBD_BONE BoneData = m_aMtxBone;
		for (auto& m : BoneData.mtxBone)
			gMath::MtxTranspose4x4_SSE(&m._11);
		m_pCBuffVS->Update(Gfx, BoneData);
	}
	else
		m_pCBuffVS->Update(Gfx, m_aMtxBone);

	//バインド処理
	m_pCBuffVS->Bind(Gfx);
}
