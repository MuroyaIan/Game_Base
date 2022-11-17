
//入力用構造体
struct VS_IN
{
	float3 pos : POSITION;              //座標
	matrix mtxTransform : WORLD_MTX;    //ワールド行列
};

//定数バッファ（変換行列）
cbuffer CB_CAMERA : register(b1)
{
	matrix mtxView;     //ビュー行列
	matrix mtxProj;     //投影行列
};

//エントリーポイント
float4 main(VS_IN vsi) : SV_Position
{
	float4 pos = mul(float4(vsi.pos, 1.0f), vsi.mtxTransform);
	pos = mul(pos, mtxView);
	pos = mul(pos, mtxProj);

	return pos;
}
