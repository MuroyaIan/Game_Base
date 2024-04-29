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
struct CBD_COLOR					//ポリゴン色用データ
{
	//変数宣言
	dx::XMFLOAT4 ms_FaceColor[24];	//ポリゴン色（面ごと）

	CBD_COLOR() noexcept
	{
		ms_FaceColor[0]  = dx::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		ms_FaceColor[1]  = dx::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		ms_FaceColor[2]  = dx::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
		ms_FaceColor[3]  = dx::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
		ms_FaceColor[4]  = dx::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
		ms_FaceColor[5]  = dx::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
		ms_FaceColor[6]  = dx::XMFLOAT4(1.0f, 0.5f, 0.0f, 1.0f);
		ms_FaceColor[7]  = dx::XMFLOAT4(0.5f, 1.0f, 0.0f, 1.0f);
		ms_FaceColor[8]  = dx::XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f);
		ms_FaceColor[9]  = dx::XMFLOAT4(0.5f, 0.0f, 1.0f, 1.0f);
		ms_FaceColor[10] = dx::XMFLOAT4(0.0f, 1.0f, 0.5f, 1.0f);
		ms_FaceColor[11] = dx::XMFLOAT4(0.0f, 0.5f, 1.0f, 1.0f);
		ms_FaceColor[12] = dx::XMFLOAT4(1.0f, 0.7f, 0.0f, 1.0f);
		ms_FaceColor[13] = dx::XMFLOAT4(0.7f, 1.0f, 0.0f, 1.0f);
		ms_FaceColor[14] = dx::XMFLOAT4(1.0f, 0.0f, 0.7f, 1.0f);
		ms_FaceColor[15] = dx::XMFLOAT4(0.7f, 0.0f, 1.0f, 1.0f);
		ms_FaceColor[16] = dx::XMFLOAT4(0.0f, 1.0f, 0.7f, 1.0f);
		ms_FaceColor[17] = dx::XMFLOAT4(0.0f, 0.7f, 1.0f, 1.0f);
		ms_FaceColor[18] = dx::XMFLOAT4(1.0f, 0.3f, 0.0f, 1.0f);
		ms_FaceColor[19] = dx::XMFLOAT4(0.3f, 1.0f, 0.0f, 1.0f);
		ms_FaceColor[20] = dx::XMFLOAT4(1.0f, 0.0f, 0.3f, 1.0f);
		ms_FaceColor[21] = dx::XMFLOAT4(0.3f, 0.0f, 1.0f, 1.0f);
		ms_FaceColor[22] = dx::XMFLOAT4(0.0f, 1.0f, 0.3f, 1.0f);
		ms_FaceColor[23] = dx::XMFLOAT4(0.0f, 0.3f, 1.0f, 1.0f);
	}

	~CBD_COLOR() noexcept = default;
};

struct CBD_MTX_VP				//変換行列（VP）
{
	//変数宣言
	dx::XMFLOAT4X4 ms_MtxView;	//ビュー行列
	dx::XMFLOAT4X4 ms_MtxProj;	//投影行列

	CBD_MTX_VP() noexcept : ms_MtxView(), ms_MtxProj()
	{
		dx::XMStoreFloat4x4(&ms_MtxView, dx::XMMatrixIdentity());
		dx::XMStoreFloat4x4(&ms_MtxProj, dx::XMMatrixIdentity());
	}

	CBD_MTX_VP(dx::XMFLOAT4X4 mtxV, dx::XMFLOAT4X4 mtxP) noexcept : ms_MtxView(mtxV), ms_MtxProj(mtxP)
	{}

	~CBD_MTX_VP() noexcept = default;
};

struct CBD_MTX_LWVP					//変換行列（LWVP）
{
	//変数宣言
	dx::XMFLOAT4X4 ms_MtxLocal;		//ローカル行列
	dx::XMFLOAT4X4 ms_MtxWorld;		//ワールド行列
	dx::XMFLOAT4X4 ms_MtxView;		//ビュー行列
	dx::XMFLOAT4X4 ms_MtxProj;		//投影行列

	CBD_MTX_LWVP() noexcept : ms_MtxLocal(), ms_MtxWorld(), ms_MtxView(), ms_MtxProj()
	{
		dx::XMStoreFloat4x4(&ms_MtxLocal, dx::XMMatrixIdentity());
		dx::XMStoreFloat4x4(&ms_MtxWorld, dx::XMMatrixIdentity());
		dx::XMStoreFloat4x4(&ms_MtxView, dx::XMMatrixIdentity());
		dx::XMStoreFloat4x4(&ms_MtxProj, dx::XMMatrixIdentity());
	}

	CBD_MTX_LWVP(dx::XMFLOAT4X4 mtxL, dx::XMFLOAT4X4 mtxW, dx::XMFLOAT4X4 mtxV, dx::XMFLOAT4X4 mtxP) noexcept :
		ms_MtxLocal(mtxL), ms_MtxWorld(mtxW), ms_MtxView(mtxV), ms_MtxProj(mtxP)
	{}

	~CBD_MTX_LWVP() noexcept = default;
};

struct CBD_MATERIAL					//マテリアル情報
{
	dx::XMFLOAT4 ms_Ambient;		//環境光
	dx::XMFLOAT4 ms_Diffuse;		//拡散反射光
	dx::XMFLOAT4 ms_Emissive;		//発射光
	dx::XMFLOAT4 ms_Transparent;	//透過度
	dx::XMFLOAT4 ms_Specular;		//鏡面反射光
	float ms_Shininess;				//光沢
	float ms_Disp_MinLayerNum;
	float ms_Disp_MaxLayerNum;		//視差マップのサンプリング範囲
	float ms_Disp_DepthScale;		//視差マップの深さ係数

	CBD_MATERIAL() noexcept :
		ms_Ambient(0.0f, 0.0f, 0.0f, 0.0f), ms_Diffuse(0.0f, 0.0f, 0.0f, 0.0f), ms_Emissive(0.0f, 0.0f, 0.0f, 0.0f), ms_Transparent(0.0f, 0.0f, 0.0f, 0.0f),
		ms_Specular(0.0f, 0.0f, 0.0f, 0.0f), ms_Shininess(2.0f), ms_Disp_MinLayerNum(1.0f), ms_Disp_MaxLayerNum(2.0f), ms_Disp_DepthScale(0.0f)
	{}

	~CBD_MATERIAL() noexcept = default;
};

struct CBD_BONE								//骨データ
{
	//変数宣言
	dx::XMFLOAT4X4 ms_MtxBone[MAX_BONE];	//骨のワールド行列

	CBD_BONE() noexcept : ms_MtxBone()
	{
		for (auto& l_MtxBone : ms_MtxBone)
			dx::XMStoreFloat4x4(&l_MtxBone, dx::XMMatrixIdentity());
	}

	~CBD_BONE() noexcept = default;
};
