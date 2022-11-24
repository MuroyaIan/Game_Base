
//入力用構造体
struct VS_IN
{
	float2 tex : TEXCOORD;
};

//サンプラー
SamplerState Sampler : register(s0);

//テクスチャ
Texture2D Tex2D : register(t0);

//エントリーポイント
float4 main(VS_IN vsi) : SV_Target
{
	return Tex2D.Sample(Sampler, vsi.tex);
}
