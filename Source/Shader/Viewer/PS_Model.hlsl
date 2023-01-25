/**
 * @file PS_Model.hlsl
 * @brief 頂点シェーダ（モデル実装）
 * @author 室谷イアン
 * @date 2023/01/19
 * @履歴 2023/01/19：ファイル作成
 */

//インクルード部
#include <PS_Model.hlsli>

//入力用構造体
struct PS_IN
{
	float2 tex : TEXCOORD;				//UV座標

	float3 vNorV_Model : NORMAL;		//法線（ビュー空間）
	float3 vNorV_ToCamera : CAM_NOR;	//カメラへの単位ベクトル（ビュー空間）
	float3 vDirV_ToLight : LIGHT_DIR;	//光への方向ベクトル（ビュー空間）
	float3 vNorV_ToLight : LIGHT_NOR;	//光への単位ベクトル（ビュー空間）
};

//プロトタイプ宣言
float3 CalcDirectionalLight(PS_IN psi, float3 ModelNormal, float3x3 ModelColor);	//平行光源の計算

//エントリーポイント
float4 main(PS_IN psi) : SV_TARGET
{
	//テクスチャ取得
	const float2x4 Texture = {
		TexMap[0].Sample(Sampler, psi.tex),		//Diffuse
		TexMap[1].Sample(Sampler, psi.tex),		//Specular
	};

	//モデル色計算
	const float3x3 ModelColor = {
		cbDiffuse.rgb * cbDiffuse.w * Texture._11_12_13,	//Diffuse
		cbSpecular.rgb * Texture._21_22_23,					//Specular
		cbAmbient.rgb										//Ambient
	};

	//平行光源の計算
	const float3 Directional = CalcDirectionalLight(psi, psi.vNorV_Model, ModelColor);

	//グローバル環境光の計算
	const float3 g_Ambient = GlobalAmbient.rgb * GlobalAmbient.a * ModelColor._11_12_13;

	//最終の出力色計算
	return float4(saturate(Directional + g_Ambient), 1.0f);
}

//平行光源の計算
float3 CalcDirectionalLight(PS_IN psi, float3 ModelNormal, float3x3 ModelColor)
{
	//平行光源の色
	const float3 LightRGB = LightColor.rgb * LightColor.a;

	//拡散色算出
	const float3 Diffuse = LightRGB * max(0.0f, dot(psi.vNorV_ToLight, ModelNormal)) * ModelColor._11_12_13;

	//鏡面反射色算出
	const float3 vRef = ModelNormal * dot(psi.vDirV_ToLight, ModelNormal) * 2.0f - psi.vDirV_ToLight;	//鏡面反射ベクトル
	//const float PowerS = pow(2.0f, Texture._24 * 13.0f);												//αチャンネル付きスペキュラマップ?
	const float3 Specular = LightRGB * pow(max(0.0f, dot(normalize(vRef), psi.vNorV_ToCamera)), cbShininess) * ModelColor._21_22_23;

	//環境光の計算
	const float3 Ambient = LightRGB * ModelColor._31_32_33;

	//最終の出力色計算
	return Diffuse + Specular + Ambient;
}
