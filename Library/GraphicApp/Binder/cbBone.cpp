
//===== インクルード部 =====
#include <GraphicApp/Binder/cbBone.h>
#include <Tool/gMath.h>

using vcbBone = VERTEX_CBUFFER<CBD_MTX_BONE>;

//===== 静的メンバ変数 =====
std::unique_ptr<vcbBone> CB_BONE::m_pVcBuff = nullptr;
int CB_BONE::m_RefCount = 0;

//===== クラス実装 =====
CB_BONE::CB_BONE(GRAPHIC& Gfx, const CBD_MTX_BONE& aMtxBone, bool Transpose) :
	BINDER(), m_aMtxBone(aMtxBone), m_bTransposeMatrix(Transpose)
{
	//定数バッファ初期化
	if (!m_pVcBuff)
		m_pVcBuff = std::make_unique<vcbBone>(Gfx, static_cast<UINT>(CB_SLOT_VS::BONE));
	m_RefCount++;
}

CB_BONE::~CB_BONE() noexcept
{
	//バッファ解放
	m_RefCount--;
	if (m_RefCount == 0)
		m_pVcBuff.reset();
}

//バインド処理
void CB_BONE::Bind(const GRAPHIC& Gfx) noexcept
{
	//バッファ更新
	if (m_bTransposeMatrix) {
		CBD_MTX_BONE BoneData = m_aMtxBone;
		for (auto& m : BoneData.mtxBone)
			gMath::MtxTranspose4x4_SSE(&m._11);
		m_pVcBuff->Update(Gfx, BoneData);
	}
	else
		m_pVcBuff->Update(Gfx, m_aMtxBone);

	//バインド処理
	m_pVcBuff->Bind(Gfx);
}
