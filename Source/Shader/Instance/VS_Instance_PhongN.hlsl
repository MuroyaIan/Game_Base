/**
 * @file VS_Instance_PhongN.hlsl
 * @brief ���_�V�F�[�_�i�C���X�^���V���O+Phong���f��+�@���}�b�s���O�j
 * @author ���J�C�A��
 * @date 2023/02/18
 * @���� 2023/02/18�F�t�@�C���쐬
 */

//�C���N���[�h��
#include <VS_Instance.hlsli>

//���͗p�\����
struct VS_IN
{
	float3 pos : POSITION;			//���W
	float2 tex : TEXCOORD;			//UV���W
	float3 normal : NORMAL;			//�@��
	float3 binormal : BINORMAL;		//�]�@��
	float3 tangent : TANGENT;		//�ڐ�

	matrix mtxWorld : WORLD_MTX;	//���[���h�s��
};

//�o�͗p�\����
struct VS_OUT
{
	float3 posWV : POSITION;			//���W�i�r���[��ԁj
	float2 tex : TEXCOORD;				//UV���W

	float3 vNorV_Model : NORMAL;		//�@���i�r���[��ԁj
	float3 vNorV_ToCamera : CAM_NOR;	//�J�����ւ̒P�ʃx�N�g���i�r���[��ԁj
	matrix mtxView : MTX_V;				//�r���[�s��

	float4 pos : SV_Position;
};

//�G���g���[�|�C���g
VS_OUT main(VS_IN vsi)
{
	VS_OUT vso;

	//���W�v�Z
	vso.pos = float4(vsi.pos, 1.0f);
	vso.pos.x *= -1.0f;						//����n��
	vso.pos = mul(vso.pos, vsi.mtxWorld);
	vso.pos = mul(vso.pos, mtxView);
	vso.posWV = vso.pos.xyz;				//���_���W�i�r���[��ԁj�ˌ��v�Z�p
	vso.pos = mul(vso.pos, mtxProj);

	//UV���W
	vso.tex = vsi.tex;

	//�@���v�Z�i�r���[��Ԃ֕ϊ��j
	vso.vNorV_Model = vsi.normal;
	vso.vNorV_Model.x *= -1.0f;		//����n��
	vso.vNorV_Model = mul(vso.vNorV_Model, (float3x3) vsi.mtxWorld);
	vso.vNorV_Model = mul(vso.vNorV_Model, (float3x3) mtxView);
	vso.vNorV_Model = normalize(vso.vNorV_Model);

	//�J�����ւ̒P�ʃx�N�g���i�r���[��Ԃ֕ϊ��j
	vso.vNorV_ToCamera = normalize(-vso.posWV);

	//���̑�
	vso.mtxView = mtxView;

	return vso;
}
