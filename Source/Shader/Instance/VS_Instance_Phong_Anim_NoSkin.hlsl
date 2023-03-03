/**
 * @file VS_Instance_Phong_Anim_NoSkin.hlsl
 * @brief ���_�V�F�[�_�i�C���X�^���V���O+Phong���f��+���Ȃ��A�j���[�V�����t���j
 * @author ���J�C�A��
 * @date 2023/02/28
 * @���� 2023/02/28�F�t�@�C���쐬
 */

//�C���N���[�h��
#define MODEL_ANIM_NOSKIN
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
	int animFrame : ANIM_FRAME;		//�A�j���[�V�����Đ��t���[��
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

	//�t���[���p���Z�o
	matrix mtxL = {
		NoSkinMap.Load(int3(vsi.animFrame * 4,	   0, 0)),
		NoSkinMap.Load(int3(vsi.animFrame * 4 + 1, 0, 0)),
		NoSkinMap.Load(int3(vsi.animFrame * 4 + 2, 0, 0)),
		NoSkinMap.Load(int3(vsi.animFrame * 4 + 3, 0, 0))
	};

	//���W�v�Z
	vso.pos = mul(float4(vsi.pos, 1.0f), mtxL);
	vso.pos.x *= -1.0f;							//����n��
	vso.pos = mul(vso.pos, vsi.mtxWorld);
	vso.pos = mul(vso.pos, mtxView);
	vso.posWV = vso.pos.xyz;					//���_���W�i�r���[��ԁj�ˌ��v�Z�p
	vso.pos = mul(vso.pos, mtxProj);

	//UV���W
	vso.tex = vsi.tex;

	//�@��
	vso.vNorV_Model = mul(vsi.normal, (float3x3) mtxL);
	vso.vNorV_Model.x *= -1.0f;											//����n��
	vso.vNorV_Model = mul(vso.vNorV_Model, (float3x3) vsi.mtxWorld);
	vso.vNorV_Model = mul(vso.vNorV_Model, (float3x3) mtxView);
	vso.vNorV_Model = normalize(vso.vNorV_Model);

	//�J�����ւ̒P�ʃx�N�g���i�r���[��Ԃ֕ϊ��j
	vso.vNorV_ToCamera = normalize(-vso.posWV);

	//���̑�
	vso.mtxView = mtxView;

	return vso;
}
