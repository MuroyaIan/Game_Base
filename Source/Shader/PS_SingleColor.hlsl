
//�萔�o�b�t�@�i�|���S���F�j
cbuffer C_BUFFER : register(b0)
{
	float4 FaceColor;
};

//�G���g���[�|�C���g
float4 main(uint tid : SV_PrimitiveID) : SV_Target
{
	return FaceColor;
}
