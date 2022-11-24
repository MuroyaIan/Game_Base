
//���͗p�\����
struct PS_IN
{
	float3 posWV : POSITION;    //���W�i�ϊ���j
	float2 tex : TEXCOORD;      //UV���W
	float3 normalWV : NORMAL;   //�@���i�ϊ���j

	matrix mtxView : MTX_V;     //�r���[�s��
};

//�T���v���[
SamplerState Sampler : register(s0);

//�e�N�X�`��
Texture2D TexMap[3] : register(t0);		//0:Diffuse, 1:Specular, 2:Normal

//�萔�o�b�t�@�i�}�e���A���j
cbuffer CB_MATERIAL : register(b2)
{
	float4 cbAmbient;       //����
	float4 cbDiffuse;       //�g�U���ˌ�
	float4 cbEmissive;      //���ˌ�
	float4 cbTransparent;   //���ߓx
	float4 cbSpecular;      //���ʔ��ˌ�
	float cbShininess;      //����
	float Pad1;
	float Pad2;
	float Pad3;             //���u��
}

//�O���[�o���萔
static const float3 LightPos = { -1.0f, 1.0f, -1.0f };              //���s�����̌���
static const float4 LightColorD = { 1.0f, 1.0f, 1.0f, 1.0f };       //���s�����̊g�U���ˌ�
static const float4 LightColorS = { 1.0f, 1.0f, 1.0f, 1.0f };       //���s�����̋��ʔ��ˌ�
static const float4 LightColorA = { 1.0f, 1.0f, 1.0f, 1.0f };       //���s�����̊���
static const float4 GlobalAmbient = { 1.0f, 1.0f, 1.0f, 0.2f };     //������

//�v���g�^�C�v�錾
float3 CalcDirectionalLight(PS_IN psi, float3 ModelNormal, float3 ColorD); //���s�����̌v�Z

//�G���g���[�|�C���g
float4 main(PS_IN psi) : SV_TARGET
{
	//�@���v�Z
	const float3 vNor_Model = normalize(psi.normalWV);

	//�e�N�X�`���v�Z
	const float4 TexD = TexMap[0].Sample(Sampler, psi.tex);
	const float3 ColorD = TexD.rgb * TexD.a;
	const float4 TexS = TexMap[1].Sample(Sampler, psi.tex);
	const float3 ColorS = TexS.rgb * TexS.a;

	//���s�����̌v�Z
	const float3 Directional = CalcDirectionalLight(psi, vNor_Model, ColorD);

	//�O���[�o�������̌v�Z
	const float3 g_Ambient = GlobalAmbient.rgb * GlobalAmbient.a * ColorD;

	//�ŏI�̏o�͐F�v�Z
	return float4(saturate(Directional + g_Ambient), 1.0f);
}

//���s�����̌v�Z
float3 CalcDirectionalLight(PS_IN psi, float3 ModelNormal, float3 ColorD)
{
	//���ւ̒P�ʃx�N�g��
	const float3 vToLight = mul(LightPos.xyz, (float3x3) psi.mtxView);
	const float3 vNor_ToLight = normalize(vToLight);

	//�g�U�F�Z�o
	const float3 Diffuse = LightColorD.rgb * LightColorD.a * max(0.0f, dot(vNor_ToLight, ModelNormal)) * cbDiffuse.rgb * ColorD.rgb;

	//���ʔ��ːF�Z�o
	const float3 vRef = ModelNormal * dot(vToLight, ModelNormal) * 2.0f - vToLight;     //���ʔ��˃x�N�g��
	const float3 Specular = LightColorS.rgb * LightColorS.a * pow(max(0.0f, dot(normalize(vRef), normalize(-psi.posWV))), cbShininess) * cbSpecular.rgb;

	//�����̌v�Z
	const float3 Ambient = LightColorA.rgb * LightColorA.a * cbAmbient.rgb;

	//�ŏI�̏o�͐F�v�Z
	return Diffuse + Specular + Ambient;
}
