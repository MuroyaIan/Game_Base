/**
 * @file PS_Default.hlsl
 * @brief �s�N�Z���V�F�[�_�i�|���S�����ƂɐF��t����j
 * @author ���J�C�A��
 * @date 2023/01/18
 * @���� 2023/01/18�F�t�@�C���쐬
 */

//�萔�o�b�t�@�i�|���S���F�j
cbuffer C_BUFFER : register(b0)
{
	float4 FaceColor[24];	//�|���S����ʗp�F�z��(24�F)
};

//�G���g���[�|�C���g
float4 main(uint tid : SV_PrimitiveID) : SV_Target
{
	return FaceColor[tid % 24];
}
