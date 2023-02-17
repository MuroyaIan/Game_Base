/**
 * @file PS_Phong.hlsl
 * @brief ���_�V�F�[�_�iPhong���f���j
 * @author ���J�C�A��
 * @date 2023/02/17
 * @���� 2023/02/17�F�t�@�C���쐬
 */

//�C���N���[�h��
#include <Light.hlsli>
#include <Phong.hlsli>

//���͗p�\����
struct PS_IN
{
	float3 posWV : POSITION;			//���W�i�r���[��ԁj
	float2 tex : TEXCOORD;				//UV���W

	float3 vNorV_Model : NORMAL;		//�@���i�r���[��ԁj
	float3 vNorV_ToCamera : CAM_NOR;	//�J�����ւ̒P�ʃx�N�g���i�r���[��ԁj
	matrix mtxView : MTX_V;				//�r���[�s��
};

//�T���v���[
SamplerState Sampler : register(s0);

//�e�N�X�`��
Texture2D TexMap[4] : register(t0);		//0:Diffuse, 1:Specular, 2:Normal, 3:Displacement

//�萔�o�b�t�@�i�}�e���A���j
cbuffer CB_MATERIAL : register(b0)
{
	MATERIAL_DATA matData;
}

//�萔�o�b�t�@�i���C�g���j
cbuffer CB_LIGHT : register(b1)
{
	LIGHT_DIRECTIONAL DirectionalLight;		//���s����
	LIGHT_POINT PointLight[16];				//�_����
	float4 AmbientLight;					//����
}

//�v���g�^�C�v�錾
float3 CalcPointLightLoop(LIGHT_VECTOR_PT LightVec, float3x3 ModelColor, float Shininess, float3 PosWV, float4x4 mtxView); //�_�����̌v�Z

//�G���g���[�|�C���g
float4 main(PS_IN psi) : SV_Target
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

	//���s�����̌v�Z
	const float3 PosL = {
		DirectionalLight.Pos.x,
		DirectionalLight.Pos.y,
		DirectionalLight.Pos.z
	};
	const float3 vDirV_ToLight = mul(PosL, (float3x3) psi.mtxView);		//���ʔ��˗p�A�^���I�Ɉʒu��ݒ�
	const float3 vNorV_ToLight = normalize(vDirV_ToLight);				//���s�����ւ̒P�ʃx�N�g��
	const LIGHT_VECTOR LightVec = {
		psi.vNorV_Model,
		psi.vNorV_ToCamera,
		vDirV_ToLight,
		vNorV_ToLight
	};
	const float3 LightColor = DirectionalLight.Color_D.rgb * DirectionalLight.Color_D.a;
	const float3 Directional = CalcDirectionalLight(LightVec, ModelColor, LightColor, matData.Shininess);

	//�_�����̌v�Z
	const LIGHT_VECTOR_PT LightVecPt = {
		psi.vNorV_Model,
		psi.vNorV_ToCamera,
	};
	const float3 Point = CalcPointLightLoop(LightVecPt, ModelColor, matData.Shininess, psi.posWV, psi.mtxView);

	//�O���[�o�������̌v�Z
	const float3 g_Ambient = AmbientLight.rgb * AmbientLight.a * ModelColor._11_12_13;

	//�ŏI�̏o�͐F�v�Z
	return float4(saturate(Directional + Point + g_Ambient), 1.0f);
}

//�_�����̌v�Z
float3 CalcPointLightLoop(LIGHT_VECTOR_PT LightVec, float3x3 ModelColor, float Shininess, float3 PosWV, float4x4 mtxView)
{
	//�o�͗p�ϐ�
	float3 Color = float3(0.0f, 0.0f, 0.0f);

	//�v�Z���[�v
	[unroll]
	for (int i = 0; i < 16; i++) {

		//�_�����̐F�����Z
		LIGHT_POINT LightPt = {
			PointLight[i].Pos,
			PointLight[i].Color_D,
			PointLight[i].AttConst,
			PointLight[i].AttLinear,
			PointLight[i].AttQuadratic,
			PointLight[i].Pad
		};
		Color += CalcPointLight(LightVec, ModelColor, LightPt, Shininess, PosWV, mtxView);
	}

	//�ŏI�̏o�͐F�v�Z
	return Color;
}
