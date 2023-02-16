/**
 * @file VS_Model.hlsli
 * @brief リソース参照（ビュワー用）
 * @author 室谷イアン
 * @date 2023/01/23
 * @履歴 2023/01/23：ファイル作成
 */

//インクルード部
#include <../Light.hlsli>
#include <../Phong.hlsli>

//サンプラー
SamplerState Sampler : register(s0);

//テクスチャ
Texture2D TexMap[4] : register(t0);		//0:Diffuse, 1:Specular, 2:Normal, 3:Displacement

//定数バッファ（マテリアル）
cbuffer CB_MATERIAL : register(b0)
{
	MATERIAL_DATA matData;
}

//定数バッファ（ライト情報）
cbuffer CB_LIGHT : register(b1)
{
	LIGHT_DIRECTIONAL DirectionalLight;		//平行光源
	LIGHT_POINT PointLight[16];				//点光源
	float4 AmbientLight;					//環境光
}
