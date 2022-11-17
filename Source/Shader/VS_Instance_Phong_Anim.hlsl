
//入力用構造体
struct VS_IN
{
	float3 pos : POSITION;          //座標
	float2 tex : TEXCOORD;          //UV座標
	float3 normal : NORMAL;         //法線
	int boneID[4] : BONE;           //骨番号
	float boneWeight[4] : WEIGHT;   //骨比重

	matrix mtxWorld : WORLD_MTX;    //ワールド行列
};

//出力用構造体
struct VS_OUT
{
	float3 posWV : POSITION;        //座標（変換後）
	float2 tex : TEXCOORD;          //UV座標
	float3 normalWV : NORMAL;       //法線（変換後）

	matrix mtxView : MTX_V;         //ビュー行列（ピクセルシェーダ用）

	float4 pos : SV_Position;
};

//定数バッファ（変換行列）
cbuffer CB_CAMERA : register(b1)
{
	matrix mtxView;     //ビュー行列
	matrix mtxProj;     //投影行列
};

//定数バッファ（骨情報）
cbuffer CB_BONE : register(b2)
{
	matrix mtxBone[250];    //骨行列
};

//定数バッファ（ローカル行列）
cbuffer CB_LOCAL : register(b3)
{
	matrix mtxSkin;     //骨なしメッシュ用ローカル行列
};

//エントリーポイント
VS_OUT main(VS_IN vsi)
{
	VS_OUT vso;

	//フレーム姿勢算出
	matrix mtxL = {
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	};
	[unroll]
	for (int i = 0; i < 4; i++)
		mtxL += mtxBone[vsi.boneID[i]] * vsi.boneWeight[i];     //影響する骨行列の加算

	//座標計算
	vso.pos = mul(float4(vsi.pos, 1.0f), mtxL);
	vso.pos = mul(vso.pos, mtxSkin);
	vso.pos.x *= -1.0f;
	vso.pos = mul(vso.pos, vsi.mtxWorld);
	vso.pos = mul(vso.pos, mtxView);
	vso.posWV = vso.pos.xyz;
	vso.pos = mul(vso.pos, mtxProj);

	//テクスチャ
	vso.tex = vsi.tex;

	//法線
	vso.normalWV = mul(vsi.normal, (float3x3) mtxL);
	vso.normalWV = mul(vso.normalWV, (float3x3) mtxSkin);
	vso.normalWV.x *= -1.0f;
	vso.normalWV = mul(vso.normalWV, (float3x3) vsi.mtxWorld);
	vso.normalWV = mul(vso.normalWV, (float3x3) mtxView);

	//その他
	vso.mtxView = mtxView;

	return vso;
}
