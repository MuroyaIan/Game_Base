/**
 * @file PS_Model.hlsl
 * @brief ���_�V�F�[�_�i���f�������j
 * @author ���J�C�A��
 * @date 2023/01/19
 * @���� 2023/01/19�F�t�@�C���쐬
 */

//�C���N���[�h��
#include <PS_Model.hlsli>

//���͗p�\����
struct PS_IN
{
	float2 tex : TEXCOORD;				//UV���W

	float3 vNorV_Model : NORMAL;		//�@���i�r���[��ԁj
	float3 vNorV_ToCamera : CAM_NOR;	//�J�����ւ̒P�ʃx�N�g���i�r���[��ԁj
	float3 vDirV_ToLight : LIGHT_DIR;	//���ւ̕����x�N�g���i�r���[��ԁj
	float3 vNorV_ToLight : LIGHT_NOR;	//���ւ̒P�ʃx�N�g���i�r���[��ԁj
};

//�v���g�^�C�v�錾
float3 CalcDirectionalLight(PS_IN psi, float3 ModelNormal, float3x3 ModelColor);	//���s�����̌v�Z

//�G���g���[�|�C���g
float4 main(PS_IN psi) : SV_TARGET
{
	//�e�N�X�`���擾
	const float2x4 Texture = {
		TexMap[0].Sample(Sampler, psi.tex),		//Diffuse
		TexMap[1].Sample(Sampler, psi.tex),		//Specular
	};

	//���f���F�v�Z
	const float3x3 ModelColor = {
		cbDiffuse.rgb * cbDiffuse.w * Texture._11_12_13,	//Diffuse
		cbSpecular.rgb * Texture._21_22_23,					//Specular
		cbAmbient.rgb										//Ambient
	};

	//���s�����̌v�Z
	const float3 Directional = CalcDirectionalLight(psi, psi.vNorV_Model, ModelColor);

	//�O���[�o�������̌v�Z
	const float3 g_Ambient = GlobalAmbient.rgb * GlobalAmbient.a * ModelColor._11_12_13;

	//�ŏI�̏o�͐F�v�Z
	return float4(saturate(Directional + g_Ambient), 1.0f);
}

//���s�����̌v�Z
float3 CalcDirectionalLight(PS_IN psi, float3 ModelNormal, float3x3 ModelColor)
{
	//���s�����̐F
	const float3 LightRGB = LightColor.rgb * LightColor.a;

	//�g�U�F�Z�o
	const float3 Diffuse = LightRGB * max(0.0f, dot(psi.vNorV_ToLight, ModelNormal)) * ModelColor._11_12_13;

	//���ʔ��ːF�Z�o
	const float3 vRef = ModelNormal * dot(psi.vDirV_ToLight, ModelNormal) * 2.0f - psi.vDirV_ToLight;	//���ʔ��˃x�N�g��
	//const float PowerS = pow(2.0f, Texture._24 * 13.0f);												//���`�����l���t���X�y�L�����}�b�v?
	const float3 Specular = LightRGB * pow(max(0.0f, dot(normalize(vRef), psi.vNorV_ToCamera)), cbShininess) * ModelColor._21_22_23;

	//�����̌v�Z
	const float3 Ambient = LightRGB * ModelColor._31_32_33;

	//�ŏI�̏o�͐F�v�Z
	return Diffuse + Specular + Ambient;
}
