/**
 * @file VS_Model_Bone.hlsl
 * @brief ���_�V�F�[�_�i���f�����`��p�j
 * @author ���J�C�A��
 * @date 2023/01/19
 * @���� 2023/01/19�F�t�@�C���쐬
 */

//�C���N���[�h��
#include <../Base/VS_Base.hlsli>

//���͗p�\����
struct VS_IN
{
	float3 pos : POSITION;		//���W
	float4 color : COLOR;		//���_�J���[
	matrix mtxBone : MTX_L;		//���̎p���s��i�C���X�^���V���O�j
};

//�o�͗p�\����
struct VS_OUT
{
	float4 color : COLOR;		//���_�J���[
	float4 pos : SV_Position;
};

//�G���g���[�|�C���g
VS_OUT main(VS_IN vsi)
{
	VS_OUT vso;

	//���W
	vso.pos = float4(vsi.pos, 1.0f);
	vso.pos.x *= -1.0f;						//�E��n�w�i�s���~�b�h�^�̓����������ׁ̈j
	vso.pos = mul(vso.pos, vsi.mtxBone);	//���ϊ�
	vso.pos.x *= -1.0f;						//����n�w
	vso.pos = mul(vso.pos, mtxWVP);

	//�J���[
	vso.color = vsi.color;

	return vso;
}
