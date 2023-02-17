/**
 * @file VS_Instance_Texture.hlsl
 * @brief 頂点シェーダ（インスタンシング+テクスチャ実装）
 * @author 室谷イアン
 * @date 2023/02/17
 * @履歴 2023/02/17：ファイル作成
 */

//インクルード部
#include <VS_Instance.hlsli>

//入力用構造体
struct VS_IN
{
	float3 pos : POSITION;				//座標
	float2 tex : TEXCOORD;				//UV座標
	matrix mtxTransform : WORLD_MTX;	//ワールド行列
};

//出力用構造体
struct VS_OUT
{
	float2 tex : TEXCOORD;		//UV座標
	float4 pos : SV_Position;
};

//エントリーポイント
VS_OUT main(VS_IN vsi)
{
	//座標計算
	VS_OUT vso;
	vso.pos = mul(float4(vsi.pos, 1.0f), vsi.mtxTransform);
	vso.pos = mul(vso.pos, mtxView);
	vso.pos = mul(vso.pos, mtxProj);

	//UV座標
	vso.tex = vsi.tex;

	return vso;
}
