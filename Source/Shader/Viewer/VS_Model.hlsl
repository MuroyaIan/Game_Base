/**
 * @file VS_Model.hlsl
 * @brief ���_�V�F�[�_�i���f�������j
 * @author ���J�C�A��
 * @date 2023/01/18
 * @���� 2023/01/18�F�t�@�C���쐬
 */

//�C���N���[�h��
#include <VS_Model.hlsli>

//���͗p�\����
struct VS_IN
{
	float3 pos : POSITION;			//���W
	float2 tex : TEXCOORD;			//UV���W
	float3 normal : NORMAL;			//�@��
	float3 binormal : BINORMAL;		//�]�@��
	float3 tangent : TANGENT;		//�ڐ�
	int boneID[4] : BONE;			//���ԍ�
	float boneWeight[4] : WEIGHT;	//����d
};

//�o�͗p�\����
struct VS_OUT
{
	float2 tex : TEXCOORD;				//UV���W

	float3 vNorV_Model : NORMAL;		//�@���i�r���[��ԁj
	float3 vNorV_ToCamera : CAM_NOR;	//�J�����ւ̒P�ʃx�N�g���i�r���[��ԁj
	float3 vDirV_ToLight : LIGHT_DIR;	//���ւ̕����x�N�g���i�r���[��ԁj
	float3 vNorV_ToLight : LIGHT_NOR;	//���ւ̒P�ʃx�N�g���i�r���[��ԁj

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
	float3 PosWV;
	vso.pos = mul(float4(vsi.pos, 1.0f), mtxL);
	vso.pos = mul(vso.pos, mtxLocal);
	vso.pos.x *= -1.0f;
	vso.pos = mul(vso.pos, mtxWorld);
	vso.pos = mul(vso.pos, mtxView);
	PosWV = vso.pos.xyz;				//���_���W�i�r���[��ԁj�ˌ��v�Z�p
	vso.pos = mul(vso.pos, mtxProj);

	//�e�N�X�`��
	vso.tex = vsi.tex;

	//�@���v�Z�i�r���[��Ԃ֕ϊ��j
	vso.vNorV_Model = mul(vsi.normal, (float3x3) mtxL);
	vso.vNorV_Model = mul(vso.vNorV_Model, (float3x3) mtxLocal);
	vso.vNorV_Model.x *= -1.0f;
	vso.vNorV_Model = mul(vso.vNorV_Model, (float3x3) mtxWorld);
	vso.vNorV_Model = mul(vso.vNorV_Model, (float3x3) mtxView);
	vso.vNorV_Model = normalize(vso.vNorV_Model);

	//�J�����ւ̒P�ʃx�N�g���i�r���[��Ԃ֕ϊ��j
	vso.vNorV_ToCamera = normalize(-PosWV);

	//���̏����v�Z�i�r���[��Ԃ֕ϊ��j
	float3 PosL = LightPos.xyz;
	PosL.x += cbLightOffset.x * 2.0f;
	PosL.y -= cbLightOffset.y * 2.0f;
	PosL.z += cbLightOffset.z * 2.0f;
	vso.vDirV_ToLight = mul(PosL, (float3x3) mtxView);	//���ʔ��ˊm�F�ׁ̈A�^���I�Ɉʒu��ݒ�
	vso.vNorV_ToLight = normalize(vso.vDirV_ToLight);	//�����ւ̒P�ʃx�N�g��

	return vso;
}
