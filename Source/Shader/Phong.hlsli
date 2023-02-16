/**
 * @file Phong.hlsli
 * @brief Phongモデル定義
 * @author 室谷イアン
 * @date 2023/02/16
 * @履歴 2023/02/16：ファイル作成
 */

//構造体宣言
struct MATERIAL_DATA
{
	float4 Ambient;				//環境光
	float4 Diffuse;				//拡散反射光
	float4 Emissive;			//発射光
	float4 Transparent;			//透過度
	float4 Specular;			//鏡面反射光
	float Shininess;			//光沢
	float Disp_MinLayerNum;
	float Disp_MaxLayerNum;		//視差マップのサンプリング範囲
	float Disp_DepthScale;		//視差マップの深さ係数
};
