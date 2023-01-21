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
	float3 posWV : POSITION;		//���W�i�ϊ���j
	float2 tex : TEXCOORD;			//UV���W

	float3x3 mtxTtoV : MTX_TWV;		//�ϊ��s��(�ڋ�ԁ˃r���[��ԁj
	float3x3 mtxView : MTX_V;		//�r���[�s��
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
float3 CalcDirectionalLight(PS_IN psi, float3 ModelNormal, float2x3 ModelColor); //���s�����̌v�Z

//�G���g���[�|�C���g
float4 main(PS_IN psi) : SV_TARGET
{
	//�e�N�X�`���擾
	const float3x4 Texture = {
		TexMap[0].Sample(Sampler, psi.tex),		//Diffuse
		TexMap[1].Sample(Sampler, psi.tex),		//Specular
		TexMap[2].Sample(Sampler, psi.tex)		//Normal
	};

	//�@���v�Z
	//const float3 vNor_Model = normalize(psi.normalWV);
	float3 vNor_Model = Texture._31_32_33 * 2.0f - 1.0f;
	vNor_Model = mul(vNor_Model, psi.mtxTtoV);
	vNor_Model = normalize(vNor_Model);

	//���f���F�v�Z
	const float2x3 ModelColor = {
		cbDiffuse.rgb * cbDiffuse.w * Texture._11_12_13,	//Diffuse
		cbSpecular.rgb * Texture._21_22_23					//Specular
	};

	//���s�����̌v�Z
	const float3 Directional = CalcDirectionalLight(psi, vNor_Model, ModelColor);

	//�O���[�o�������̌v�Z
	const float3 g_Ambient = GlobalAmbient.rgb * GlobalAmbient.a * ModelColor._11_12_13;

	//�ŏI�̏o�͐F�v�Z
	return float4(saturate(Directional + g_Ambient), 1.0f);
}

//���s�����̌v�Z
float3 CalcDirectionalLight(PS_IN psi, float3 ModelNormal, float2x3 ModelColor)
{
	//���ւ̒P�ʃx�N�g��
	float3 PosL = LightPos.xyz;
	PosL.x += Pad1 * 2.0f;
	PosL.y -= Pad2 * 2.0f;
	PosL.z += Pad3 * 2.0f;
	const float3 vToLight = mul(PosL, psi.mtxView);
	const float3 vNor_ToLight = normalize(vToLight);

	//���s�����̐F
	const float3 Light = LightColor.rgb * LightColor.a;

	//�g�U�F�Z�o
	const float3 Diffuse = Light * max(0.0f, dot(vNor_ToLight, ModelNormal)) * ModelColor._11_12_13;

	//���ʔ��ːF�Z�o
	const float3 vRef = ModelNormal * dot(vToLight, ModelNormal) * 2.0f - vToLight;		//���ʔ��˃x�N�g��
	//const float PowerS = pow(2.0f, Texture._24 * 13.0f);								//���`�����l���t���X�y�L�����}�b�v?
	const float3 Specular = Light * pow(max(0.0f, dot(normalize(vRef), normalize(-psi.posWV))), cbShininess) * ModelColor._21_22_23;

	//�����̌v�Z
	const float3 Ambient = Light * cbAmbient.rgb;

	//�ŏI�̏o�͐F�v�Z
	return Diffuse + Specular + Ambient;
}
