
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/cbMatrix_T.h>
#include <GraphicApp/Drawer/Drawer.h>
#include <Tool/gMath.h>

namespace dx = DirectX;
using cb = CONSTANT_BUFFER<dx::XMFLOAT4X4>;

//===== �ÓI�����o�ϐ� =====
std::unique_ptr<cb> CB_MTX_T::m_pCBuff{};
int CB_MTX_T::m_RefCount = 0;

//===== �N���X���� =====
CB_MTX_T::CB_MTX_T(const GRAPHIC& Gfx, CB_PTR* cbPtr, const DRAWER& Parent) :
	BINDER(), m_Parent(Parent)
{
	//�萔�o�b�t�@������
	if (!m_pCBuff)
		m_pCBuff = std::make_unique<cb>(Gfx, cbPtr, true);
	else
		m_pCBuff->SetBuffPtr(cbPtr);
	m_RefCount++;
}

CB_MTX_T::~CB_MTX_T() noexcept
{
	//�o�b�t�@���
	m_RefCount--;
	if (m_RefCount == 0)
		m_pCBuff.reset();
}

//�o�C���h����(�f�[�^�X�V)
void CB_MTX_T::Bind(const GRAPHIC& Gfx) const noexcept
{
	//�o�b�t�@�X�V
	dx::XMFLOAT4X4 World = m_Parent.GetWorldMatrix();
	dx::XMFLOAT4X4 View = Gfx.GetViewMtx();
	dx::XMFLOAT4X4 Proj = Gfx.GetProjectionMtx();
	dx::XMFLOAT4X4 mtxWVP = World;
	gMath::MtxMultiply4x4_AVX(&mtxWVP._11, &View._11);
	gMath::MtxMultiply4x4_AVX(&mtxWVP._11, &Proj._11);
	gMath::MtxTranspose4x4_SSE(&mtxWVP._11);
	m_pCBuff->Update(Gfx, mtxWVP);
}
