
//===== インクルード部 =====
#include <GraphicApp/Binder/VertexBuffer.h>

//===== クラス実装 =====
VERTEX_BUFFER::~VERTEX_BUFFER() noexcept
{
}

//バインド処理
void VERTEX_BUFFER::Bind(const GRAPHIC& Gfx) const
{
	if (m_pVertexBuffers[static_cast<int>(VB_TYPE::Instance)].Get() == nullptr) {

		//インスタンシングなし
		const UINT Stride = m_aStride[static_cast<int>(VB_TYPE::Vertex)];
		const UINT Offset = 0u;
		GetContext(Gfx)->IASetVertexBuffers(0u, 1u, m_pVertexBuffers[static_cast<int>(VB_TYPE::Vertex)].GetAddressOf(), &Stride, &Offset);
	}
	else {

		//インスタンシングあり
		ID3D11Buffer* aBufferPtr[2] = { m_pVertexBuffers[static_cast<int>(VB_TYPE::Vertex)].Get(), m_pVertexBuffers[static_cast<int>(VB_TYPE::Instance)].Get() };
		const UINT Stride[2] = { m_aStride[static_cast<int>(VB_TYPE::Vertex)], m_aStride[static_cast<int>(VB_TYPE::Instance)] };
		const UINT Offset[2] = { 0u, 0u };
		GetContext(Gfx)->IASetVertexBuffers(0u, 2u, aBufferPtr, Stride, Offset);
	}
}

//バッファ設定取得
D3D11_BUFFER_DESC VERTEX_BUFFER::GetBufferDesc(UINT ArraySize, UINT TypeSize, bool bDynamic) const noexcept
{
	D3D11_BUFFER_DESC bd{};
	bd.ByteWidth = ArraySize;

	if (!bDynamic)
		bd.Usage = D3D11_USAGE_DEFAULT;
	else
		bd.Usage = D3D11_USAGE_DYNAMIC;

	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	if (!bDynamic)
		bd.CPUAccessFlags = 0u;
	else
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	bd.MiscFlags = 0u;
	bd.StructureByteStride = TypeSize;

	return bd;
}
