
//�萔�o�b�t�@�i�|���S���F�j
cbuffer C_BUFFER
{
	float4 FaceColor[24];
};

//�G���g���[�|�C���g
float4 main(uint tid : SV_PrimitiveID) : SV_Target
{
	return FaceColor[tid % 24];
}
