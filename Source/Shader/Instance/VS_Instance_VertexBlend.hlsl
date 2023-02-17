/**
 * @file VS_Instance_VertexBlend.hlsl
 * @brief 頂点シェーダ（インスタンシング+頂点ブレンド）
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
	float4 color : COLOR;				//頂点カラー
	matrix mtxTransform : WORLD_MTX;	//ワールド行列
};

//出力用構造体
struct VS_OUT
{
	float4 color : COLOR;		//頂点カラー
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

	//頂点色
	vso.color = vsi.color;

	return vso;
}
