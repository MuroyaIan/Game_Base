
//入力用構造体
struct VS_IN
{
	float3 pos : POSITION;
	float4 color : COLOR;
};

//出力用構造体
struct VS_OUT
{
	float4 color : COLOR;
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
	vso.color = vsi.color;
	return vso;
}
