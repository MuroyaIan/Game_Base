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

struct LIGHT_VECTOR						//光源計算用ベクトル
{
	float3 vNor_Model : NORMAL;			//モデルの法線ベクトル
	float3 vNor_ToCamera : CAM_NOR;		//カメラへの単位ベクトル
	float3 vDir_ToLight : LIGHT_DIR;	//光への方向ベクトル
	float3 vNor_ToLight : LIGHT_NOR;	//光への単位ベクトル
};

//関数宣言

//平行光源の計算
float3 CalcDirectionalLight(LIGHT_VECTOR LightVec, float3x3 ModelColor, float3 LightColor, float Shininess)
{
	//拡散色算出
	const float3 Diffuse = LightColor * max(0.0f, dot(LightVec.vNor_ToLight, LightVec.vNor_Model)) * ModelColor._11_12_13;

	//鏡面反射色算出
	const float3 vRef = LightVec.vNor_Model * dot(LightVec.vDir_ToLight, LightVec.vNor_Model) * 2.0f - LightVec.vDir_ToLight;	//鏡面反射ベクトル
	//const float PowerS = pow(2.0f, Texture._24 * 13.0f);																		//αチャンネル付きスペキュラマップ?
	const float3 Specular = LightColor * pow(max(0.0f, dot(normalize(vRef), LightVec.vNor_ToCamera)), Shininess) * ModelColor._21_22_23;

	//環境光の計算
	const float3 Ambient = LightColor * ModelColor._31_32_33;

	//最終の出力色計算
	return Diffuse + Specular + Ambient;
}
