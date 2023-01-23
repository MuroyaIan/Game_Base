/**
 * @file PS_SingleColor.hlsl
 * @brief ピクセルシェーダ（単色付け）
 * @author 室谷イアン
 * @date 2023/01/18
 * @履歴 2023/01/18：ファイル作成
 */

//定数バッファ（ポリゴン色）
cbuffer C_BUFFER : register(b0)
{
	float4 FaceColor;	//ポリゴン用色
};

//エントリーポイント
float4 main(uint tid : SV_PrimitiveID) : SV_Target
{
	return FaceColor;
}
