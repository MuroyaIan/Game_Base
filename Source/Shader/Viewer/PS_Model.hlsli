/**
 * @file VS_Model.hlsli
 * @brief リソース参照（ビュワー用）
 * @author 室谷イアン
 * @date 2023/01/23
 * @履歴 2023/01/23：ファイル作成
 */

//サンプラー
SamplerState Sampler : register(s0);

//テクスチャ
Texture2D TexMap[3] : register(t0);		//0:Diffuse, 1:Specular, 2:Normal

//定数バッファ（マテリアル）
cbuffer CB_MATERIAL : register(b0)
{
	float4 cbAmbient;		//環境光
	float4 cbDiffuse;		//拡散反射光
	float4 cbEmissive;		//発射光
	float4 cbTransparent;	//透過度
	float4 cbSpecular;		//鏡面反射光
	float cbShininess;		//光沢
	float Pad1;
	float Pad2;
	float Pad3;				//仮置き
}

//グローバル定数（仮の平行光源）
static const float4 LightColor = { 1.0f, 1.0f, 1.0f, 1.0f };		//平行光源の光
static const float4 GlobalAmbient = { 1.0f, 1.0f, 1.0f, 0.2f };		//大域環境光
