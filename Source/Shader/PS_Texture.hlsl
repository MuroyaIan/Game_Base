
//���͗p�\����
struct VS_IN
{
	float2 tex : TEXCOORD;
};

//�e�N�X�`��
Texture2D Tex2D;

//�T���v���[
SamplerState Sampler;

//�G���g���[�|�C���g
float4 main(VS_IN vsi) : SV_Target
{
	return Tex2D.Sample(Sampler, vsi.tex);
}
