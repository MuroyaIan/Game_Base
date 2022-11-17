
//入力用構造体
struct VS_IN
{
	float2 tex : TEXCOORD;
};

//テクスチャ
Texture2D Tex2D;

//サンプラー
SamplerState Sampler;

//エントリーポイント
float4 main(VS_IN vsi) : SV_Target
{
	return Tex2D.Sample(Sampler, vsi.tex);
}
