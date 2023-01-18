/**
 * @file VS_Texture.hlsl
 * @brief 頂点シェーダ（テクスチャ実装）
 * @author 室谷イアン
 * @date 2023/01/18
 * @履歴 2023/01/18：ファイル作成
 */

//インクルード部
#include <VS_Base.hlsli>

//入力用構造体
struct VS_IN
{
	float3 pos : POSITION;	//座標
	float2 tex : TEXCOORD;	//UV座標
};

//出力用構造体
struct VS_OUT
{
	float2 tex : TEXCOORD;		//UV座標
	float4 pos : SV_Position;	//変換済座標
};

//エントリーポイント
VS_OUT main(VS_IN vsi)
{
	VS_OUT vso;
	vso.pos = mul(float4(vsi.pos, 1.0f), mtxWVP);
	vso.tex = vsi.tex;
	return vso;
}
