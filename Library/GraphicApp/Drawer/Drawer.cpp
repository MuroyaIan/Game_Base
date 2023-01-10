
//===== インクルード部 =====
#include <GraphicApp/Drawer/Drawer.h>
#include <typeinfo>								//ポインタ内データの型確認
#include <GraphicApp/Binder/IndexBuffer.h>
#include <GraphicApp/Binder/VertexBuffer.h>

//===== クラス実装 =====
DRAWER::DRAWER(const GRAPHIC& Gfx) noexcept : m_Gfx(Gfx), m_aBinder(),
	m_pIndexBuffer(nullptr), m_pVertexBuffer(nullptr), m_MaxInstanceNum(0)
{
}

DRAWER::~DRAWER() noexcept
{
}

//描画処理
void DRAWER::Draw(int InstanceNum) const noexcept
{
	//バインド処理
	for (auto& b : m_aBinder)
		b->Bind(m_Gfx);

	//描画処理
	if (InstanceNum < 0)
		m_Gfx.DrawIndexed(m_pIndexBuffer->GetIndexNum());
	else
		m_Gfx.DrawInstanced(m_pIndexBuffer->GetIndexNum(), InstanceNum);
}

//バインダ登録
void DRAWER::AddBind(std::unique_ptr<BINDER> pBinder)
{
	//例外処理
	if (pBinder == nullptr)
		throw ERROR_EX2("【警告】ポインタはNULLです！");

	//複数インデックスのバインドを防止
	if (typeid(*pBinder) == typeid(INDEX_BUFFER)) {
		if (m_pIndexBuffer != nullptr)
			throw ERROR_EX2("【警告】m_pIndexBufferは上書きされます！");
		m_pIndexBuffer = dynamic_cast<INDEX_BUFFER*>(pBinder.get());
	}

	//頂点バッファのポインタを取得
	if (typeid(*pBinder) == typeid(VERTEX_BUFFER)) {
		if (m_pVertexBuffer != nullptr)
			throw ERROR_EX2("【警告】m_pVertexBufferは上書きされます！");
		m_pVertexBuffer = dynamic_cast<VERTEX_BUFFER*>(pBinder.get());
	}

	//所有権移行
	m_aBinder.push_back(std::move(pBinder));
}

//インデックス数取得
UINT DRAWER::GetIndexNum() const noexcept
{
	return m_pIndexBuffer->GetIndexNum();
}
