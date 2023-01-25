/**
 * @file VS_Model_Normal.hlsl
 * @brief 頂点シェーダ（ノーマルマップ付きモデル）
 * @author 室谷イアン
 * @date 2023/01/22
 * @履歴 2023/01/22：ファイル作成
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

	float3 vNorT_ToCamera : CAM_NOR;	//カメラへの単位ベクトル（接空間）
	float3 vDirT_ToLight : LIGHT_DIR;	//光への方向ベクトル（接空間）
	float3 vNorT_ToLight : LIGHT_NOR;	//光への単位ベクトル（接空間）

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

	//BTN行列を取得（左手系）
	float3x3 mtxT = {
		vsi.tangent,
		vsi.binormal,
		vsi.normal
	};

	//カメラへの単位ベクトル（接空間へ変換）
	vso.vNorT_ToCamera = mul(PosWV, transpose((float3x3) mtxView));
	vso.vNorT_ToCamera = mul(vso.vNorT_ToCamera, transpose((float3x3) mtxWorld));
	vso.vNorT_ToCamera.x *= -1.0f;
	vso.vNorT_ToCamera = mul(vso.vNorT_ToCamera, transpose((float3x3) mtxLocal));
	vso.vNorT_ToCamera = mul(vso.vNorT_ToCamera, transpose((float3x3) mtxL));
	vso.vNorT_ToCamera = mul(vso.vNorT_ToCamera, transpose(mtxT));
	vso.vNorT_ToCamera.x *= -1.0f;
	vso.vNorT_ToCamera = normalize(-vso.vNorT_ToCamera);

	//光の情報を計算（接空間へ変換）
	float3 PosL = {
		cbLightPos.x,
		cbLightPos.y,
		cbLightPos.z
	};
	//vso.vDirT_ToLight = mul(PosL, (float3x3) mtxView);
	//vso.vDirT_ToLight = mul(vso.vDirT_ToLight, transpose((float3x3) mtxView));
	vso.vDirT_ToLight = mul(PosL, transpose((float3x3) mtxWorld));
	vso.vDirT_ToLight.x *= -1.0f;
	vso.vDirT_ToLight = mul(vso.vDirT_ToLight, transpose((float3x3) mtxLocal));
	vso.vDirT_ToLight = mul(vso.vDirT_ToLight, transpose((float3x3) mtxL));
	vso.vDirT_ToLight = mul(vso.vDirT_ToLight, transpose(mtxT));
	vso.vDirT_ToLight.x *= -1.0f;									//鏡面反射確認の為、疑似的に位置を設定
	vso.vNorT_ToLight = normalize(vso.vDirT_ToLight);				//光源への単位ベクトル

	//戻り値
	return vso;
}
