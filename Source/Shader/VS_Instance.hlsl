
//���͗p�\����
struct VS_IN
{
	float3 pos : POSITION;              //���W
	matrix mtxTransform : WORLD_MTX;    //���[���h�s��
};

//�萔�o�b�t�@�i�ϊ��s��j
cbuffer CB_CAMERA : register(b1)
{
	matrix mtxView;     //�r���[�s��
	matrix mtxProj;     //���e�s��
};

//�G���g���[�|�C���g
float4 main(VS_IN vsi) : SV_Position
{
	float4 pos = mul(float4(vsi.pos, 1.0f), vsi.mtxTransform);
	pos = mul(pos, mtxView);
	pos = mul(pos, mtxProj);

	return pos;
}
