/**
 * @file Light.hlsli
 * @brief ライト定義
 * @author 室谷イアン
 * @date 2023/02/10
 * @履歴 2023/02/10：ファイル作成
 */

//構造体宣言
struct LIGHT_DIRECTIONAL	//平行光源
{
	float4 Pos;				//ワールド座標
	float4 Color_D;			//光の色（拡散色, aは強度）
};

struct LIGHT_POINT			//点光源
{
	float4 Pos;				//ワールド座標
	float4 Color_D;			//光の色（拡散色, aは強度）

	float AttConst;
	float AttLinear;
	float AttQuadratic;		//光の減衰
	float Pad;				//仮置き
};

struct LIGHT_VECTOR						//光源計算用ベクトル（平行光源）
{
	float3 vNor_Model : NORMAL;			//モデルの法線ベクトル
	float3 vNor_ToCamera : CAM_NOR;		//カメラへの単位ベクトル
	float3 vDir_ToLight : LIGHT_DIR;	//光への方向ベクトル
	float3 vNor_ToLight : LIGHT_NOR;	//光への単位ベクトル
};

struct LIGHT_VECTOR_PT					//光源計算用ベクトル（点光源、複数の平行光源）
{
	float3 vNor_Model : NORMAL;			//モデルの法線ベクトル
	float3 vNor_ToCamera : CAM_NOR;		//カメラへの単位ベクトル
};

//関数宣言

//平行光源の計算
float3 CalcDirectionalLight(LIGHT_VECTOR LightVec, float3x3 ModelColor, float3 LightColor, float Shininess)
{
	//拡散色算出
	const float3 Diffuse = LightColor * max(0.0f, dot(LightVec.vNor_ToLight, LightVec.vNor_Model)) * ModelColor._11_12_13;

	//鏡面反射色算出
	const float3 vRef = LightVec.vNor_Model * dot(LightVec.vDir_ToLight, LightVec.vNor_Model) * 2.0f - LightVec.vDir_ToLight;	//鏡面反射ベクトル
	const float3 Specular = LightColor * pow(max(0.0f, dot(normalize(vRef), LightVec.vNor_ToCamera)), Shininess) * ModelColor._21_22_23;

	//環境光の計算
	const float3 Ambient = LightColor * ModelColor._31_32_33;

	//最終の出力色計算
	return Diffuse + Specular + Ambient;
}

//点光源の計算
float3 CalcPointLight(LIGHT_VECTOR_PT LightVec, float3x3 ModelColor, LIGHT_POINT PointLight, float Shininess, float3 PosWV, float4x4 mtxView)
{
	//出力用変数
	float3 Color = float3(0.0f, 0.0f, 0.0f);

	//ライト座標のビュー変換
	const float4 LightPosWV = mul(PointLight.Pos, mtxView);

	//光への単位ベクトル
	const float3 vToLight = LightPosWV.xyz - PosWV;
	const float dLight = length(vToLight);
	const float3 vNor_ToLight = vToLight / dLight;

	//光の減衰計算
	const float Att = 1.0f / (PointLight.AttConst + PointLight.AttLinear * dLight + PointLight.AttQuadratic * (dLight * dLight));

	//点光源の色
	const float3 Light = PointLight.Color_D.rgb * PointLight.Color_D.a * Att;

	//拡散色算出
	const float3 Diffuse = Light * max(0.0f, dot(vNor_ToLight, LightVec.vNor_Model)) * ModelColor._11_12_13;

	//鏡面反射色算出
	const float3 vRef = LightVec.vNor_Model * dot(vToLight, LightVec.vNor_Model) * 2.0f - vToLight;		//鏡面反射ベクトル
	//const float PowerS = pow(2.0f, Texture._24 * 13.0f);												//αチャンネル付きスペキュラマップ?
	const float3 Specular = Light * pow(max(0.0f, dot(normalize(vRef), LightVec.vNor_ToCamera)), Shininess) * ModelColor._21_22_23;

	//環境光の計算
	const float3 Ambient = Light * ModelColor._31_32_33;

	//色を加算
	Color += Diffuse;
	Color += Specular;
	Color += Ambient;

	//最終の出力色計算
	return Color;
}
