
//���͗p�\����
struct VS_IN
{
	float3 pos : POSITION;      //���W
	float4 color : COLOR;       //���_�J���[
	matrix mtxLocal : MTX_L;    //���[�J���s��
};

//�o�͗p�\����
struct VS_OUT
{
	float4 color : COLOR;
	float4 pos : SV_Position;
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
VS_OUT main(VS_IN vsi)
{
	VS_OUT vso;

	//���W
	vso.pos = float4(vsi.pos, 1.0f);
	vso.pos.x *= -1.0f;
	vso.pos = mul(vso.pos, mtxLocal);
	vso.pos = mul(vso.pos, vsi.mtxLocal);
	vso.pos.x *= -1.0f;
	vso.pos = mul(vso.pos, mtxWorld);
	vso.pos = mul(vso.pos, mtxView);
	vso.pos = mul(vso.pos, mtxProj);

	//�J���[
	vso.color = vsi.color;

	return vso;
}
