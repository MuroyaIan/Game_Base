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
	float2 tex : TEXCOORD;				//UV座標

	float3 vDirT_ToLight : LIGHT_DIR;	//光への方向ベクトル(接空間）
	float3 vNorT_ToLight : LIGHT_NOR;	//光への法線ベクトル(接空間）
	float3 vNorT_ToCamera : CAM_NOR;	//カメラへの法線ベクトル(接空間）

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

//定数バッファ（光源制御）
cbuffer CB_LIGHT_CTRL : register(b2)
{
	float4 cbLightOffset;	//光源の変化量
}

//グローバル定数(仮の平行光源)
static const float3 LightPos = { -1.0f, 1.0f, -1.0f };	//平行光源の向き

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
	float3 PosWV;									//頂点座標(ビュー空間)⇒光計算用
	vso.pos = mul(float4(vsi.pos, 1.0f), mtxL);
	vso.pos = mul(vso.pos, mtxLocal);
	vso.pos.x *= -1.0f;
	vso.pos = mul(vso.pos, mtxWorld);
	vso.pos = mul(vso.pos, mtxView);
	PosWV = vso.pos.xyz;
	vso.pos = mul(vso.pos, mtxProj);

	//テクスチャ
	vso.tex = vsi.tex;

	//変換行列(接空間⇒ビュー空間）
	//vso.mtxTtoV._11_12_13 = vsi.tangent;
	//vso.mtxTtoV._21_22_23 = vsi.binormal;
	//vso.mtxTtoV._31_32_33 = vsi.normal;
	//vso.mtxTtoV = mul(vso.mtxTtoV, (float3x3) mtxL);
	//vso.mtxTtoV = mul(vso.mtxTtoV, (float3x3) mtxLocal);
	//vso.mtxTtoV._11 *= -1.0f;
	//vso.mtxTtoV._21 *= -1.0f;
	//vso.mtxTtoV._31 *= -1.0f;
	//vso.mtxTtoV = mul(vso.mtxTtoV, (float3x3) mtxWorld);
	//vso.mtxTtoV = mul(vso.mtxTtoV, (float3x3) mtxView);

	//BTN行列を取得
	float3x3 mtxT = {
		vsi.tangent,
		vsi.binormal,
		vsi.normal
	};

	//光への単位ベクトル(接空間へ変換)
	float3 PosL = LightPos.xyz;
	PosL.x += cbLightOffset.x * 2.0f;
	PosL.y -= cbLightOffset.y * 2.0f;
	PosL.z += cbLightOffset.z * 2.0f;
	vso.vDirT_ToLight = mul(PosL, (float3x3) mtxView);
	vso.vDirT_ToLight = mul(vso.vDirT_ToLight, transpose((float3x3) mtxView));
	vso.vDirT_ToLight = mul(vso.vDirT_ToLight, transpose((float3x3) mtxWorld));
	vso.vDirT_ToLight.x *= -1.0f;
	vso.vDirT_ToLight = mul(vso.vDirT_ToLight, transpose(mtxT)); //平行光源の場合、疑似的に位置を単位ベクトルで設定
	vso.vDirT_ToLight.x *= -1.0f;
	vso.vNorT_ToLight = normalize(vso.vDirT_ToLight);	//光源への法線ベクトル

	//頂点から注視点への方向ベクトル(接空間へ変換)
	vso.vNorT_ToCamera = mul(PosWV, transpose((float3x3) mtxView));
	vso.vNorT_ToCamera = mul(vso.vNorT_ToCamera, transpose((float3x3) mtxWorld));
	vso.vNorT_ToCamera.x *= -1.0f;
	vso.vNorT_ToCamera = mul(vso.vNorT_ToCamera, transpose(mtxT));
	vso.vNorT_ToCamera.x *= -1.0f;
	vso.vNorT_ToCamera = normalize(-vso.vNorT_ToCamera);

	//戻り値
	return vso;
}
