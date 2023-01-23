/**
 * @file PS_Model_Normal.hlsl
 * @brief �s�N�Z���V�F�[�_�i�m�[�}���}�b�v�t�����f���j
 * @author ���J�C�A��
 * @date 2023/01/22
 * @���� 2023/01/22�F�t�@�C���쐬
 */

//�C���N���[�h��
#include <PS_Model.hlsli>

//���͗p�\����
struct PS_IN
{
	float2 tex : TEXCOORD;				//UV���W

	float3 vNorT_ToCamera : CAM_NOR;	//�J�����ւ̒P�ʃx�N�g���i�ڋ�ԁj
	float3 vDirT_ToLight : LIGHT_DIR;	//���ւ̕����x�N�g���ڋ�ԁi�ڋ�ԁj
	float3 vNorT_ToLight : LIGHT_NOR;	//���ւ̒P�ʃx�N�g���ڋ�ԁi�ڋ�ԁj
};

//�v���g�^�C�v�錾
float3 CalcDirectionalLight(PS_IN psi, float3 ModelNormal, float2x3 ModelColor);	//���s�����̌v�Z

//�G���g���[�|�C���g
float4 main(PS_IN psi) : SV_TARGET
{
	//�e�N�X�`���擾
	const float3x4 Texture = {
		TexMap[0].Sample(Sampler, psi.tex),		//Diffuse
		TexMap[1].Sample(Sampler, psi.tex),		//Specular
		TexMap[2].Sample(Sampler, psi.tex)		//Normal
	};

	//�@���擾
	float3 vNorT_Model = Texture._31_32_33 * 2.0f - 1.0f;	//�i����n�j
	vNorT_Model.x *= -1.0f;
	vNorT_Model = normalize(vNorT_Model);

	//���f���F�v�Z
	const float2x3 ModelColor = {
		cbDiffuse.rgb * cbDiffuse.w * Texture._11_12_13,	//Diffuse
		cbSpecular.rgb * Texture._21_22_23					//Specular
	};

	//���s�����̌v�Z
	const float3 Directional = CalcDirectionalLight(psi, vNorT_Model, ModelColor);

	//�O���[�o�������̌v�Z
	const float3 g_Ambient = GlobalAmbient.rgb * GlobalAmbient.a * ModelColor._11_12_13;

	//�ŏI�̏o�͐F�v�Z
	return float4(saturate(Directional + g_Ambient), 1.0f);
}

//���s�����̌v�Z
float3 CalcDirectionalLight(PS_IN psi, float3 ModelNormal, float2x3 ModelColor)
{
	//���s�����̐F
	const float3 LightRGB = LightColor.rgb * LightColor.a;

	//�g�U�F�Z�o
	const float3 Diffuse = LightRGB * max(0.0f, dot(psi.vNorT_ToLight, ModelNormal)) * ModelColor._11_12_13;

	//���ʔ��ːF�Z�o
	const float3 vRef = ModelNormal * dot(psi.vDirT_ToLight, ModelNormal) * 2.0f - psi.vDirT_ToLight;	//���ʔ��˃x�N�g��
	const float3 Specular = LightRGB * pow(max(0.0f, dot(normalize(vRef), psi.vNorT_ToCamera)), cbShininess) * ModelColor._21_22_23;

	//�����̌v�Z
	const float3 Ambient = LightRGB * cbAmbient.rgb;

	//�ŏI�̏o�͐F�v�Z
	return Diffuse + Specular + Ambient;
}
