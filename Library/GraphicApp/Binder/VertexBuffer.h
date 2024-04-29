/**
 * @file VertexBuffer.h
 * @brief 頂点バッファの処理
 * @author 室谷イアン
 * @date 2022/06/24
 * @履歴 2022/06/24：ファイル作成
 *		 2022/08/11：インスタンシング実装
 *		 2022/11/21：処理改善
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Binder/Binder.h>

//===== クラス定義 =====

//***** 頂点バッファ *****
class VERTEX_BUFFER : public CT_BINDER
{
public:

	//バッファタイプ
	enum class VB_TYPE
	{
		Vertex,		//頂点情報
		Instance,	//インスタンス情報

		MaxType
	};

	//プロトタイプ宣言
	template<class V>
	explicit VERTEX_BUFFER(const CT_GRAPHIC& Gfx, const std::vector<V>& aVertice, bool bDynamic = false) :
		CT_BINDER(), m_pVertexBuffers(static_cast<int>(VB_TYPE::MaxType)), m_bDynamicBuffer(bDynamic), m_aStride(static_cast<int>(VB_TYPE::MaxType))
	{
		//エラーハンドル
		HRESULT hr{};

		//バッファ作成
		D3D11_BUFFER_DESC bd = GetBufferDesc(
			static_cast<UINT>(sizeof(V) * aVertice.size()),
			static_cast<UINT>(sizeof(V)),
			m_bDynamicBuffer
		);
		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = aVertice.data();
		hr = GetDevice(Gfx)->CreateBuffer(&bd, &sd, &m_pVertexBuffers[static_cast<int>(VB_TYPE::Vertex)]);
		ERROR_DX(hr);

		//その他初期化
		m_aStride[static_cast<int>(VB_TYPE::Vertex)] = static_cast<UINT>(sizeof(V));
	}

	template<class V, class I>
	explicit VERTEX_BUFFER(const CT_GRAPHIC& Gfx, const std::vector<V>& aVertice, const std::vector<I>& aInstance, bool bDynamic = false) :
		CT_BINDER(), m_pVertexBuffers(static_cast<int>(VB_TYPE::MaxType)), m_bDynamicBuffer(bDynamic), m_aStride(static_cast<int>(VB_TYPE::MaxType))
	{
		//エラーハンドル
		HRESULT hr{};

		//バッファ作成（頂点情報）
		D3D11_BUFFER_DESC bd1 = GetBufferDesc(
			static_cast<UINT>(sizeof(V) * aVertice.size()),
			static_cast<UINT>(sizeof(V)),
			m_bDynamicBuffer
		);
		D3D11_SUBRESOURCE_DATA sd1{};
		sd1.pSysMem = aVertice.data();
		hr = GetDevice(Gfx)->CreateBuffer(&bd1, &sd1, &m_pVertexBuffers[static_cast<int>(VB_TYPE::Vertex)]);
		ERROR_DX(hr);

		//バッファ作成（インスタンス情報）
		int size{};
		if (aInstance.size() == 0)
			size = 1;
		else
			size = static_cast<int>(aInstance.size());
		D3D11_BUFFER_DESC bd2 = GetBufferDesc(
			static_cast<UINT>(sizeof(I) * size),
			static_cast<UINT>(sizeof(I)),
			true
		);
		hr = GetDevice(Gfx)->CreateBuffer(&bd2, nullptr, &m_pVertexBuffers[static_cast<int>(VB_TYPE::Instance)]);
		ERROR_DX(hr);

		//その他初期化
		m_aStride[static_cast<int>(VB_TYPE::Vertex)] = static_cast<UINT>(sizeof(V));
		m_aStride[static_cast<int>(VB_TYPE::Instance)] = static_cast<UINT>(sizeof(I));
	}
	~VERTEX_BUFFER() noexcept override;

	template<typename D>
	void UpdateBuffer(const CT_GRAPHIC& Gfx, const std::vector<D>& aData, VB_TYPE Type) const	//バッファ更新
	{
		//例外処理
		if (m_pVertexBuffers[static_cast<int>(Type)].Get() == nullptr)
			throw ERROR_EX2("バッファはnullです。");
		if (Type == VB_TYPE::Vertex && !m_bDynamicBuffer)
			throw ERROR_EX2("頂点バッファは動的ではない!");

		MapBuffer(Gfx, aData, m_pVertexBuffers[static_cast<int>(Type)].Get());
	}

	template<typename I>
	void ResetInstanceBuffer(const CT_GRAPHIC& Gfx, const std::vector<I>& aInstance)	//インスタンスバッファ再設定
	{
		//エラーハンドル
		HRESULT hr{};

		//インスタンスバッファ解放⇒新しく生成
		int size{};
		if (aInstance.size() == 0)
			size = 1;
		else
			size = static_cast<int>(aInstance.size());
		D3D11_BUFFER_DESC bd2 = GetBufferDesc(
			static_cast<UINT>(sizeof(I) * size),
			static_cast<UINT>(sizeof(I)),
			true
		);
		hr = GetDevice(Gfx)->CreateBuffer(&bd2, nullptr, &m_pVertexBuffers[static_cast<int>(VB_TYPE::Instance)]);
		ERROR_DX(hr);

		//その他初期化
		m_aStride[static_cast<int>(VB_TYPE::Instance)] = static_cast<UINT>(sizeof(I));
	}

	void Bind(const CT_GRAPHIC& Gfx) const override;	//バインド処理

private:

	//変数宣言
	std::vector<ComPtr<ID3D11Buffer>> m_pVertexBuffers;		//ポインタ配列
	bool m_bDynamicBuffer;									//頂点バッファはダイナミックかどうか
	std::vector<UINT> m_aStride;							//頂点バッファ配列の要素1個分のサイズの配列

	//プロトタイプ宣言
	D3D11_BUFFER_DESC GetBufferDesc(UINT ArraySize, UINT TypeSize, bool bDynamic) const noexcept;	//バッファ設定取得
};
