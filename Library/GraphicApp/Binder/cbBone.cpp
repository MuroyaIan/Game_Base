
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/cbBone.h>
#include <Tool/gMath.h>

using vcbBone = VERTEX_CBUFFER<CB_MTX_BONE>;

//===== �ÓI�����o�ϐ� =====
std::unique_ptr<vcbBone> CB_BONE::m_pVcBuff = nullptr;

//===== �N���X���� =====
CB_BONE::CB_BONE(GRAPHIC& Gfx, const CB_MTX_BONE& aMtxBone, bool Transpose) :
	BINDER(), m_aMtxBone(aMtxBone), m_bTransposeMatrix(Transpose)
{
	//�萔�o�b�t�@������
	if (!m_pVcBuff)
		m_pVcBuff = std::make_unique<vcbBone>(Gfx, static_cast<UINT>(CB_SLOT_VS::BONE));
}

CB_BONE::~CB_BONE() noexcept
{
}

//�o�C���h����
void CB_BONE::Bind(GRAPHIC& Gfx) noexcept
{
	//�o�b�t�@�X�V
	if (m_bTransposeMatrix) {
		CB_MTX_BONE BoneData = m_aMtxBone;
		for (auto& m : BoneData.mtxBone)
			gMath::MtxTranspose4x4_SSE(&m._11);
		m_pVcBuff->Update(Gfx, BoneData);
	}
	else
		m_pVcBuff->Update(Gfx, m_aMtxBone);

	//�o�C���h����
	m_pVcBuff->Bind(Gfx);
}
