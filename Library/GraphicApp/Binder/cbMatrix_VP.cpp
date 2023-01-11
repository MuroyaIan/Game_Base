
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/cbMatrix_VP.h>
#include <Tool/gMath.h>

namespace dx = DirectX;
using vscb = VS_CBUFFER<CBD_MTX_VP>;

//===== �ÓI�����o�ϐ� =====
std::unique_ptr<vscb> CB_MTX_VP::m_pCBuffVS{};
int CB_MTX_VP::m_RefCount = 0;

//===== �N���X���� =====
CB_MTX_VP::CB_MTX_VP(const GRAPHIC& Gfx) : BINDER()
{
	//�萔�o�b�t�@������
	if (!m_pCBuffVS)
		m_pCBuffVS = std::make_unique<vscb>(Gfx, static_cast<UINT>(CB_SLOT_VS::Camera));
	m_RefCount++;
}

CB_MTX_VP::~CB_MTX_VP() noexcept
{
	//�o�b�t�@���
	m_RefCount--;
	if (m_RefCount == 0)
		m_pCBuffVS.reset();
}

//�o�C���h����
void CB_MTX_VP::Bind(const GRAPHIC& Gfx) const noexcept
{
	//�o�b�t�@�X�V
	dx::XMFLOAT4X4 View = Gfx.GetViewMtx();
	dx::XMFLOAT4X4 Proj = Gfx.GetProjectionMtx();
	gMath::MtxTranspose4x4_SSE(&View._11);
	gMath::MtxTranspose4x4_SSE(&Proj._11);
	CBD_MTX_VP aMtx = { View, Proj };
	m_pCBuffVS->Update(Gfx, aMtx);

	//�o�C���h����
	m_pCBuffVS->Bind(Gfx);
}
