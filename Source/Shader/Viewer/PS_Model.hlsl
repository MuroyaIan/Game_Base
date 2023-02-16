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

//エントリーポイント
float4 main(PS_IN psi) : SV_TARGET
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

	//光源計算用ベクトル
	const LIGHT_VECTOR LightVec = {
		psi.vNorV_Model,
		psi.vNorV_ToCamera,
		psi.vDirV_ToLight,
		psi.vNorV_ToLight
	};

	//平行光源の計算
	const float3 LightColor = DirectionalLight.Color_D.rgb * DirectionalLight.Color_D.a;
	const float3 Directional = CalcDirectionalLight(LightVec, ModelColor, LightColor, matData.Shininess);

	//グローバル環境光の計算
	const float3 g_Ambient = AmbientLight.rgb * AmbientLight.a * ModelColor._11_12_13;

	//最終の出力色計算
	return float4(saturate(Directional + g_Ambient), 1.0f);
}
