/**
 * @file PS_Phong.hlsl
 * @brief 頂点シェーダ（Phongモデル）
 * @author 室谷イアン
 * @date 2023/02/17
 * @履歴 2023/02/17：ファイル作成
 */

//インクルード部
#include <Light.hlsli>
#include <Phong.hlsli>

//入力用構造体
struct PS_IN
{
	float3 posWV : POSITION;			//座標（ビュー空間）
	float2 tex : TEXCOORD;				//UV座標

	float3 vNorV_Model : NORMAL;		//法線（ビュー空間）
	float3 vNorV_ToCamera : CAM_NOR;	//カメラへの単位ベクトル（ビュー空間）
	matrix mtxView : MTX_V;				//ビュー行列
};

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

//プロトタイプ宣言
float3 CalcPointLightLoop(LIGHT_VECTOR_PT LightVec, float3x3 ModelColor, float Shininess, float3 PosWV, float4x4 mtxView); //点光源の計算

//エントリーポイント
float4 main(PS_IN psi) : SV_Target
{
	//テクスチャ取得（左手系）
	const float2x4 Texture = {
		TexMap[0].Sample(Sampler, psi.tex),		//Diffuse
		TexMap[1].Sample(Sampler, psi.tex),		//Specular
	};

	//モデル色計算
	const float3x3 ModelColor = {
		matData.Diffuse.rgb * matData.Diffuse.w * Texture._11_12_13,	//Diffuse
		matData.Specular.rgb * Texture._21_22_23,						//Specular
		matData.Ambient.rgb												//Ambient
	};

	//平行光源の計算
	const float3 PosL = {
		DirectionalLight.Pos.x,
		DirectionalLight.Pos.y,
		DirectionalLight.Pos.z
	};
	const float3 vDirV_ToLight = mul(PosL, (float3x3) psi.mtxView);		//鏡面反射用、疑似的に位置を設定
	const float3 vNorV_ToLight = normalize(vDirV_ToLight);				//平行光源への単位ベクトル
	const LIGHT_VECTOR LightVec = {
		psi.vNorV_Model,
		psi.vNorV_ToCamera,
		vDirV_ToLight,
		vNorV_ToLight
	};
	const float3 LightColor = DirectionalLight.Color_D.rgb * DirectionalLight.Color_D.a;
	const float3 Directional = CalcDirectionalLight(LightVec, ModelColor, LightColor, matData.Shininess);

	//点光源の計算
	const LIGHT_VECTOR_PT LightVecPt = {
		psi.vNorV_Model,
		psi.vNorV_ToCamera,
	};
	const float3 Point = CalcPointLightLoop(LightVecPt, ModelColor, matData.Shininess, psi.posWV, psi.mtxView);

	//グローバル環境光の計算
	const float3 g_Ambient = AmbientLight.rgb * AmbientLight.a * ModelColor._11_12_13;

	//最終の出力色計算
	return float4(saturate(Directional + Point + g_Ambient), 1.0f);
}

//点光源の計算
float3 CalcPointLightLoop(LIGHT_VECTOR_PT LightVec, float3x3 ModelColor, float Shininess, float3 PosWV, float4x4 mtxView)
{
	//出力用変数
	float3 Color = float3(0.0f, 0.0f, 0.0f);

	//計算ループ
	[unroll]
	for (int i = 0; i < 16; i++) {

		//点光源の色を加算
		LIGHT_POINT LightPt = {
			PointLight[i].Pos,
			PointLight[i].Color_D,
			PointLight[i].AttConst,
			PointLight[i].AttLinear,
			PointLight[i].AttQuadratic,
			PointLight[i].Pad
		};
		Color += CalcPointLight(LightVec, ModelColor, LightPt, Shininess, PosWV, mtxView);
	}

	//最終の出力色計算
	return Color;
}
