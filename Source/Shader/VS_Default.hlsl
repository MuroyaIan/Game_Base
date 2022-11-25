
//入力用構造体
struct VS_IN
{
	float3 pos : POSITION;
};

//定数バッファ（変換行列）
cbuffer C_BUFFER : register(b0)
{
	matrix mtxTransform;
};

//エントリーポイント
float4 main(VS_IN vsi) : SV_Position
{
	return mul(float4(vsi.pos, 1.0f), mtxTransform);
}
