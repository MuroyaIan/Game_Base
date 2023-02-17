/**
 * @file VS_Instance_Phong_Anim.hlsl
 * @brief ���_�V�F�[�_�i�C���X�^���V���O+Phong���f��+�A�j���[�V�����t���j
 * @author ���J�C�A��
 * @date 2023/02/17
 * @���� 2023/02/17�F�t�@�C���쐬
 */

//�C���N���[�h��
#define MODEL_ANIM
#include <VS_Instance.hlsli>

//���͗p�\����
struct VS_IN
{
	float3 pos : POSITION;			//���W
	float2 tex : TEXCOORD;			//UV���W
	float3 normal : NORMAL;			//�@��
	int boneID[4] : BONE;			//���ԍ�
	float boneWeight[4] : WEIGHT;	//����d

	matrix mtxWorld : WORLD_MTX;	//���[���h�s��
};

//�o�͗p�\����
struct VS_OUT
{
	float3 posWV : POSITION;	//���W�i�ϊ���j
	float2 tex : TEXCOORD;		//UV���W
	float3 normalWV : NORMAL;	//�@���i�ϊ���j

	matrix mtxView : MTX_V;		//�r���[�s��i�s�N�Z���V�F�[�_�p�j

	float4 pos : SV_Position;
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
	vso.pos = mul(vso.pos, mtxSkin);
	vso.pos.x *= -1.0f;						//����n��
	vso.pos = mul(vso.pos, vsi.mtxWorld);
	vso.pos = mul(vso.pos, mtxView);
	vso.posWV = vso.pos.xyz;
	vso.pos = mul(vso.pos, mtxProj);

	//UV���W
	vso.tex = vsi.tex;

	//�@��
	vso.normalWV = mul(vsi.normal, (float3x3) mtxL);
	vso.normalWV = mul(vso.normalWV, (float3x3) mtxSkin);
	vso.normalWV.x *= -1.0f;									//����n��
	vso.normalWV = mul(vso.normalWV, (float3x3) vsi.mtxWorld);
	vso.normalWV = mul(vso.normalWV, (float3x3) mtxView);

	//���̑�
	vso.mtxView = mtxView;

	return vso;
}
