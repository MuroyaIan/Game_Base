
//===== インクルード部 =====
#include <GraphicApp/Binder/VertexBuffer.h>

//===== クラス実装 =====
VERTEX_BUFFER::~VERTEX_BUFFER() noexcept
{
}

//バインド処理
void VERTEX_BUFFER::Bind(GRAPHIC& Gfx) noexcept
{
	if (m_pVertexBuffer[static_cast<int>(VB_TYPE::INSTANCE)].Get() == nullptr) {

		//インスタンシングなし
		const UINT Stride = m_aStride[static_cast<int>(VB_TYPE::VERTEX)];
		const UINT Offset = 0u;
		GetContext(Gfx)->IASetVertexBuffers(0u, 1u, m_pVertexBuffer[static_cast<int>(VB_TYPE::VERTEX)].GetAddressOf(), &Stride, &Offset);
	}
	else {

		//インスタンシングあり
		ID3D11Buffer* aBufferPtr[2] = { m_pVertexBuffer[static_cast<int>(VB_TYPE::VERTEX)].Get(), m_pVertexBuffer[static_cast<int>(VB_TYPE::INSTANCE)].Get() };
		const UINT Stride[2] = { m_aStride[static_cast<int>(VB_TYPE::VERTEX)], m_aStride[static_cast<int>(VB_TYPE::INSTANCE)] };
		const UINT Offset[2] = { 0u, 0u };
		GetContext(Gfx)->IASetVertexBuffers(0u, 2u, aBufferPtr, Stride, Offset);
	}
}
