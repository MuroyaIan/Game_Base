/**
 * @file VS_Instance.hlsl
 * @brief 頂点シェーダ（インスタンシング）
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
	matrix mtxTransform : WORLD_MTX;	//ワールド行列
};

//エントリーポイント
float4 main(VS_IN vsi) : SV_Position
{
	//座標計算
	float4 pos = mul(float4(vsi.pos, 1.0f), vsi.mtxTransform);
	pos = mul(pos, mtxView);
	pos = mul(pos, mtxProj);

	return pos;
}
