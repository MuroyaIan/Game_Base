/**
 * @file cbdRef.h
 * @brief 定数バッファデータまとめ
 * @author 室谷イアン
 * @date 2022/12/06
 * @履歴 2022/12/06：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Drawer/VertexShaderData.h>

//===== 構造体宣言 =====
struct CBD_COLOR						//ポリゴン色用データ
{
	//変数宣言
	DirectX::XMFLOAT4 FaceColor[24];	//ポリゴン色（面ごと）

	CBD_COLOR() noexcept
	{
		FaceColor[0] = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		FaceColor[1] = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		FaceColor[2] = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
		FaceColor[3] = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
		FaceColor[4] = DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
		FaceColor[5] = DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
		FaceColor[6] = DirectX::XMFLOAT4(1.0f, 0.5f, 0.0f, 1.0f);
		FaceColor[7] = DirectX::XMFLOAT4(0.5f, 1.0f, 0.0f, 1.0f);
		FaceColor[8] = DirectX::XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f);
		FaceColor[9] = DirectX::XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f);
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

	~CBD_COLOR() noexcept
	{}
};

struct CBD_MTX_VP					//変換行列（VP）
{
	//変数宣言
	DirectX::XMFLOAT4X4 mtxView;	//ビュー行列
	DirectX::XMFLOAT4X4 mtxProj;	//投影行列

	CBD_MTX_VP() noexcept : mtxView(), mtxProj()
	{
		DirectX::XMStoreFloat4x4(&mtxView, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&mtxProj, DirectX::XMMatrixIdentity());
	}

	CBD_MTX_VP(DirectX::XMFLOAT4X4 mtxV, DirectX::XMFLOAT4X4 mtxP) noexcept : mtxView(mtxV), mtxProj(mtxP)
	{}

	~CBD_MTX_VP() noexcept
	{}
};

struct CBD_MTX_LWVP					//変換行列（LWVP）
{
	//変数宣言
	DirectX::XMFLOAT4X4 mtxLocal;	//ローカル行列
	DirectX::XMFLOAT4X4 mtxWorld;	//ワールド行列
	DirectX::XMFLOAT4X4 mtxView;	//ビュー行列
	DirectX::XMFLOAT4X4 mtxProj;	//投影行列

	CBD_MTX_LWVP() noexcept : mtxLocal(), mtxWorld(), mtxView(), mtxProj()
	{
		DirectX::XMStoreFloat4x4(&mtxLocal, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&mtxWorld, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&mtxView, DirectX::XMMatrixIdentity());
		DirectX::XMStoreFloat4x4(&mtxProj, DirectX::XMMatrixIdentity());
	}

	CBD_MTX_LWVP(DirectX::XMFLOAT4X4 mtxL, DirectX::XMFLOAT4X4 mtxW, DirectX::XMFLOAT4X4 mtxV, DirectX::XMFLOAT4X4 mtxP) noexcept :
		mtxLocal(mtxL), mtxWorld(mtxW), mtxView(mtxV), mtxProj(mtxP)
	{}

	~CBD_MTX_LWVP() noexcept
	{}
};

struct CBD_MATERIAL					//マテリアル情報
{
	DirectX::XMFLOAT4 Ambient;		//環境光
	DirectX::XMFLOAT4 Diffuse;		//拡散反射光
	DirectX::XMFLOAT4 Emissive;		//発射光
	DirectX::XMFLOAT4 Transparent;	//透過度
	DirectX::XMFLOAT4 Specular;		//鏡面反射光
	float Shininess;				//光沢
	float Pad1;
	float Pad2;
	float Pad3;						//仮置き

	CBD_MATERIAL() noexcept :
		Ambient(0.0f, 0.0f, 0.0f, 0.0f), Diffuse(0.0f, 0.0f, 0.0f, 0.0f), Emissive(0.0f, 0.0f, 0.0f, 0.0f), Transparent(0.0f, 0.0f, 0.0f, 0.0f),
		Specular(0.0f, 0.0f, 0.0f, 0.0f), Shininess(2.0f), Pad1(0.0f), Pad2(0.0f), Pad3(0.0f)
	{}

	~CBD_MATERIAL() noexcept {}
};

struct CBD_MTX_LOCAL				//変換行列（ローカル）
{
	//変数宣言
	DirectX::XMFLOAT4X4 mtxSkin;	//骨なしメッシュ用ローカル行列

	CBD_MTX_LOCAL() noexcept : mtxSkin()
	{
		DirectX::XMStoreFloat4x4(&mtxSkin, DirectX::XMMatrixIdentity());
	}

	~CBD_MTX_LOCAL() noexcept
	{}
};

struct CBD_BONE								//骨データ
{
	//変数宣言
	DirectX::XMFLOAT4X4 mtxBone[MAX_BONE];	//骨のワールド行列

	CBD_BONE() noexcept : mtxBone()
	{
		for (auto& b : mtxBone)
			DirectX::XMStoreFloat4x4(&b, DirectX::XMMatrixIdentity());
	}

	~CBD_BONE() noexcept
	{}
};
