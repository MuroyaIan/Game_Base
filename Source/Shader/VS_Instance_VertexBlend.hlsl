
//���͗p�\����
struct VS_IN
{
	float3 pos : POSITION;              //���W
	float4 color : COLOR;               //���_�J���[
	matrix mtxTransform : WORLD_MTX;    //���[���h�s��
};

//�o�͗p�\����
struct VS_OUT
{
	float4 color : COLOR;
	float4 pos : SV_Position;
};

//�萔�o�b�t�@�i�ϊ��s��j
cbuffer CB_CAMERA : register(b0)
{
	matrix mtxView;     //�r���[�s��
	matrix mtxProj;     //���e�s��
};

//�G���g���[�|�C���g
VS_OUT main(VS_IN vsi)
{
	VS_OUT vso;
	vso.pos = mul(float4(vsi.pos, 1.0f), vsi.mtxTransform);
	vso.pos = mul(vso.pos, mtxView);
	vso.pos = mul(vso.pos, mtxProj);
	vso.color = vsi.color;
	return vso;
}
