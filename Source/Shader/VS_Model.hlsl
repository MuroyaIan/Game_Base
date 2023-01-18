/**
 * @file VS_Model.hlsl
 * @brief ���_�V�F�[�_�i���f�������j
 * @author ���J�C�A��
 * @date 2023/01/18
 * @���� 2023/01/18�F�t�@�C���쐬
 */

//���͗p�\����
struct VS_IN
{
	float3 pos : POSITION;			//���W
	float2 tex : TEXCOORD;			//UV���W
	float3 normal : NORMAL;			//�@��
	int boneID[4] : BONE;			//���ԍ�
	float boneWeight[4] : WEIGHT;	//����d
};

//�o�͗p�\����
struct VS_OUT
{
	float3 posWV : POSITION;	//���W�i�ϊ���j
	float2 tex : TEXCOORD;		//UV���W
	float3 normalWV : NORMAL;	//�@���i�ϊ���j

	matrix mtxView : MTX_V;		//�r���[�s��

	float4 pos : SV_Position;
};

//�萔�o�b�t�@�i�ϊ��s��j
cbuffer CB_MTX_LWVP : register(b0)
{
	matrix mtxLocal;	//���[�J���s��(���Ȃ����b�V���̕ϊ�)
	matrix mtxWorld;	//���[���h�s��
	matrix mtxView;		//�r���[�s��
	matrix mtxProj;		//���e�s��
};

//�萔�o�b�t�@�i�����j
cbuffer CB_BONE : register(b1)
{
	matrix mtxBone[250];	//���s��
};

//�G���g���[�|�C���g
VS_OUT main(VS_IN vsi)
{
	VS_OUT vso;

	//�t���[���p���Z�o
	matrix mtxL = {
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	};
	[unroll]
	for (int i = 0; i < 4; i++)
		mtxL += mtxBone[vsi.boneID[i]] * vsi.boneWeight[i];		//�e�����鍜�s��̉��Z

	//���W�v�Z
	vso.pos = mul(float4(vsi.pos, 1.0f), mtxL);
	vso.pos = mul(vso.pos, mtxLocal);
	vso.pos.x *= -1.0f;
	vso.pos = mul(vso.pos, mtxWorld);
	vso.pos = mul(vso.pos, mtxView);
	vso.posWV = vso.pos.xyz;
	vso.pos = mul(vso.pos, mtxProj);

	//�e�N�X�`��
	vso.tex = vsi.tex;

	//�@��
	vso.normalWV = mul(vsi.normal, (float3x3) mtxL);
	vso.normalWV = mul(vso.normalWV, (float3x3) mtxLocal);
	vso.normalWV.x *= -1.0f;
	vso.normalWV = mul(vso.normalWV, (float3x3) mtxWorld);
	vso.normalWV = mul(vso.normalWV, (float3x3) mtxView);

	//���̑�
	vso.mtxView = mtxView;

	return vso;
}
