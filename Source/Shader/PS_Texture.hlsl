
//���͗p�\����
struct VS_IN
{
	float2 tex : TEXCOORD;
};

//�T���v���[
SamplerState Sampler : register(s0);

//�e�N�X�`��
Texture2D Tex2D : register(t0);

//�G���g���[�|�C���g
float4 main(VS_IN vsi) : SV_Target
{
	return Tex2D.Sample(Sampler, vsi.tex);
}
