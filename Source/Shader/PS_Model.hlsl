/**
 * @file PS_Model.hlsl
 * @brief 頂点シェーダ（モデル実装）
 * @author 室谷イアン
 * @date 2023/01/19
 * @履歴 2023/01/19：ファイル作成
 */

//入力用構造体
struct PS_IN
{
	float3 posWV : POSITION;		//座標（変換後）
	float2 tex : TEXCOORD;			//UV座標

	float3x3 mtxTtoV : MTX_TWV;		//変換行列(接空間⇒ビュー空間）
	float3x3 mtxView : MTX_V;		//ビュー行列
};

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

//グローバル定数(仮の平行光源)
static const float3 LightPos = { -1.0f, 1.0f, -1.0f };				//平行光源の向き
static const float4 LightColor = { 1.0f, 1.0f, 1.0f, 1.0f };		//平行光源の光
static const float4 GlobalAmbient = { 1.0f, 1.0f, 1.0f, 0.2f };		//大域環境光

//プロトタイプ宣言
float3 CalcDirectionalLight(PS_IN psi, float3 ModelNormal, float2x3 ModelColor); //平行光源の計算

//エントリーポイント
float4 main(PS_IN psi) : SV_TARGET
{
	//テクスチャ取得
	const float3x4 Texture = {
		TexMap[0].Sample(Sampler, psi.tex),		//Diffuse
		TexMap[1].Sample(Sampler, psi.tex),		//Specular
		TexMap[2].Sample(Sampler, psi.tex)		//Normal
	};

	//法線計算
	//const float3 vNor_Model = normalize(psi.normalWV);
	float3 vNor_Model = Texture._31_32_33 * 2.0f - 1.0f;
	vNor_Model = mul(vNor_Model, psi.mtxTtoV);
	vNor_Model = normalize(vNor_Model);

	//モデル色計算
	const float2x3 ModelColor = {
		cbDiffuse.rgb * cbDiffuse.w * Texture._11_12_13,	//Diffuse
		cbSpecular.rgb * Texture._21_22_23					//Specular
	};

	//平行光源の計算
	const float3 Directional = CalcDirectionalLight(psi, vNor_Model, ModelColor);

	//グローバル環境光の計算
	const float3 g_Ambient = GlobalAmbient.rgb * GlobalAmbient.a * ModelColor._11_12_13;

	//最終の出力色計算
	return float4(saturate(Directional + g_Ambient), 1.0f);
}

//平行光源の計算
float3 CalcDirectionalLight(PS_IN psi, float3 ModelNormal, float2x3 ModelColor)
{
	//光への単位ベクトル
	float3 PosL = LightPos.xyz;
	PosL.x += Pad1 * 2.0f;
	PosL.y -= Pad2 * 2.0f;
	PosL.z += Pad3 * 2.0f;
	const float3 vToLight = mul(PosL, psi.mtxView);
	const float3 vNor_ToLight = normalize(vToLight);

	//平行光源の色
	const float3 Light = LightColor.rgb * LightColor.a;

	//拡散色算出
	const float3 Diffuse = Light * max(0.0f, dot(vNor_ToLight, ModelNormal)) * ModelColor._11_12_13;

	//鏡面反射色算出
	const float3 vRef = ModelNormal * dot(vToLight, ModelNormal) * 2.0f - vToLight;		//鏡面反射ベクトル
	//const float PowerS = pow(2.0f, Texture._24 * 13.0f);								//αチャンネル付きスペキュラマップ?
	const float3 Specular = Light * pow(max(0.0f, dot(normalize(vRef), normalize(-psi.posWV))), cbShininess) * ModelColor._21_22_23;

	//環境光の計算
	const float3 Ambient = Light * cbAmbient.rgb;

	//最終の出力色計算
	return Diffuse + Specular + Ambient;
}
