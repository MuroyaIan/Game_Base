/**
 * @file VS_Base.hlsli
 * @brief リソース参照
 * @author 室谷イアン
 * @date 2023/01/18
 * @履歴 2023/01/18：ファイル作成
 */

//定数バッファ（変換行列）
cbuffer CB_MTX_WVP : register(b0)
{
	matrix mtxWVP;
};
