/**
 * @file VS_Model_Normal.hlsl
 * @brief ���_�V�F�[�_�i�m�[�}���}�b�v�t�����f���j
 * @author ���J�C�A��
 * @date 2023/01/22
 * @���� 2023/01/22�F�t�@�C���쐬
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

	float3 vNorT_ToCamera : CAM_NOR;	//�J�����ւ̒P�ʃx�N�g���i�ڋ�ԁj
	float3 vDirT_ToLight : LIGHT_DIR;	//���ւ̕����x�N�g���i�ڋ�ԁj
	float3 vNorT_ToLight : LIGHT_NOR;	//���ւ̒P�ʃx�N�g���i�ڋ�ԁj

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

	//BTN�s����擾�i����n�j
	float3x3 mtxT = {
		vsi.tangent,
		vsi.binormal,
		vsi.normal
	};

	//�J�����ւ̒P�ʃx�N�g���i�ڋ�Ԃ֕ϊ��j
	vso.vNorT_ToCamera = mul(PosWV, transpose((float3x3) mtxView));
	vso.vNorT_ToCamera = mul(vso.vNorT_ToCamera, transpose((float3x3) mtxWorld));
	vso.vNorT_ToCamera.x *= -1.0f;
	vso.vNorT_ToCamera = mul(vso.vNorT_ToCamera, transpose((float3x3) mtxLocal));
	vso.vNorT_ToCamera = mul(vso.vNorT_ToCamera, transpose((float3x3) mtxL));
	vso.vNorT_ToCamera = mul(vso.vNorT_ToCamera, transpose(mtxT));
	vso.vNorT_ToCamera.x *= -1.0f;
	vso.vNorT_ToCamera = normalize(-vso.vNorT_ToCamera);

	//���̏����v�Z�i�ڋ�Ԃ֕ϊ��j
	float3 PosL = {
		cbLightPos.x,
		cbLightPos.y,
		cbLightPos.z
	};
	//vso.vDirT_ToLight = mul(PosL, (float3x3) mtxView);
	//vso.vDirT_ToLight = mul(vso.vDirT_ToLight, transpose((float3x3) mtxView));
	vso.vDirT_ToLight = mul(PosL, transpose((float3x3) mtxWorld));
	vso.vDirT_ToLight.x *= -1.0f;
	vso.vDirT_ToLight = mul(vso.vDirT_ToLight, transpose((float3x3) mtxLocal));
	vso.vDirT_ToLight = mul(vso.vDirT_ToLight, transpose((float3x3) mtxL));
	vso.vDirT_ToLight = mul(vso.vDirT_ToLight, transpose(mtxT));
	vso.vDirT_ToLight.x *= -1.0f;									//���ʔ��ˊm�F�ׁ̈A�^���I�Ɉʒu��ݒ�
	vso.vNorT_ToLight = normalize(vso.vDirT_ToLight);				//�����ւ̒P�ʃx�N�g��

	//�߂�l
	return vso;
}
