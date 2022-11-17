
//���͗p�\����
struct VS_IN
{
	float3 pos : POSITION;      //���W
	float2 tex : TEXCOORD;      //UV���W
	float3 normal : NORMAL;     //�@��
};

//�o�͗p�\����
struct VS_OUT
{
	float3 posWV : POSITION;    //���W�i�ϊ���j
	float2 tex : TEXCOORD;      //UV���W
	float3 normalWV : NORMAL;   //�@���i�ϊ���j

	matrix mtxView : MTX_V;     //�r���[�s��

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

	//���W�v�Z
	vso.pos = mul(float4(vsi.pos, 1.0f), mtxLocal);
	vso.pos.x *= -1.0f;
	vso.pos = mul(vso.pos, mtxWorld);
	vso.pos = mul(vso.pos, mtxView);
	vso.posWV = vso.pos.xyz;
	vso.pos = mul(vso.pos, mtxProj);

	//�e�N�X�`��
	vso.tex = vsi.tex;

	//�@��
	vso.normalWV = mul(vsi.normal, (float3x3) mtxLocal);
	vso.normalWV.x *= -1.0f;
	vso.normalWV = mul(vso.normalWV, (float3x3) mtxWorld);
	vso.normalWV = mul(vso.normalWV, (float3x3) mtxView);

	//���̑�
	vso.mtxView = mtxView;

	return vso;
}
