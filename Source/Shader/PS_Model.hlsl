/**
 * @file PS_Model.hlsl
 * @brief ���_�V�F�[�_�i���f�������j
 * @author ���J�C�A��
 * @date 2023/01/19
 * @���� 2023/01/19�F�t�@�C���쐬
 */

//���͗p�\����
struct PS_IN
{
	float3 posWV : POSITION;	//���W�i�ϊ���j
	float2 tex : TEXCOORD;		//UV���W
	float3 normalWV : NORMAL;	//�@���i�ϊ���j

	matrix mtxView : MTX_V;		//�r���[�s��
};

//�T���v���[
SamplerState Sampler : register(s0);

//�e�N�X�`��
Texture2D TexMap[3] : register(t0);		//0:Diffuse, 1:Specular, 2:Normal

//�萔�o�b�t�@�i�}�e���A���j
cbuffer CB_MATERIAL : register(b0)
{
	float4 cbAmbient;		//����
	float4 cbDiffuse;		//�g�U���ˌ�
	float4 cbEmissive;		//���ˌ�
	float4 cbTransparent;	//���ߓx
	float4 cbSpecular;		//���ʔ��ˌ�
	float cbShininess;		//����
	float Pad1;
	float Pad2;
	float Pad3;				//���u��
}

//�O���[�o���萔(���̕��s����)
static const float3 LightPos = { -1.0f, 1.0f, -1.0f };				//���s�����̌���
static const float4 LightColor = { 1.0f, 1.0f, 1.0f, 1.0f };		//���s�����̌�
static const float4 GlobalAmbient = { 1.0f, 1.0f, 1.0f, 0.2f };		//������

//�v���g�^�C�v�錾
float3 CalcDirectionalLight(PS_IN psi, float3 ModelNormal, float3x3 Color);		//���s�����̌v�Z

//�G���g���[�|�C���g
float4 main(PS_IN psi) : SV_TARGET
{
	//�@���v�Z
	const float3 vNor_Model = normalize(psi.normalWV);

	//�e�N�X�`���v�Z
	const float3x3 Color = {
		TexMap[0].Sample(Sampler, psi.tex).xyz,		//Diffuse
		TexMap[1].Sample(Sampler, psi.tex).xyz,		//Specular
		TexMap[2].Sample(Sampler, psi.tex).xyz		//Normal
	};

	//���s�����̌v�Z
	const float3 Directional = CalcDirectionalLight(psi, vNor_Model, Color);

	//�O���[�o�������̌v�Z
	const float3 g_Ambient = GlobalAmbient.rgb * GlobalAmbient.a * Color._11_12_13;

	//�ŏI�̏o�͐F�v�Z
	return float4(saturate(Directional + g_Ambient), 1.0f);
}

//���s�����̌v�Z
float3 CalcDirectionalLight(PS_IN psi, float3 ModelNormal, float3x3 Color)
{
	//���ւ̒P�ʃx�N�g��
	const float3 vToLight = mul(LightPos.xyz, (float3x3) psi.mtxView);
	const float3 vNor_ToLight = normalize(vToLight);

	//���s�����̐F
	const float3 Light = LightColor.rgb * LightColor.a;

	//�g�U�F�Z�o
	const float3 Diffuse = Light * max(0.0f, dot(vNor_ToLight, ModelNormal)) * cbDiffuse.rgb * Color._11_12_13;

	//���ʔ��ːF�Z�o
	const float3 vRef = ModelNormal * dot(vToLight, ModelNormal) * 2.0f - vToLight;		//���ʔ��˃x�N�g��
	const float3 Specular = Light * pow(max(0.0f, dot(normalize(vRef), normalize(-psi.posWV))), cbShininess) * cbSpecular.rgb * Color._21_22_23;

	//�����̌v�Z
	const float3 Ambient = Light * cbAmbient.rgb;

	//�ŏI�̏o�͐F�v�Z
	return Diffuse + Specular + Ambient;
}
