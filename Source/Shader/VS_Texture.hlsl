
//入力用構造体
struct VS_IN
{
	float3 pos : POSITION;
	float2 tex : TEXCOORD;
};

//出力用構造体
struct VS_OUT
{
	float2 tex : TEXCOORD;
	float4 pos : SV_Position;
};

//定数バッファ（変換行列）
cbuffer C_BUFFER
{
	matrix mtxTransform;
};

//エントリーポイント
VS_OUT main(VS_IN vsi)
{
	VS_OUT vso;
	vso.pos = mul(float4(vsi.pos, 1.0f), mtxTransform);
	vso.tex = vsi.tex;
	return vso;
}
