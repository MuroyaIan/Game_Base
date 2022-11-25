
//���͗p�\����
struct VS_IN
{
	float3 pos : POSITION;
};

//�萔�o�b�t�@�i�ϊ��s��j
cbuffer C_BUFFER : register(b0)
{
	matrix mtxTransform;
};

//�G���g���[�|�C���g
float4 main(VS_IN vsi) : SV_Position
{
	return mul(float4(vsi.pos, 1.0f), mtxTransform);
}
