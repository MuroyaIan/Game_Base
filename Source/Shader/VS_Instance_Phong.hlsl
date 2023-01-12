
//���͗p�\����
struct VS_IN
{
	float3 pos : POSITION;              //���W
	float2 tex : TEXCOORD;              //UV���W
	float3 normal : NORMAL;             //�@��

	matrix mtxWorld : WORLD_MTX;        //���[���h�s��
};

//�o�͗p�\����
struct VS_OUT
{
	float3 posWV : POSITION;            //���W�i�ϊ���j
	float2 tex : TEXCOORD;              //UV���W
	float3 normalWV : NORMAL;           //�@���i�ϊ���j

	matrix mtxView : MTX_V;             //�r���[�s��i�s�N�Z���V�F�[�_�p�j

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

	//���W�v�Z
	vso.pos = float4(vsi.pos, 1.0f);
	vso.pos.x *= -1.0f;
	vso.pos = mul(vso.pos, vsi.mtxWorld);
	vso.pos = mul(vso.pos, mtxView);
	vso.posWV = vso.pos.xyz;
	vso.pos = mul(vso.pos, mtxProj);

	//�e�N�X�`��
	vso.tex = vsi.tex;

	//�@��
	vso.normalWV = vsi.normal;
	vso.normalWV.x *= -1.0f;
	vso.normalWV = mul(vso.normalWV, (float3x3) vsi.mtxWorld);
	vso.normalWV = mul(vso.normalWV, (float3x3) mtxView);

	//���̑�
	vso.mtxView = mtxView;

	return vso;
}
