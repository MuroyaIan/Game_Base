
//入力用構造体
struct VS_IN
{
	float3 pos : POSITION;      //座標
	matrix mtxLocal : MTX_L;    //ローカル行列
};

//定数バッファ（変換行列）
cbuffer C_BUFFER
{
	matrix mtxLocal;    //ローカル行列
	matrix mtxWorld;    //ワールド行列
	matrix mtxView;     //ビュー行列
	matrix mtxProj;     //投影行列
};

//エントリーポイント
float4 main(VS_IN vsi) : SV_Position
{
	float4 pos = float4(vsi.pos, 1.0f);
	pos.x *= -1.0f;
	pos = mul(pos, mtxLocal);
	pos = mul(pos, vsi.mtxLocal);
	pos.x *= -1.0f;
	pos = mul(pos, mtxWorld);
	pos = mul(pos, mtxView);
	pos = mul(pos, mtxProj);

	return pos;
}
