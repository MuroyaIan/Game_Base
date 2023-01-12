/**
 * @file ConstantBuffer.h
 * @brief 定数バッファの処理
 * @author 室谷イアン
 * @date 2022/06/25
 * @履歴 2022/06/25：ファイル作成
 *		 2022/11/24：処理改善
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Binder/Binder.h>
#include <GraphicApp/Binder/cbdRef.h>

//===== 列挙型宣言 =====
enum class CB_SLOT_VS
{
	Default,
	Camera,		//カメラ
	Bone,		//骨
	Local		//ローカル行列
};

enum class CB_SLOT_PS
{
	Default,
	Light,		//光
	Material	//マテリアル
};

//===== 列挙型宣言 =====
struct CB_PTR									//定数バッファのポインタ情報
{
	//変数宣言
	std::vector<ID3D11Buffer*> m_aBuffPtrVS;
	std::vector<ID3D11Buffer*> m_aBuffPtrPS;	//ポインタ配列

	CB_PTR() noexcept : m_aBuffPtrVS(0), m_aBuffPtrPS(0)
	{}

	~CB_PTR() noexcept
	{}
};

//===== クラス定義 =====

//***** 定数バッファ *****
template<typename C>
class CONSTANT_BUFFER : public BINDER
{
public:

	//プロトタイプ宣言
	explicit CONSTANT_BUFFER(const GRAPHIC& Gfx, const C& Consts, CB_PTR* cbPtr, bool SlotVS = false, bool SlotPS = false) :
		BINDER(), m_pConstantBuffer(), m_bSlotVS(SlotVS), m_bSlotPS(SlotPS)
	{
		//エラーハンドル
		HRESULT hr{};

		//バッファ作成
		D3D11_BUFFER_DESC bd = GetBufferDesc(static_cast<UINT>(sizeof(C)));
		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = &Consts;
		hr = GetDevice(Gfx)->CreateBuffer(&bd, &sd, &m_pConstantBuffer);
		ERROR_DX(hr);

		//ポインタ参照を設定
		SetBuffPtr(cbPtr);
	}

	explicit CONSTANT_BUFFER(const GRAPHIC& Gfx, CB_PTR* cbPtr, bool SlotVS = false, bool SlotPS = false) :
		BINDER(), m_pConstantBuffer(), m_bSlotVS(SlotVS), m_bSlotPS(SlotPS)				//バッファ初期化なし
	{
		//エラーハンドル
		HRESULT hr{};

		//バッファ作成
		D3D11_BUFFER_DESC bd = GetBufferDesc(static_cast<UINT>(sizeof(C)));
		hr = GetDevice(Gfx)->CreateBuffer(&bd, nullptr, &m_pConstantBuffer);
		ERROR_DX(hr);

		//ポインタ参照を設定
		if (cbPtr != nullptr) {
			if (m_bSlotVS)
				cbPtr->m_aBuffPtrVS.push_back(m_pConstantBuffer.Get());
			if (m_bSlotPS)
				cbPtr->m_aBuffPtrPS.push_back(m_pConstantBuffer.Get());
		}
	}

	~CONSTANT_BUFFER() noexcept override {}

	void Update(const GRAPHIC& Gfx, const C& Consts) const	//バッファ更新
	{
		MapBuffer(Gfx, Consts, m_pConstantBuffer.Get());
	}

	void SetBuffPtr(CB_PTR* cbPtr) const noexcept			//バッファポインタ設定
	{
		if (cbPtr != nullptr) {
			if (m_bSlotVS)
				cbPtr->m_aBuffPtrVS.push_back(m_pConstantBuffer.Get());
			if (m_bSlotPS)
				cbPtr->m_aBuffPtrPS.push_back(m_pConstantBuffer.Get());
		}
	}

	void Bind(const GRAPHIC& Gfx) const noexcept override	//バインド処理
	{
		(void)Gfx;
	}

private:

	//変数宣言
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;		//ポインタ
	bool m_bSlotVS;												//スロット有効(VS)
	bool m_bSlotPS;												//スロット有効(PS)

	//プロトタイプ宣言
	D3D11_BUFFER_DESC GetBufferDesc(UINT ArraySize)				//バッファ設定取得
	{
		D3D11_BUFFER_DESC bd{};
		bd.ByteWidth = ArraySize;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0u;
		bd.StructureByteStride = 0u;

		return bd;
	}
};

//***** 定数バッファマネージャ *****
class CBUFF_MGR : public BINDER
{
public:

	//プロトタイプ宣言
	explicit CBUFF_MGR(CB_PTR& cbPtrRef) noexcept :
		m_aBuffPtrVS(std::move(cbPtrRef.m_aBuffPtrVS)), m_aBuffPtrPS(std::move(cbPtrRef.m_aBuffPtrPS)),
		m_BuffSizeVS(static_cast<UINT>(m_aBuffPtrVS.size())), m_BuffSizePS(static_cast<UINT>(m_aBuffPtrPS.size()))
	{}

	~CBUFF_MGR() noexcept override {}

	void Bind(const GRAPHIC& Gfx) const noexcept override	//バインド処理
	{
		//それぞれのシェーダへバインドする
		if (m_BuffSizeVS > 0u)
			BINDER::GetContext(Gfx)->VSSetConstantBuffers(0u, m_BuffSizeVS, m_aBuffPtrVS.data());
		if (m_BuffSizePS > 0u)
			BINDER::GetContext(Gfx)->PSSetConstantBuffers(0u, m_BuffSizePS, m_aBuffPtrPS.data());
	}

private:

	//変数宣言
	std::vector<ID3D11Buffer*> m_aBuffPtrVS;
	std::vector<ID3D11Buffer*> m_aBuffPtrPS;	//定数バッファのポインタ配列
	UINT m_BuffSizeVS;
	UINT m_BuffSizePS;							//定数バッファのサイズ
};
