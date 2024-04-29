/**
 * @file VS_Default.hlsl
 * @brief ���_�V�F�[�_�i���W�̂݁j
 * @author ���J�C�A��
 * @date 2023/01/18
 * @���� 2023/01/18�F�t�@�C���쐬
 */

//�C���N���[�h��
#include "VS_Base.hlsli"

//���͗p�\����
struct VS_IN
{
	float3 pos : POSITION;	//���W
};

//�G���g���[�|�C���g
float4 main(VS_IN vsi) : SV_Position
{
	return mul(float4(vsi.pos, 1.0f), mtxWVP);
}
