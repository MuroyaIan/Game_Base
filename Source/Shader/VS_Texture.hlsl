
//���͗p�\����
struct VS_IN
{
	float3 pos : POSITION;
	float2 tex : TEXCOORD;
};

//�o�͗p�\����
struct VS_OUT
{
	float2 tex : TEXCOORD;
	float4 pos : SV_Position;
};

//�萔�o�b�t�@�i�ϊ��s��j
cbuffer C_BUFFER
{
	matrix mtxTransform;
};

//�G���g���[�|�C���g
VS_OUT main(VS_IN vsi)
{
	VS_OUT vso;
	vso.pos = mul(float4(vsi.pos, 1.0f), mtxTransform);
	vso.tex = vsi.tex;
	return vso;
}
