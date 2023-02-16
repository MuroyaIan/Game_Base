/**
 * @file VS_Model_Bone.hlsl
 * @brief 頂点シェーダ（モデル骨描画用）
 * @author 室谷イアン
 * @date 2023/01/19
 * @履歴 2023/01/19：ファイル作成
 */

//インクルード部
#include <../Base/VS_Base.hlsli>

//入力用構造体
struct VS_IN
{
	float3 pos : POSITION;		//座標
	float4 color : COLOR;		//頂点カラー
	matrix mtxBone : MTX_L;		//骨の姿勢行列（インスタンシング）
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
	VS_OUT vso;

	//座標
	vso.pos = float4(vsi.pos, 1.0f);
	vso.pos.x *= -1.0f;						//右手系ヘ（ピラミッド型はメモリ生成の為）
	vso.pos = mul(vso.pos, vsi.mtxBone);	//骨変換
	vso.pos.x *= -1.0f;						//左手系ヘ
	vso.pos = mul(vso.pos, mtxWVP);

	//カラー
	vso.color = vsi.color;

	return vso;
}
