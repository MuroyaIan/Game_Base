/**
 * @file PS_VertexBlend.hlsl
 * @brief �s�N�Z���V�F�[�_�i���_�u�����h�j
 * @author ���J�C�A��
 * @date 2023/01/18
 * @���� 2023/01/18�F�t�@�C���쐬
 */

//���͗p�\����
struct VS_IN
{
	float4 color : COLOR;	//���_�F
};

//�G���g���[�|�C���g
float4 main(VS_IN vsi) : SV_Target
{
	return vsi.color;
}
