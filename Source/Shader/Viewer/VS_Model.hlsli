/**
 * @file VS_Model.hlsli
 * @brief リソース参照（ビュワー用）
 * @author 室谷イアン
 * @date 2023/01/23
 * @履歴 2023/01/23：ファイル作成
 */

//インクルード部
#include <../Light.hlsli>

//定数バッファ（変換行列）
cbuffer CB_MTX_LWVP : register(b0)
{
	matrix mtxLocal;	//ローカル行列（骨なしメッシュの変換）
	matrix mtxWorld;	//ワールド行列
	matrix mtxView;		//ビュー行列
	matrix mtxProj;		//投影行列
};

//定数バッファ（骨情報）
cbuffer CB_BONE : register(b1)
{
	matrix mtxBone[250];	//骨行列
};

//定数バッファ（光源情報）
cbuffer CB_LIGHT : register(b2)
{
	LIGHT_DIRECTIONAL DirectionalLight;		//平行光源
	LIGHT_POINT PointLight[16];				//点光源
	float4 AmbientLight;					//環境光
}
