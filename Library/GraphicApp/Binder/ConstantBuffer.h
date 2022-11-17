/**
 * @file ConstantBuffer.h
 * @brief 定数バッファの処理
 * @author 室谷イアン
 * @date 2022/06/25
 * @履歴 2022/06/25：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Binder/Binder.h>
#include <GraphicApp/Drawer/VertexShaderData.h>

//===== 列挙型宣言 =====
enum class CB_SLOT_VS
{
	DEFAULT,
	CAMERA,
	BONE,
	LOCAL
};

enum class CB_SLOT_PS
{
	DEFAULT,
	LIGHT,
	MATERIAL
};

//===== 構造体宣言 =====
struct CB_COLOR							//カラー用定数バッファ
{
	//変数宣言
	DirectX::XMFLOAT4 FaceColor[24];	//ポリゴン色（面情報）

	CB_COLOR() noexcept
	{
		FaceColor[0]  = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		FaceColor[1]  = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		FaceColor[2]  = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
		FaceColor[3]  = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
		FaceColor[4]  = DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
		FaceColor[5]  = DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
		FaceColor[6]  = DirectX::XMFLOAT4(1.0f, 0.5f, 0.0f, 1.0f);
		FaceColor[7]  = DirectX::XMFLOAT4(0.5f, 1.0f, 0.0f, 1.0f);
		FaceColor[8]  = DirectX::XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f);
		FaceColor[9]  = DirectX::XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f);
		FaceColor[10] = DirectX::XMFLOAT4(0.0f, 1.0f, 0.5f, 1.0f);
		FaceColor[11] = DirectX::XMFLOAT4(0.0f, 0.5f, 1.0f, 1.0f);
		FaceColor[12] = DirectX::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f);
		FaceColor[13] = DirectX::XMFLOAT4(0.7f, 1.0f, 0.0f, 1.0f);
		FaceColor[14] = DirectX::XMFLOAT4(1.0f, 0.0f, 0.7f, 1.0f);
		FaceColor[15] = DirectX::XMFLOAT4(0.7f, 0.0f, 1.0f, 1.0f);
		FaceColor[16] = DirectX::XMFLOAT4(0.0f, 1.0f, 0.7f, 1.0f);
		FaceColor[17] = DirectX::XMFLOAT4(0.0f, 0.7f, 1.0f, 1.0f);
		FaceColor[18] = DirectX::XMFLOAT4(1.0f, 0.3f, 0.0f, 1.0f);
		FaceColor[19] = DirectX::XMFLOAT4(0.3f, 1.0f, 0.0f, 1.0f);
		FaceColor[20] = DirectX::XMFLOAT4(1.0f, 0.0f, 0.3f, 1.0f);
		FaceColor[21] = DirectX::XMFLOAT4(0.3f, 0.0f, 1.0f, 1.0f);
		FaceColor[22] = DirectX::XMFLOAT4(0.0f, 1.0f, 0.3f, 1.0f);
		FaceColor[23] = DirectX::XMFLOAT4(0.0f, 0.3f, 1.0f, 1.0f);
	}
	~CB_COLOR() noexcept
	{}
};

struct CB_MTX_V_P					//変換行列用定数バッファ
{
	//変数宣言
	DirectX::XMFLOAT4X4 mtxView;	//ビュー行列
	DirectX::XMFLOAT4X4 mtxProj;	//投影行列

	CB_MTX_V_P() noexcept : mtxView(), mtxProj()
	{
		DirectX::XMStoreFloat4x4(&mtxView, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&mtxProj, DirectX::XMMatrixIdentity());
	}
	CB_MTX_V_P(DirectX::XMFLOAT4X4 mtxV, DirectX::XMFLOAT4X4 mtxP) noexcept : mtxView(mtxV), mtxProj(mtxP)
	{}
	~CB_MTX_V_P() noexcept
	{}
};

struct CB_MTX_L_W_V_P					//変換行列用定数バッファ
{
	//変数宣言
	DirectX::XMFLOAT4X4 mtxLocal;	//ローカル行列
	DirectX::XMFLOAT4X4 mtxWorld;	//ワールド行列
	DirectX::XMFLOAT4X4 mtxView;	//ビュー行列
	DirectX::XMFLOAT4X4 mtxProj;	//投影行列

	CB_MTX_L_W_V_P() noexcept : mtxLocal(), mtxWorld(), mtxView(), mtxProj()
	{
		DirectX::XMStoreFloat4x4(&mtxLocal, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&mtxWorld, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&mtxView, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&mtxProj, DirectX::XMMatrixIdentity());
	}
	CB_MTX_L_W_V_P(DirectX::XMFLOAT4X4 mtxL, DirectX::XMFLOAT4X4 mtxW, DirectX::XMFLOAT4X4 mtxV, DirectX::XMFLOAT4X4 mtxP) noexcept : mtxLocal(mtxL), mtxWorld(mtxW), mtxView(mtxV), mtxProj(mtxP)
	{}
	~CB_MTX_L_W_V_P() noexcept
	{}
};

struct CB_MTX_BONE							//骨行列用定数バッファ
{
	//変数宣言
	DirectX::XMFLOAT4X4 mtxBone[MAX_BONE];	//ワールド行列

	CB_MTX_BONE() noexcept : mtxBone()
	{
		for (auto& b : mtxBone)
			DirectX::XMStoreFloat4x4(&b, DirectX::XMMatrixIdentity());
	}
	~CB_MTX_BONE() noexcept
	{}
};

struct CB_MTX_LOCAL							//骨行列用定数バッファ
{
	//変数宣言
	DirectX::XMFLOAT4X4 mtxSkin;			//骨なしメッシュ用ローカル行列

	CB_MTX_LOCAL() noexcept : mtxSkin()
	{
		DirectX::XMStoreFloat4x4(&mtxSkin, DirectX::XMMatrixIdentity());
	}
	~CB_MTX_LOCAL() noexcept
	{}
};

//===== クラス定義 =====

//***** 定数バッファ *****
template<typename C>
class CONSTANT_BUFFER : public BINDER
{
public:

	//プロトタイプ宣言
	CONSTANT_BUFFER(GRAPHIC& Gfx, const C& Consts, UINT Slot = 0u) :
		BINDER(), m_pConstantBuffer(), m_Slot(Slot)
	{
		//エラーハンドル
		HRESULT hr{};

		//バッファ作成
		D3D11_BUFFER_DESC cbd{};
		cbd.ByteWidth = static_cast<UINT>(sizeof(Consts));
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.StructureByteStride = 0u;
		D3D11_SUBRESOURCE_DATA csd{};
		csd.pSysMem = &Consts;
		hr = GetDevice(Gfx)->CreateBuffer(&cbd, &csd, &m_pConstantBuffer);
		ERROR_DX(hr);
	}
	CONSTANT_BUFFER(GRAPHIC& Gfx, UINT Slot = 0u) :
		BINDER(), m_pConstantBuffer(), m_Slot(Slot)		//バッファ初期化なし
	{
		//エラーハンドル
		HRESULT hr{};

		//バッファ作成
		D3D11_BUFFER_DESC cbd{};
		cbd.ByteWidth = static_cast<UINT>(sizeof(C));
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.StructureByteStride = 0u;
		hr = GetDevice(Gfx)->CreateBuffer(&cbd, nullptr, &m_pConstantBuffer);
		ERROR_DX(hr);
	}
	~CONSTANT_BUFFER() noexcept override {}

	void Update(GRAPHIC& Gfx, const C& Consts) const	//バッファ更新
	{
		//エラーハンドル
		HRESULT hr{};

		//更新処理
		D3D11_MAPPED_SUBRESOURCE msr{};
		hr = GetContext(Gfx)->Map(
			m_pConstantBuffer.Get(), 0u,
			D3D11_MAP_WRITE_DISCARD, 0u,
			&msr);												//GPUのアクセスをロック
		ERROR_DX(hr);
		memcpy(msr.pData, &Consts, sizeof(Consts));				//データ書込み
		GetContext(Gfx)->Unmap(m_pConstantBuffer.Get(), 0u);	//GPUのアクセスを解放
	}

protected:

	//変数宣言
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;		//ポインタ
	UINT m_Slot;												//入力スロット
};

//***** 定数バッファ（頂点シェーダ用） *****
template<typename C>
class VERTEX_CBUFFER : public CONSTANT_BUFFER<C>
{
public:

	//プロトタイプ宣言
	VERTEX_CBUFFER(GRAPHIC& Gfx, const C& Consts, UINT Slot = 0u) : C_BUFF::CONSTANT_BUFFER(Gfx, Consts, Slot) {}
	VERTEX_CBUFFER(GRAPHIC& Gfx, UINT Slot = 0u) : C_BUFF::CONSTANT_BUFFER(Gfx, Slot) {}
	~VERTEX_CBUFFER() noexcept override {}

	void Bind(GRAPHIC& Gfx) noexcept override	//バインド処理
	{
		BINDER::GetContext(Gfx)->VSSetConstantBuffers(C_BUFF::m_Slot, 1u, C_BUFF::m_pConstantBuffer.GetAddressOf());
	}

private:

	//テンプレート対策
	using C_BUFF = CONSTANT_BUFFER<C>;
};

//***** 定数バッファ（ピクセルシェーダ用） *****
template<typename C>
class PIXEL_CBUFFER : public CONSTANT_BUFFER<C>
{
public:

	//プロトタイプ宣言
	PIXEL_CBUFFER(GRAPHIC& Gfx, const C& Consts, UINT Slot = 0u) : C_BUFF::CONSTANT_BUFFER(Gfx, Consts, Slot) {}
	PIXEL_CBUFFER(GRAPHIC& Gfx, UINT Slot = 0u) : C_BUFF::CONSTANT_BUFFER(Gfx, Slot) {}
	~PIXEL_CBUFFER() noexcept override {}

	void Bind(GRAPHIC& Gfx) noexcept override	//バインド処理
	{
		BINDER::GetContext(Gfx)->PSSetConstantBuffers(C_BUFF::m_Slot, 1u, C_BUFF::m_pConstantBuffer.GetAddressOf());
	}

private:

	//テンプレート対策
	using C_BUFF = CONSTANT_BUFFER<C>;
};
