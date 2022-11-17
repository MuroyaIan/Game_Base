
//入力用構造体
struct VS_IN
{
	float3 pos : POSITION;      //座標
	float4 color : COLOR;       //頂点カラー
	matrix mtxLocal : MTX_L;    //ローカル行列
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
	matrix mtxLocal;    //ローカル行列
	matrix mtxWorld;    //ワールド行列
	matrix mtxView;     //ビュー行列
	matrix mtxProj;     //投影行列
};

//エントリーポイント
VS_OUT main(VS_IN vsi)
{
	VS_OUT vso;

	//座標
	vso.pos = float4(vsi.pos, 1.0f);
	vso.pos.x *= -1.0f;
	vso.pos = mul(vso.pos, mtxLocal);
	vso.pos = mul(vso.pos, vsi.mtxLocal);
	vso.pos.x *= -1.0f;
	vso.pos = mul(vso.pos, mtxWorld);
	vso.pos = mul(vso.pos, mtxView);
	vso.pos = mul(vso.pos, mtxProj);

	//カラー
	vso.color = vsi.color;

	return vso;
}
