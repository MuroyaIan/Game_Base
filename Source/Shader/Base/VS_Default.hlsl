/**
 * @file VS_Default.hlsl
 * @brief 頂点シェーダ（座標のみ）
 * @author 室谷イアン
 * @date 2023/01/18
 * @履歴 2023/01/18：ファイル作成
 */

//インクルード部
#include "VS_Base.hlsli"

//入力用構造体
struct VS_IN
{
	float3 pos : POSITION;	//座標
};

//エントリーポイント
float4 main(VS_IN vsi) : SV_Position
{
	return mul(float4(vsi.pos, 1.0f), mtxWVP);
}
