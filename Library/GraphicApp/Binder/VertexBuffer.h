/**
 * @file VertexBuffer.h
 * @brief 頂点バッファの処理
 * @author 室谷イアン
 * @date 2022/06/24
 * @履歴 2022/06/24：ファイル作成
 *		 2022/08/11：インスタンシング実装
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Binder/Binder.h>

//===== クラス定義 =====

//***** 頂点バッファ *****
class VERTEX_BUFFER : public BINDER
{
public:

	//バッファタイプ
	enum class VB_TYPE
	{
		VERTEX,		//頂点情報
		INSTANCE,	//インスタンス情報

		MAX_TYPE
	};

	//プロトタイプ宣言
	template<class V>
	VERTEX_BUFFER(GRAPHIC& Gfx, const std::vector<V>& Vertices, bool bDynamic = false) :
		BINDER(), m_aStride(), m_pVertexBuffer(), m_bDynamicBuffer(bDynamic)
	{
		//エラーハンドル
		HRESULT hr{};

		//バッファ作成
		D3D11_BUFFER_DESC bd{};
		bd.ByteWidth = static_cast<UINT>(sizeof(V) * Vertices.size());
		if (!m_bDynamicBuffer)
			bd.Usage = D3D11_USAGE_DEFAULT;
		else
			bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		if (!m_bDynamicBuffer)
			bd.CPUAccessFlags = 0u;
		else
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0u;
		bd.StructureByteStride = static_cast<UINT>(sizeof(V));
		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = Vertices.data();
		hr = GetDevice(Gfx)->CreateBuffer(&bd, &sd, &m_pVertexBuffer[static_cast<int>(VB_TYPE::VERTEX)]);
		ERROR_DX(hr);

		//その他初期化
		m_aStride[static_cast<int>(VB_TYPE::VERTEX)] = static_cast<UINT>(sizeof(V));
	}

	template<class V, class I>
	VERTEX_BUFFER(GRAPHIC& Gfx, const std::vector<V>& Vertices, const std::vector<I>& Instances, bool bDynamic = false) :
		BINDER(), m_aStride(), m_pVertexBuffer(), m_bDynamicBuffer(bDynamic)
	{
		//エラーハンドル
		HRESULT hr{};

		//バッファ作成（頂点情報）
		D3D11_BUFFER_DESC bd1{};
		bd1.ByteWidth = static_cast<UINT>(sizeof(V) * Vertices.size());
		bd1.Usage = D3D11_USAGE_DEFAULT;
		bd1.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd1.CPUAccessFlags = 0u;
		bd1.MiscFlags = 0u;
		bd1.StructureByteStride = static_cast<UINT>(sizeof(V));
		D3D11_SUBRESOURCE_DATA sd1{};
		sd1.pSysMem = Vertices.data();
		hr = GetDevice(Gfx)->CreateBuffer(&bd1, &sd1, &m_pVertexBuffer[static_cast<int>(VB_TYPE::VERTEX)]);
		ERROR_DX(hr);

		//バッファ作成（インスタンス情報）
		int size{};
		if (Instances.size() == 0)
			size = 1;
		else
			size = static_cast<int>(Instances.size());
		D3D11_BUFFER_DESC bd2{};
		bd2.ByteWidth = static_cast<UINT>(sizeof(I) * size);
		bd2.Usage = D3D11_USAGE_DYNAMIC;
		bd2.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd2.MiscFlags = 0u;
		bd2.StructureByteStride = static_cast<UINT>(sizeof(I));
		hr = GetDevice(Gfx)->CreateBuffer(&bd2, nullptr, &m_pVertexBuffer[static_cast<int>(VB_TYPE::INSTANCE)]);
		ERROR_DX(hr);

		//その他初期化
		m_aStride[static_cast<int>(VB_TYPE::VERTEX)] = static_cast<UINT>(sizeof(V));
		m_aStride[static_cast<int>(VB_TYPE::INSTANCE)] = static_cast<UINT>(sizeof(I));
	}
	~VERTEX_BUFFER() noexcept override;

	template<typename D>
	void UpdateBuffer(GRAPHIC& Gfx, const std::vector<D>& aData, VB_TYPE type) const	//バッファ更新
	{
		//例外処理
		if (m_pVertexBuffer[static_cast<int>(type)].Get() == nullptr)
			throw ERROR_EX2("バッファはnullです。");
		if (type == VB_TYPE::VERTEX && !m_bDynamicBuffer)
			throw ERROR_EX2("頂点バッファは動的ではない!");

		//エラーハンドル
		HRESULT hr{};

		//更新処理
		D3D11_MAPPED_SUBRESOURCE msr{};
		hr = GetContext(Gfx)->Map(
			m_pVertexBuffer[static_cast<int>(type)].Get(), 0u,
			D3D11_MAP_WRITE_DISCARD, 0u,
			&msr);																		//GPUのアクセスをロック
		ERROR_DX(hr);
		memcpy(msr.pData, aData.data(), static_cast<UINT>(sizeof(D) * aData.size()));	//データ書込み
		GetContext(Gfx)->Unmap(m_pVertexBuffer[static_cast<int>(type)].Get(), 0u);		//GPUのアクセスを解放
	}

	template<typename I>
	void ResetInstanceBuffer(GRAPHIC& Gfx, const std::vector<I>& Instances)				//インスタンスバッファ再設定
	{
		//エラーハンドル
		HRESULT hr{};

		//インスタンスバッファ解放⇒新しく生成
		int size{};
		if (Instances.size() == 0)
			size = 1;
		else
			size = static_cast<int>(Instances.size());
		D3D11_BUFFER_DESC bd2{};
		bd2.ByteWidth = static_cast<UINT>(sizeof(I) * size);
		bd2.Usage = D3D11_USAGE_DYNAMIC;
		bd2.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd2.MiscFlags = 0u;
		bd2.StructureByteStride = static_cast<UINT>(sizeof(I));
		hr = GetDevice(Gfx)->CreateBuffer(&bd2, nullptr, &m_pVertexBuffer[static_cast<int>(VB_TYPE::INSTANCE)]);
		ERROR_DX(hr);

		//その他初期化
		m_aStride[static_cast<int>(VB_TYPE::INSTANCE)] = static_cast<UINT>(sizeof(I));
	}

	void Bind(GRAPHIC& Gfx) noexcept override;													//バインド処理

protected:

	//変数宣言
	UINT m_aStride[static_cast<int>(VB_TYPE::MAX_TYPE)];										//頂点バッファ配列の要素1個分のサイズ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer[static_cast<int>(VB_TYPE::MAX_TYPE)];	//ポインタ
	bool m_bDynamicBuffer;																		//頂点バッファはダイナミックかどうか
};
