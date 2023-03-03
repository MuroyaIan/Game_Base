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

	//テクスチャ
	Texture2D AnimMap : register(t0);		//姿勢行列（骨あり）

#endif // MODEL_ANIM

#ifdef MODEL_ANIM_NOSKIN

	//テクスチャ
	Texture2D NoSkinMap : register(t0);		//姿勢行列（骨なし）

#endif // MODEL_ANIM
