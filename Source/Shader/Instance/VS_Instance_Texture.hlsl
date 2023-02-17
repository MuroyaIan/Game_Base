/**
 * @file VS_Instance_Texture.hlsl
 * @brief ���_�V�F�[�_�i�C���X�^���V���O+�e�N�X�`�������j
 * @author ���J�C�A��
 * @date 2023/02/17
 * @���� 2023/02/17�F�t�@�C���쐬
 */

//�C���N���[�h��
#include <VS_Instance.hlsli>

//���͗p�\����
struct VS_IN
{
	float3 pos : POSITION;				//���W
	float2 tex : TEXCOORD;				//UV���W
	matrix mtxTransform : WORLD_MTX;	//���[���h�s��
};

//�o�͗p�\����
struct VS_OUT
{
	float2 tex : TEXCOORD;		//UV���W
	float4 pos : SV_Position;
};

//�G���g���[�|�C���g
VS_OUT main(VS_IN vsi)
{
	//���W�v�Z
	VS_OUT vso;
	vso.pos = mul(float4(vsi.pos, 1.0f), vsi.mtxTransform);
	vso.pos = mul(vso.pos, mtxView);
	vso.pos = mul(vso.pos, mtxProj);

	//UV���W
	vso.tex = vsi.tex;

	return vso;
}
