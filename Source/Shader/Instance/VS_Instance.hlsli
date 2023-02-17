/**
 * @file VS_Instance.hlsli
 * @brief リソース参照（インスタンシング用）
 * @author 室谷イアン
 * @date 2023/02/17
 * @履歴 2023/02/17：ファイル作成
 */

//インクルード部
#include <../Light.hlsli>

//定数バッファ（変換行列）
cbuffer CB_CAMERA : register(b0)
{
	matrix mtxView;		//ビュー行列
	matrix mtxProj;		//投影行列
};

//定数バッファ（光源情報）
cbuffer CB_LIGHT : register(b1)
{
	LIGHT_DIRECTIONAL DirectionalLight;		//平行光源
	LIGHT_POINT PointLight[16];				//点光源
	float4 AmbientLight;					//環境光
}

#ifdef MODEL_ANIM

	//定数バッファ（骨情報）
	cbuffer CB_BONE : register(b2)
	{
		matrix mtxBone[250];	//骨行列
	};

	//定数バッファ（ローカル行列）
	cbuffer CB_LOCAL : register(b3)
	{
		matrix mtxSkin;		//骨なしメッシュ用ローカル行列
	};

#endif // MODEL_ANIM
