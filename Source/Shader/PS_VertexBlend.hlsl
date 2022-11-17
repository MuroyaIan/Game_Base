
//入力用構造体
struct VS_IN
{
	float4 color : COLOR;
};

//エントリーポイント
float4 main(VS_IN vsi) : SV_Target
{
	return vsi.color;
}
