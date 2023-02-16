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

//�G���g���[�|�C���g
float4 main(PS_IN psi) : SV_TARGET
{
	//�e�N�X�`���擾�i����n�j
	const float2x4 Texture = {
		TexMap[0].Sample(Sampler, psi.tex),		//Diffuse
		TexMap[1].Sample(Sampler, psi.tex),		//Specular
	};

	//���f���F�v�Z
	const float3x3 ModelColor = {
		matData.Diffuse.rgb * matData.Diffuse.w * Texture._11_12_13,	//Diffuse
		matData.Specular.rgb * Texture._21_22_23,						//Specular
		matData.Ambient.rgb												//Ambient
	};

	//�����v�Z�p�x�N�g��
	const LIGHT_VECTOR LightVec = {
		psi.vNorV_Model,
		psi.vNorV_ToCamera,
		psi.vDirV_ToLight,
		psi.vNorV_ToLight
	};

	//���s�����̌v�Z
	const float3 LightColor = DirectionalLight.Color_D.rgb * DirectionalLight.Color_D.a;
	const float3 Directional = CalcDirectionalLight(LightVec, ModelColor, LightColor, matData.Shininess);

	//�O���[�o�������̌v�Z
	const float3 g_Ambient = AmbientLight.rgb * AmbientLight.a * ModelColor._11_12_13;

	//�ŏI�̏o�͐F�v�Z
	return float4(saturate(Directional + g_Ambient), 1.0f);
}
