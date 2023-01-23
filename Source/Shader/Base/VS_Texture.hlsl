/**
 * @file VS_Texture.hlsl
 * @brief ���_�V�F�[�_�i�e�N�X�`�������j
 * @author ���J�C�A��
 * @date 2023/01/18
 * @���� 2023/01/18�F�t�@�C���쐬
 */

//�C���N���[�h��
#include <VS_Base.hlsli>

//���͗p�\����
struct VS_IN
{
	float3 pos : POSITION;	//���W
	float2 tex : TEXCOORD;	//UV���W
};

//�o�͗p�\����
struct VS_OUT
{
	float2 tex : TEXCOORD;		//UV���W
	float4 pos : SV_Position;	//�ϊ��ύ��W
};

//�G���g���[�|�C���g
VS_OUT main(VS_IN vsi)
{
	VS_OUT vso;
	vso.pos = mul(float4(vsi.pos, 1.0f), mtxWVP);
	vso.tex = vsi.tex;
	return vso;
}
