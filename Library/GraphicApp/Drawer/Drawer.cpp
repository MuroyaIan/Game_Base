
//===== インクルード部 =====
#include <GraphicApp/Drawer/Drawer.h>
#include <GraphicApp/Binder/IndexBuffer.h>
#include <GraphicApp/Binder/VertexBuffer.h>

//===== クラス実装 =====
DRAWER::DRAWER() noexcept : m_aBinder(), m_pIndexBuffer(nullptr), m_InstanceNum(0), m_pVertexBuffer(nullptr)
{
}
DRAWER::~DRAWER() noexcept
{
}

//インデックス数取得
UINT DRAWER::GetIndexNum() const noexcept
{
	if (m_pIndexBuffer != nullptr)
		return m_pIndexBuffer->GetIndexNum();
	else
		return GetStaticIndexBuffer().GetIndexNum();
}

//書込み処理
void DRAWER::Draw(GRAPHIC& Gfx, bool bDrawInstance) const noexcept(!IS_DEBUG)
{
	for (auto& b : m_aBinder)										//バインド処理
		b->Bind(Gfx);
	for (auto& sb : GetStaticBind())								//バインド処理（静的）
		sb->Bind(Gfx);

	if (m_pIndexBuffer != nullptr) {
		if (!bDrawInstance)
			Gfx.DrawIndexed(m_pIndexBuffer->GetIndexNum());			//フレームバッファ書込み
		else
			Gfx.DrawInstanced(m_pIndexBuffer->GetIndexNum(), m_InstanceNum);
	}
	else {
		if (!bDrawInstance)
			Gfx.DrawIndexed(GetStaticIndexBuffer().GetIndexNum());	//インデックスバッファ固定の場合
		else
			Gfx.DrawInstanced(GetStaticIndexBuffer().GetIndexNum(), m_InstanceNum);
	}
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
