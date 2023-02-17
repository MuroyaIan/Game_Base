/**
 * @file VS_Instance.hlsl
 * @brief ���_�V�F�[�_�i�C���X�^���V���O�j
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
	matrix mtxTransform : WORLD_MTX;	//���[���h�s��
};

//�G���g���[�|�C���g
float4 main(VS_IN vsi) : SV_Position
{
	//���W�v�Z
	float4 pos = mul(float4(vsi.pos, 1.0f), vsi.mtxTransform);
	pos = mul(pos, mtxView);
	pos = mul(pos, mtxProj);

	return pos;
}
