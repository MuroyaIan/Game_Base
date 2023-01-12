
//入力用構造体
struct VS_IN
{
	float3 pos : POSITION;              //座標
	float2 tex : TEXCOORD;              //UV座標
	float3 normal : NORMAL;             //法線

	matrix mtxWorld : WORLD_MTX;        //ワールド行列
};

//出力用構造体
struct VS_OUT
{
	float3 posWV : POSITION;            //座標（変換後）
	float2 tex : TEXCOORD;              //UV座標
	float3 normalWV : NORMAL;           //法線（変換後）

	matrix mtxView : MTX_V;             //ビュー行列（ピクセルシェーダ用）

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

	//座標計算
	vso.pos = float4(vsi.pos, 1.0f);
	vso.pos.x *= -1.0f;
	vso.pos = mul(vso.pos, vsi.mtxWorld);
	vso.pos = mul(vso.pos, mtxView);
	vso.posWV = vso.pos.xyz;
	vso.pos = mul(vso.pos, mtxProj);

	//テクスチャ
	vso.tex = vsi.tex;

	//法線
	vso.normalWV = vsi.normal;
	vso.normalWV.x *= -1.0f;
	vso.normalWV = mul(vso.normalWV, (float3x3) vsi.mtxWorld);
	vso.normalWV = mul(vso.normalWV, (float3x3) mtxView);

	//その他
	vso.mtxView = mtxView;

	return vso;
}
