
//入力用構造体
struct VS_IN
{
	float3 pos : POSITION;              //座標
	float2 tex : TEXCOORD;              //UV座標
	matrix mtxTransform : WORLD_MTX;    //ワールド行列
};

//出力用構造体
struct VS_OUT
{
	float2 tex : TEXCOORD;
	float4 pos : SV_Position;
};

//定数バッファ（変換行列）
cbuffer CB_CAMERA : register(b0)
{
	matrix mtxView;     //ビュー行列
	matrix mtxProj;     //投影行列
};

//エントリーポイント
VS_OUT main(VS_IN vsi)
{
	VS_OUT vso;
	vso.pos = mul(float4(vsi.pos, 1.0f), vsi.mtxTransform);
	vso.pos = mul(vso.pos, mtxView);
	vso.pos = mul(vso.pos, mtxProj);
	vso.tex = vsi.tex;
	return vso;
}
