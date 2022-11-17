
//���͗p�\����
struct VS_IN
{
	float3 pos : POSITION;      //���W
	matrix mtxLocal : MTX_L;    //���[�J���s��
};

//�萔�o�b�t�@�i�ϊ��s��j
cbuffer C_BUFFER
{
	matrix mtxLocal;    //���[�J���s��
	matrix mtxWorld;    //���[���h�s��
	matrix mtxView;     //�r���[�s��
	matrix mtxProj;     //���e�s��
};

//�G���g���[�|�C���g
float4 main(VS_IN vsi) : SV_Position
{
	float4 pos = float4(vsi.pos, 1.0f);
	pos.x *= -1.0f;
	pos = mul(pos, mtxLocal);
	pos = mul(pos, vsi.mtxLocal);
	pos.x *= -1.0f;
	pos = mul(pos, mtxWorld);
	pos = mul(pos, mtxView);
	pos = mul(pos, mtxProj);

	return pos;
}
