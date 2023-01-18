/**
 * @file PS_Default.hlsl
 * @brief ピクセルシェーダ（ポリゴンごとに色を付ける）
 * @author 室谷イアン
 * @date 2023/01/18
 * @履歴 2023/01/18：ファイル作成
 */

//定数バッファ（ポリゴン色）
cbuffer C_BUFFER : register(b0)
{
	float4 FaceColor[24];	//ポリゴン区別用色配列(24色)
};

//エントリーポイント
float4 main(uint tid : SV_PrimitiveID) : SV_Target
{
	return FaceColor[tid % 24];
}
