
//���͗p�\����
struct VS_IN
{
	float4 color : COLOR;
};

//�G���g���[�|�C���g
float4 main(VS_IN vsi) : SV_Target
{
	return vsi.color;
}
