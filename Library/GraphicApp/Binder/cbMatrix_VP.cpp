
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/cbMatrix_VP.h>
#include <Tool/gMath.h>

namespace dx = DirectX;
using cb = CONSTANT_BUFFER<CBD_MTX_VP>;

//===== �ÓI�����o�ϐ� =====
std::unique_ptr<cb> CB_MTX_VP::m_pCBuff{};
int CB_MTX_VP::m_RefCount = 0;

//===== �N���X���� =====
CB_MTX_VP::CB_MTX_VP(const CT_GRAPHIC& Gfx, CB_PTR* cbPtr) : BINDER()
{
	//�萔�o�b�t�@������
	if (!m_pCBuff)
		m_pCBuff = std::make_unique<cb>(Gfx, cbPtr, true);
	else
		m_pCBuff->SetBuffPtr(cbPtr);
	m_RefCount++;
}

CB_MTX_VP::~CB_MTX_VP() noexcept
{
	//�o�b�t�@���
	m_RefCount--;
	if (m_RefCount == 0)
		m_pCBuff.reset();
}

//�o�C���h����(�f�[�^�X�V)
void CB_MTX_VP::Bind(const CT_GRAPHIC& Gfx) const
{
	//�o�b�t�@�X�V
	CBD_MTX_VP aMtx;
	aMtx.mtxView = Gfx.GetViewMtx();
	aMtx.mtxProj = Gfx.GetProjectionMtx();
	gMath::MtxTranspose4x4_SSE(&aMtx.mtxView._11);
	gMath::MtxTranspose4x4_SSE(&aMtx.mtxProj._11);
	m_pCBuff->Update(Gfx, aMtx);
}
