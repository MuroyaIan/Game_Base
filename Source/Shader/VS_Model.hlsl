/**
 * @file VS_Model.hlsl
 * @brief 頂点シェーダ（モデル実装）
 * @author 室谷イアン
 * @date 2023/01/18
 * @履歴 2023/01/18：ファイル作成
 */

//入力用構造体
struct VS_IN
{
	float3 pos : POSITION;			//座標
	float2 tex : TEXCOORD;			//UV座標
	float3 normal : NORMAL;			//法線
	float3 binormal : BINORMAL;		//従法線
	float3 tangent : TANGENT;		//接線
	int boneID[4] : BONE;			//骨番号
	float boneWeight[4] : WEIGHT;	//骨比重
};

//出力用構造体
struct VS_OUT
{
	float3 posWV : POSITION;		//座標（変換後）
	float2 tex : TEXCOORD;			//UV座標

	float3x3 mtxTtoV : MTX_TWV;		//変換行列(接空間⇒ビュー空間）
	float3x3 mtxView : MTX_V;		//ビュー行列

	float4 pos : SV_Position;
};

//定数バッファ（変換行列）
cbuffer CB_MTX_LWVP : register(b0)
{
	matrix mtxLocal;	//ローカル行列(骨なしメッシュの変換)
	matrix mtxWorld;	//ワールド行列
	matrix mtxView;		//ビュー行列
	matrix mtxProj;		//投影行列
};

//定数バッファ（骨情報）
cbuffer CB_BONE : register(b1)
{
	matrix mtxBone[250];	//骨行列
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
		mtxL += mtxBone[vsi.boneID[i]] * vsi.boneWeight[i];		//影響する骨行列の加算

	//座標計算
	vso.pos = mul(float4(vsi.pos, 1.0f), mtxL);
	vso.pos = mul(vso.pos, mtxLocal);
	vso.pos.x *= -1.0f;
	vso.pos = mul(vso.pos, mtxWorld);
	vso.pos = mul(vso.pos, mtxView);
	vso.posWV = vso.pos.xyz;
	vso.pos = mul(vso.pos, mtxProj);

	//テクスチャ
	vso.tex = vsi.tex;

	//変換行列(接空間⇒ビュー空間）
	vso.mtxTtoV._11_12_13 = vsi.tangent;
	vso.mtxTtoV._21_22_23 = vsi.binormal;
	vso.mtxTtoV._31_32_33 = vsi.normal;
	vso.mtxTtoV = mul(vso.mtxTtoV, (float3x3) mtxL);
	vso.mtxTtoV = mul(vso.mtxTtoV, (float3x3) mtxLocal);
	vso.mtxTtoV._11 *= -1.0f;
	vso.mtxTtoV._21 *= -1.0f;
	vso.mtxTtoV._31 *= -1.0f;
	vso.mtxTtoV = mul(vso.mtxTtoV, (float3x3) mtxWorld);
	vso.mtxTtoV = mul(vso.mtxTtoV, (float3x3) mtxView);

	//その他
	vso.mtxView = (float3x3) mtxView;

	return vso;
}
