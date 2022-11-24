
//入力用構造体
struct PS_IN
{
	float3 posWV : POSITION;    //座標（変換後）
	float2 tex : TEXCOORD;      //UV座標
	float3 normalWV : NORMAL;   //法線（変換後）

	matrix mtxView : MTX_V;     //ビュー行列
};

//構造体宣言
struct LIGHT_DIRECTIONAL    //平行光源
{
	float4 Pos;             //ワールド座標
	float4 Color_D;         //光の色（拡散色, aは強度）
};

struct LIGHT_POINT			//点光源
{
	float4 Pos;             //ワールド座標
	float4 Color_D;         //光の色（拡散色, aは強度）

	float AttConst;
	float AttLinear;
	float AttQuadratic;     //光の減衰
	float Pad;              //仮置き
};

//サンプラー
SamplerState Sampler : register(s0);

//テクスチャ
Texture2D TexMap[3] : register(t0);		//0:Diffuse, 1:Specular, 2:Normal

//定数バッファ（ライト情報）
cbuffer CB_LIGHT : register(b1)
{
	LIGHT_DIRECTIONAL DirectionalLight;     //平行光源
	LIGHT_POINT PointLight[16];             //点光源
	float4 AmbientLight;                    //環境光
}

//定数バッファ（マテリアル）
cbuffer CB_MATERIAL : register(b2)
{
	float4 mAmbient;        //環境光
	float4 mDiffuse;        //拡散反射光
	float4 mEmissive;       //発射光
	float4 mTransparent;    //透過度
	float4 mSpecular;       //鏡面反射光
	float mShininess;       //光沢
	float mPad1;
	float mPad2;
	float mPad3;            //仮置き
};

//プロトタイプ宣言
float3 CalcDirectionalLight(PS_IN psi, float3 ModelNormal);     //平行光源の計算
float3 CalcPointLight(PS_IN psi, float3 ModelNormal);           //点光源の計算

//エントリーポイント
float4 main(PS_IN psi) : SV_Target
{
	//法線計算
	const float3 vNor_Model = normalize(psi.normalWV);

	//平行光源の計算
	const float3 Directional = CalcDirectionalLight(psi, vNor_Model);

	//点光源の計算
	const float3 Point = CalcPointLight(psi, vNor_Model);

	//グローバル環境光の計算
	const float3 gAmbient = AmbientLight.rgb * AmbientLight.a;

	//最終の出力色計算
	//return float4(saturate((Directional + Point + Ambient) * (float3) psi.material), 1.0f);
	return float4(saturate(Directional + Point + gAmbient), 1.0f) * TexMap[0].Sample(Sampler, psi.tex);
}

//平行光源の計算
float3 CalcDirectionalLight(PS_IN psi, float3 ModelNormal)
{
	//光への単位ベクトル
	const float3 vToLight = mul(DirectionalLight.Pos.xyz, (float3x3) psi.mtxView);
	const float3 vNor_ToLight = normalize(vToLight);

	//平行光源の色
	const float3 Light = DirectionalLight.Color_D.rgb * DirectionalLight.Color_D.a;

	//拡散色算出
	const float3 Diffuse = Light * max(0.0f, dot(vNor_ToLight, ModelNormal)) * mDiffuse.rgb;

	//鏡面反射色算出
	const float3 vRef = ModelNormal * dot(vToLight, ModelNormal) * 2.0f - vToLight;     //鏡面反射ベクトル
	const float3 Specular = Light * pow(max(0.0f, dot(normalize(vRef), normalize(-psi.posWV))), mShininess) * mSpecular.rgb;

	//環境光の計算
	const float3 Ambient = Light * mAmbient.rgb;

	//最終の出力色計算
	return Diffuse + Specular + Ambient;
}

//点光源の計算
float3 CalcPointLight(PS_IN psi, float3 ModelNormal)
{
	//出力用変数
	float3 Color = float3(0.0f, 0.0f, 0.0f);

	//計算ループ
	[unroll]
	for (int i = 0; i < 16; i++)
	{
		//ライト座標のビュー変換
		const float4 LightPosWV = mul(PointLight[i].Pos, psi.mtxView);

		//光への単位ベクトル
		const float3 vToLight = LightPosWV.xyz - psi.posWV;
		const float dLight = length(vToLight);
		const float3 vNor_ToLight = vToLight / dLight;

		//光の減衰計算
		const float Att = 1.0f / (PointLight[i].AttConst + PointLight[i].AttLinear * dLight + PointLight[i].AttQuadratic * (dLight * dLight));

		//点光源の色
		const float3 Light = PointLight[i].Color_D.rgb * PointLight[i].Color_D.a * Att;

		//拡散色算出
		const float3 Diffuse = Light * max(0.0f, dot(vNor_ToLight, ModelNormal)) * mDiffuse.rgb;

		//鏡面反射色算出
		const float3 vRef = ModelNormal * dot(vToLight, ModelNormal) * 2.0f - vToLight;     //鏡面反射ベクトル
		const float3 Specular = Light * pow(max(0.0f, dot(normalize(vRef), normalize(-psi.posWV))), mShininess) * mSpecular.rgb;

		//環境光の計算
		const float3 Ambient = Light * mAmbient.rgb;

		//色を加算
		Color += Diffuse;
		Color += Specular;
		Color += Ambient;
	}

	//最終の出力色計算
	return Color;
}
