/**
 * @file VS_Model.hlsl
 * @brief 頂点シェーダ（モデル実装）
 * @author 室谷イアン
 * @date 2023/01/18
 * @履歴 2023/01/18：ファイル作成
 */

//インクルード部
#include <VS_Model.hlsli>

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
	float2 tex : TEXCOORD;				//UV座標

	float3 vNorV_Model : NORMAL;		//法線（ビュー空間）
	float3 vNorV_ToCamera : CAM_NOR;	//カメラへの単位ベクトル（ビュー空間）
	float3 vDirV_ToLight : LIGHT_DIR;	//光への方向ベクトル（ビュー空間）
	float3 vNorV_ToLight : LIGHT_NOR;	//光への単位ベクトル（ビュー空間）

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
	float3 PosWV;
	vso.pos = mul(float4(vsi.pos, 1.0f), mtxL);
	vso.pos = mul(vso.pos, mtxLocal);
	vso.pos.x *= -1.0f;
	vso.pos = mul(vso.pos, mtxWorld);
	vso.pos = mul(vso.pos, mtxView);
	PosWV = vso.pos.xyz;				//頂点座標（ビュー空間）⇒光計算用
	vso.pos = mul(vso.pos, mtxProj);

	//テクスチャ
	vso.tex = vsi.tex;

	//法線計算（ビュー空間へ変換）
	vso.vNorV_Model = mul(vsi.normal, (float3x3) mtxL);
	vso.vNorV_Model = mul(vso.vNorV_Model, (float3x3) mtxLocal);
	vso.vNorV_Model.x *= -1.0f;
	vso.vNorV_Model = mul(vso.vNorV_Model, (float3x3) mtxWorld);
	vso.vNorV_Model = mul(vso.vNorV_Model, (float3x3) mtxView);
	vso.vNorV_Model = normalize(vso.vNorV_Model);

	//カメラへの単位ベクトル（ビュー空間へ変換）
	vso.vNorV_ToCamera = normalize(-PosWV);

	//光の情報を計算（ビュー空間へ変換）
	float3 PosL = LightPos.xyz;
	PosL.x += cbLightOffset.x * 2.0f;
	PosL.y -= cbLightOffset.y * 2.0f;
	PosL.z += cbLightOffset.z * 2.0f;
	vso.vDirV_ToLight = mul(PosL, (float3x3) mtxView);	//鏡面反射確認の為、疑似的に位置を設定
	vso.vNorV_ToLight = normalize(vso.vDirV_ToLight);	//光源への単位ベクトル

	return vso;
}
