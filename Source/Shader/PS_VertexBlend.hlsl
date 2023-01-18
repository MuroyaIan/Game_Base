/**
 * @file PS_VertexBlend.hlsl
 * @brief ピクセルシェーダ（頂点ブレンド）
 * @author 室谷イアン
 * @date 2023/01/18
 * @履歴 2023/01/18：ファイル作成
 */

//入力用構造体
struct VS_IN
{
	float4 color : COLOR;	//頂点色
};

//エントリーポイント
float4 main(VS_IN vsi) : SV_Target
{
	return vsi.color;
}
