
//===== インクルード部 =====
#include <GraphicApp/Binder/cbBone.h>
#include <Tool/gMath.h>

using cb = CONSTANT_BUFFER<CBD_BONE>;

//===== 静的メンバ変数 =====
std::unique_ptr<cb> CB_BONE::m_pCBuff{};
int CB_BONE::m_RefCount = 0;

//===== クラス実装 =====
CB_BONE::CB_BONE(const CT_GRAPHIC& Gfx, CB_PTR* cbPtr, const CBD_BONE& aMtxBone, bool Transpose) :
	BINDER(), m_aMtxBone(aMtxBone), m_bTransposeMatrix(Transpose)
{
	//定数バッファ初期化
	if (!m_pCBuff)
		m_pCBuff = std::make_unique<cb>(Gfx, cbPtr, true);
	else
		m_pCBuff->SetBuffPtr(cbPtr);
	m_RefCount++;
}

CB_BONE::~CB_BONE() noexcept
{
	//バッファ解放
	m_RefCount--;
	if (m_RefCount == 0)
		m_pCBuff.reset();
}

//バインド処理(データ更新)
void CB_BONE::Bind(const CT_GRAPHIC& Gfx) const
{
	//バッファ更新
	if (m_bTransposeMatrix) {
		CBD_BONE BoneData = m_aMtxBone;
		for (auto& m : BoneData.mtxBone)
			gMath::MtxTranspose4x4_SSE(&m._11);
		m_pCBuff->Update(Gfx, BoneData);
	}
	else
		m_pCBuff->Update(Gfx, m_aMtxBone);
}
