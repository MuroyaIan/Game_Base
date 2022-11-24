
//入力用構造体
struct PS_IN
{
	float3 posWV : POSITION;    //座標（変換後）
	float2 tex : TEXCOORD;      //UV座標
	float3 normalWV : NORMAL;   //法線（変換後）

	matrix mtxView : MTX_V;     //ビュー行列
};

//サンプラー
SamplerState Sampler : register(s0);

//テクスチャ
Texture2D TexMap[3] : register(t0);		//0:Diffuse, 1:Specular, 2:Normal

//定数バッファ（マテリアル）
cbuffer CB_MATERIAL : register(b2)
{
	float4 cbAmbient;       //環境光
	float4 cbDiffuse;       //拡散反射光
	float4 cbEmissive;      //発射光
	float4 cbTransparent;   //透過度
	float4 cbSpecular;      //鏡面反射光
	float cbShininess;      //光沢
	float Pad1;
	float Pad2;
	float Pad3;             //仮置き
}

//グローバル定数
static const float3 LightPos = { -1.0f, 1.0f, -1.0f };              //平行光源の向き
static const float4 LightColorD = { 1.0f, 1.0f, 1.0f, 1.0f };       //平行光源の拡散反射光
static const float4 LightColorS = { 1.0f, 1.0f, 1.0f, 1.0f };       //平行光源の鏡面反射光
static const float4 LightColorA = { 1.0f, 1.0f, 1.0f, 1.0f };       //平行光源の環境光
static const float4 GlobalAmbient = { 1.0f, 1.0f, 1.0f, 0.2f };     //大域環境光

//プロトタイプ宣言
float3 CalcDirectionalLight(PS_IN psi, float3 ModelNormal, float3 ColorD); //平行光源の計算

//エントリーポイント
float4 main(PS_IN psi) : SV_TARGET
{
	//法線計算
	const float3 vNor_Model = normalize(psi.normalWV);

	//テクスチャ計算
	const float4 TexD = TexMap[0].Sample(Sampler, psi.tex);
	const float3 ColorD = TexD.rgb * TexD.a;
	const float4 TexS = TexMap[1].Sample(Sampler, psi.tex);
	const float3 ColorS = TexS.rgb * TexS.a;

	//平行光源の計算
	const float3 Directional = CalcDirectionalLight(psi, vNor_Model, ColorD);

	//グローバル環境光の計算
	const float3 g_Ambient = GlobalAmbient.rgb * GlobalAmbient.a * ColorD;

	//最終の出力色計算
	return float4(saturate(Directional + g_Ambient), 1.0f);
}

//平行光源の計算
float3 CalcDirectionalLight(PS_IN psi, float3 ModelNormal, float3 ColorD)
{
	//光への単位ベクトル
	const float3 vToLight = mul(LightPos.xyz, (float3x3) psi.mtxView);
	const float3 vNor_ToLight = normalize(vToLight);

	//拡散色算出
	const float3 Diffuse = LightColorD.rgb * LightColorD.a * max(0.0f, dot(vNor_ToLight, ModelNormal)) * cbDiffuse.rgb * ColorD.rgb;

	//鏡面反射色算出
	const float3 vRef = ModelNormal * dot(vToLight, ModelNormal) * 2.0f - vToLight;     //鏡面反射ベクトル
	const float3 Specular = LightColorS.rgb * LightColorS.a * pow(max(0.0f, dot(normalize(vRef), normalize(-psi.posWV))), cbShininess) * cbSpecular.rgb;

	//環境光の計算
	const float3 Ambient = LightColorA.rgb * LightColorA.a * cbAmbient.rgb;

	//最終の出力色計算
	return Diffuse + Specular + Ambient;
}
