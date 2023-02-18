/**
 * @file VS_Instance_Phong_Anim.hlsl
 * @brief 頂点シェーダ（インスタンシング+Phongモデル+アニメーション付き）
 * @author 室谷イアン
 * @date 2023/02/18
 * @履歴 2023/02/18：ファイル作成
 */

//インクルード部
#define MODEL_ANIM
#include <VS_Instance.hlsli>

//入力用構造体
struct VS_IN
{
	float3 pos : POSITION;			//座標
	float2 tex : TEXCOORD;			//UV座標
	float3 normal : NORMAL;			//法線
	int boneID[4] : BONE;			//骨番号
	float boneWeight[4] : WEIGHT;	//骨比重

	matrix mtxWorld : WORLD_MTX;	//ワールド行列
};

//出力用構造体
struct VS_OUT
{
	float3 posWV : POSITION;			//座標（ビュー空間）
	float2 tex : TEXCOORD;				//UV座標

	float3 vNorV_Model : NORMAL;		//法線（ビュー空間）
	float3 vNorV_ToCamera : CAM_NOR;	//カメラへの単位ベクトル（ビュー空間）
	matrix mtxView : MTX_V;				//ビュー行列

	float4 pos : SV_Position;
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
	vso.pos = mul(vso.pos, mtxSkin);
	vso.pos.x *= -1.0f;						//左手系へ
	vso.pos = mul(vso.pos, vsi.mtxWorld);
	vso.pos = mul(vso.pos, mtxView);
	vso.posWV = vso.pos.xyz;				//頂点座標（ビュー空間）⇒光計算用
	vso.pos = mul(vso.pos, mtxProj);

	//UV座標
	vso.tex = vsi.tex;

	//法線
	vso.vNorV_Model = mul(vsi.normal, (float3x3) mtxL);
	vso.vNorV_Model = mul(vso.vNorV_Model, (float3x3) mtxSkin);
	vso.vNorV_Model.x *= -1.0f;									//左手系へ
	vso.vNorV_Model = mul(vso.vNorV_Model, (float3x3) vsi.mtxWorld);
	vso.vNorV_Model = mul(vso.vNorV_Model, (float3x3) mtxView);
	vso.vNorV_Model = normalize(vso.vNorV_Model);

	//カメラへの単位ベクトル（ビュー空間へ変換）
	vso.vNorV_ToCamera = normalize(-vso.posWV);

	//その他
	vso.mtxView = mtxView;

	return vso;
}
