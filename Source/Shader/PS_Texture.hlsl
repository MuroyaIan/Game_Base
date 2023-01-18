/**
 * @file PS_Texture.hlsl
 * @brief �s�N�Z���V�F�[�_�i�e�N�X�`�������j
 * @author ���J�C�A��
 * @date 2023/01/18
 * @���� 2023/01/18�F�t�@�C���쐬
 */

//���͗p�\����
struct VS_IN
{
	float2 tex : TEXCOORD;	//UV���W
};

//�T���v���[
SamplerState Sampler : register(s0);

//�e�N�X�`��
Texture2D TexDiffuse : register(t0);	//Diffuse�}�b�v

//�G���g���[�|�C���g
float4 main(VS_IN vsi) : SV_Target
{
	return TexDiffuse.Sample(Sampler, vsi.tex);
}
