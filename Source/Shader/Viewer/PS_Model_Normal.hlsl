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
float2 CalcDisplacementMapUV(PS_IN psi);	//視差マップのUVを計算

//エントリーポイント
float4 main(PS_IN psi) : SV_TARGET
{
	//UV座標を計算
	float2 TexDisp = psi.tex;

#ifdef DISP_MAP

	TexDisp = CalcDisplacementMapUV(psi);

#endif // DISP_MAP

	//テクスチャ取得（左手系）
	const float3x4 Texture = {
		TexMap[0].Sample(Sampler, TexDisp),		//Diffuse
		TexMap[1].Sample(Sampler, TexDisp),		//Specular
		TexMap[2].Sample(Sampler, TexDisp)		//Normal
	};

	//モデル色計算
	const float3x3 ModelColor = {
		matData.Diffuse.rgb * matData.Diffuse.w * Texture._11_12_13,	//Diffuse
		matData.Specular.rgb * Texture._21_22_23,						//Specular
		matData.Ambient.rgb												//Ambient
	};

	//法線取得
	float3 vNorT_Model = Texture._31_32_33 * 2.0f - 1.0f;
	vNorT_Model.x *= -1.0f;									//左手系ヘ
	vNorT_Model = normalize(vNorT_Model);

	//光源計算用ベクトル
	const LIGHT_VECTOR LightVec =
	{
		vNorT_Model,
		psi.vNorT_ToCamera,
		psi.vDirT_ToLight,
		psi.vNorT_ToLight
	};

	//平行光源の計算
	const float3 LightColor = DirectionalLight.Color_D.rgb * DirectionalLight.Color_D.a;
	const float3 Directional = CalcDirectionalLight(LightVec, ModelColor, LightColor, matData.Shininess);

	//グローバル環境光の計算
	const float3 g_Ambient = AmbientLight.rgb * AmbientLight.a * ModelColor._11_12_13;

	//最終の出力色計算
	return float4(saturate(Directional + g_Ambient), 1.0f);
}

//視差マップのUVを計算
float2 CalcDisplacementMapUV(PS_IN psi)
{
	//サンプリング数設定（注視方向に応じて深さ計算用レイヤの分割数を設定）
	const float MinLayerNum = matData.Disp_MinLayerNum;
	const float MaxLayerNum = matData.Disp_MaxLayerNum;
	const float LayerNum = lerp(MinLayerNum, MaxLayerNum, abs(dot(float3(0.0f, 0.0f, 1.0f), psi.vNorT_ToCamera)));

	//ループ計算用のデータを設定
	float Depth_Layer = 0.0f;																//レイヤ深さ
	const float DepthPerLayer = 1.0f / LayerNum;											//レイヤごとの深さ
	const float DepthScale = matData.Disp_DepthScale;										//視差マップ用深さ係数
	//float2 vParallax = psi.vNorT_ToCamera.xy * DepthScale;
	const float2 vParallax = psi.vNorT_ToCamera.xy / psi.vNorT_ToCamera.z * DepthScale;		//視差ベクトル（UV座標の総変化量）
	const float2 vParallaxPerLayer = vParallax / LayerNum;									//レイヤごとの視差ベクトル
	float2 CurrentUV = psi.tex;																//UV座標
	float Depth_DispMap = TexMap[3].Sample(Sampler, CurrentUV).r;							//視差マップから取得した深さ

	//深さ計算
	[loop][fastopt]
	while (Depth_Layer < Depth_DispMap) {

		//レイヤごとのレイヤ深さと視差マップ深さを比較し、逆転する箇所を特定
		Depth_Layer += DepthPerLayer;
		CurrentUV += vParallaxPerLayer;
		Depth_DispMap = TexMap[3].SampleLevel(Sampler, CurrentUV, 0.0f).r;
	}

	//逆転直前と直後の比較差から視差ベクトルの比重を求める
	const float AfterDepth = Depth_Layer - Depth_DispMap;
	const float2 PrevUV = CurrentUV - vParallaxPerLayer;											//逆転直前のUV座標
	const float BeforeDepth = TexMap[3].Sample(Sampler, PrevUV).r - Depth_Layer + DepthPerLayer;
	const float Weight = AfterDepth / (AfterDepth + BeforeDepth);

	//UV座標を確定
	float2 TexDisp = PrevUV * Weight + CurrentUV * (1.0f - Weight);

	//UV座標範囲確認（超過分のピクセルを破棄する）⇒平面の場合はこれで解決
	TexDisp = TexDisp > 1.0f ? -1.0f : TexDisp;
	TexDisp = TexDisp < 0.0f ? -1.0f : TexDisp;
	clip(TexDisp);

	return TexDisp;
}
