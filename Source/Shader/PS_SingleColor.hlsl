
//定数バッファ（ポリゴン色）
cbuffer C_BUFFER : register(b0)
{
	float4 FaceColor;
};

//エントリーポイント
float4 main(uint tid : SV_PrimitiveID) : SV_Target
{
	return FaceColor;
}
