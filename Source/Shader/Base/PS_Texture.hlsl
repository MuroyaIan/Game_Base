/**
 * @file PS_Texture.hlsl
 * @brief ピクセルシェーダ（テクスチャ実装）
 * @author 室谷イアン
 * @date 2023/01/18
 * @履歴 2023/01/18：ファイル作成
 */

//入力用構造体
struct VS_IN
{
	float2 tex : TEXCOORD;	//UV座標
};

//サンプラー
SamplerState Sampler : register(s0);

//テクスチャ
Texture2D TexDiffuse : register(t0);	//Diffuseマップ

//エントリーポイント
float4 main(VS_IN vsi) : SV_Target
{
	return TexDiffuse.Sample(Sampler, vsi.tex);
}
