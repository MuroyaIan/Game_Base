
//===== �C���N���[�h�� =====
#include <GraphicApp/Binder/Topology.h>

//===== �N���X���� =====
TOPOLOGY::TOPOLOGY(D3D11_PRIMITIVE_TOPOLOGY Type) noexcept :
	BINDER(), m_Type(Type)
{
}

TOPOLOGY::~TOPOLOGY() noexcept
{
}

//�o�C���h����
void TOPOLOGY::Bind(GRAPHIC& Gfx) noexcept
{
	GetContext(Gfx)->IASetPrimitiveTopology(m_Type);
}
