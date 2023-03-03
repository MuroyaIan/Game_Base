
//===== インクルード部 =====
#include <GraphicApp/Binder/Topology.h>

//===== クラス実装 =====
TOPOLOGY::TOPOLOGY(D3D11_PRIMITIVE_TOPOLOGY Type) noexcept :
	BINDER(), m_Type(Type)
{
}

TOPOLOGY::~TOPOLOGY() noexcept
{
}

//バインド処理
void TOPOLOGY::Bind(const GRAPHIC& Gfx) const
{
	GetContext(Gfx)->IASetPrimitiveTopology(m_Type);
}
