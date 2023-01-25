/**
 * @file PS_Model_Normal.hlsl
 * @brief ピクセルシェーダ（ノーマルマップ付きモデル）
 * @author 室谷イアン
 * @date 2023/01/22
 * @履歴 2023/01/22：ファイル作成
 */

//インクルード部
#include <PS_Model.hlsli>

//入力用構造体
struct PS_IN
{
	float2 tex : TEXCOORD;				//UV座標

	float3 vNorT_ToCamera : CAM_NOR;	//カメラへの単位ベクトル（接空間）
	float3 vDirT_ToLight : LIGHT_DIR;	//光への方向ベクトル接空間（接空間）
	float3 vNorT_ToLight : LIGHT_NOR;	//光への単位ベクトル接空間（接空間）
};

//プロトタイプ宣言
float3 CalcDirectionalLight(PS_IN psi, float3 ModelNormal, float2x3 ModelColor);	//平行光源の計算

//エントリーポイント
float4 main(PS_IN psi) : SV_TARGET
{
	//UV座標を計算
	float height_scale = 0.1f;

	//サンプリング数設定（注視方向に応じてレイヤ層を分割する）
	const float minLayers = 8;
	const float maxLayers = 32;
	float numLayers = lerp(maxLayers, minLayers, abs(dot(float3(0.0f, 0.0f, 1.0f), psi.vNorT_ToCamera)));



	// calculate the size of each layer
	float layerDepth = 1.0f / numLayers; //層ごとの深さの割合
	// depth of current layer
	float currentLayerDepth = 0.0f;
	// the amount to shift the texture coordinates per layer (from vector P)
	//float2 vParallax = psi.vNorT_ToCamera.xy * height_scale;
	float2 vParallax = psi.vNorT_ToCamera.xy / psi.vNorT_ToCamera.z * height_scale;
	float2 deltaTexCoords = vParallax / numLayers; //層ごとの視差ベクトル



	// get initial values
	float2 currentTexCoords = psi.tex;
	float currentDepthMapValue = TexMap[3].Sample(Sampler, currentTexCoords).r;

	[loop][fastopt]
	while (currentLayerDepth < currentDepthMapValue)
	{
		// shift texture coordinates along direction of P
		currentTexCoords += deltaTexCoords;
		// get depthmap value at current texture coordinates
		currentDepthMapValue = TexMap[3].SampleLevel(Sampler, currentTexCoords, 0).r;
		// get depth of next layer
		currentLayerDepth += layerDepth;
	}



	// get texture coordinates before collision (reverse operations)
	float2 prevTexCoords = currentTexCoords - deltaTexCoords;

	// get depth after and before collision for linear interpolation
	float afterDepth = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = TexMap[3].Sample(Sampler, prevTexCoords).r - currentLayerDepth + layerDepth;

	// interpolation of texture coordinates
	float weight = afterDepth / (afterDepth - beforeDepth);
	float2 texDisp = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

	//float height = TexMap[3].Sample(Sampler, psi.tex).r;
	//float2 vParallax = psi.vNorT_ToCamera.xy / psi.vNorT_ToCamera.z * (height * height_scale);
	//float2 texDisp = psi.tex + vParallax;

	//UV座標確認（超過分のピクセルを破棄する）⇒平面の場合はこれで解決
	texDisp = texDisp > 1.0f ? -1.0f : texDisp;
	texDisp = texDisp < 0.0f ? -1.0f : texDisp;
	clip(texDisp);



	//float2 texDisp = psi.tex;

	//テクスチャ取得
	const float3x4 Texture = {
		TexMap[0].Sample(Sampler, texDisp),		//Diffuse
		TexMap[1].Sample(Sampler, texDisp),		//Specular
		TexMap[2].Sample(Sampler, texDisp)		//Normal
	};

	//法線取得
	float3 vNorT_Model = Texture._31_32_33 * 2.0f - 1.0f;	//（左手系）
	vNorT_Model.x *= -1.0f;
	vNorT_Model = normalize(vNorT_Model);

	//モデル色計算
	const float2x3 ModelColor = {
		cbDiffuse.rgb * cbDiffuse.w * Texture._11_12_13,	//Diffuse
		cbSpecular.rgb * Texture._21_22_23					//Specular
	};

	//平行光源の計算
	const float3 Directional = CalcDirectionalLight(psi, vNorT_Model, ModelColor);

	//グローバル環境光の計算
	const float3 g_Ambient = GlobalAmbient.rgb * GlobalAmbient.a * ModelColor._11_12_13;

	//最終の出力色計算
	return float4(saturate(Directional + g_Ambient), 1.0f);
}

//平行光源の計算
float3 CalcDirectionalLight(PS_IN psi, float3 ModelNormal, float2x3 ModelColor)
{
	//平行光源の色
	const float3 LightRGB = LightColor.rgb * LightColor.a;

	//拡散色算出
	const float3 Diffuse = LightRGB * max(0.0f, dot(psi.vNorT_ToLight, ModelNormal)) * ModelColor._11_12_13;

	//鏡面反射色算出
	const float3 vRef = ModelNormal * dot(psi.vDirT_ToLight, ModelNormal) * 2.0f - psi.vDirT_ToLight;	//鏡面反射ベクトル
	const float3 Specular = LightRGB * pow(max(0.0f, dot(normalize(vRef), psi.vNorT_ToCamera)), cbShininess) * ModelColor._21_22_23;

	//環境光の計算
	const float3 Ambient = LightRGB * cbAmbient.rgb;

	//最終の出力色計算
	return Diffuse + Specular + Ambient;
}
