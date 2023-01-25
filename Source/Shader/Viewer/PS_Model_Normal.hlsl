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
float3 CalcDirectionalLight(PS_IN psi, float3 ModelNormal, float3x3 ModelColor);	//���s�����̌v�Z
float2 CalcDisplacementMapUV(PS_IN psi);											//�����}�b�v��UV���v�Z

//�G���g���[�|�C���g
float4 main(PS_IN psi) : SV_TARGET
{
	//UV���W���v�Z
	float2 TexDisp = psi.tex;

#ifdef DISP_MAP

	TexDisp = CalcDisplacementMapUV(psi);

#endif // DISP_MAP

	//�e�N�X�`���擾
	const float3x4 Texture = {
		TexMap[0].Sample(Sampler, TexDisp),		//Diffuse
		TexMap[1].Sample(Sampler, TexDisp),		//Specular
		TexMap[2].Sample(Sampler, TexDisp)		//Normal
	};

	//�@���擾
	float3 vNorT_Model = Texture._31_32_33 * 2.0f - 1.0f;	//�i����n�j
	vNorT_Model.x *= -1.0f;
	vNorT_Model = normalize(vNorT_Model);

	//���f���F�v�Z
	const float3x3 ModelColor = {
		cbDiffuse.rgb * cbDiffuse.w * Texture._11_12_13,	//Diffuse
		cbSpecular.rgb * Texture._21_22_23,					//Specular
		cbAmbient.rgb										//Ambient
	};

	//���s�����̌v�Z
	const float3 Directional = CalcDirectionalLight(psi, vNorT_Model, ModelColor);

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
	const float3 Diffuse = LightRGB * max(0.0f, dot(psi.vNorT_ToLight, ModelNormal)) * ModelColor._11_12_13;

	//���ʔ��ːF�Z�o
	const float3 vRef = ModelNormal * dot(psi.vDirT_ToLight, ModelNormal) * 2.0f - psi.vDirT_ToLight;	//���ʔ��˃x�N�g��
	const float3 Specular = LightRGB * pow(max(0.0f, dot(normalize(vRef), psi.vNorT_ToCamera)), cbShininess) * ModelColor._21_22_23;

	//�����̌v�Z
	const float3 Ambient = LightRGB * ModelColor._31_32_33;

	//�ŏI�̏o�͐F�v�Z
	return Diffuse + Specular + Ambient;
}

//�����}�b�v��UV���v�Z
float2 CalcDisplacementMapUV(PS_IN psi)
{
	//�T���v�����O���ݒ�i���������ɉ����Đ[���v�Z�p���C���̕�������ݒ�j
	const float MinLayerNum = cbDisp_MinLayerNum;
	const float MaxLayerNum = cbDisp_MaxLayerNum;
	const float LayerNum = lerp(MinLayerNum, MaxLayerNum, abs(dot(float3(0.0f, 0.0f, 1.0f), psi.vNorT_ToCamera)));

	//���[�v�v�Z�p�̃f�[�^��ݒ�
	float Depth_Layer = 0.0f;																//���C���[��
	const float DepthPerLayer = 1.0f / LayerNum;											//���C�����Ƃ̐[��
	const float DepthScale = cbDisp_DepthScale;												//�����}�b�v�p�[���W��
	//float2 vParallax = psi.vNorT_ToCamera.xy * DepthScale;
	const float2 vParallax = psi.vNorT_ToCamera.xy / psi.vNorT_ToCamera.z * DepthScale;		//�����x�N�g���iUV���W�̑��ω��ʁj
	const float2 vParallaxPerLayer = vParallax / LayerNum;									//���C�����Ƃ̎����x�N�g��
	float2 CurrentUV = psi.tex;																//UV���W
	float Depth_DispMap = TexMap[3].Sample(Sampler, CurrentUV).r;							//�����}�b�v����擾�����[��

	//�[���v�Z
	[loop][fastopt]
	while (Depth_Layer < Depth_DispMap) {

		//���C�����Ƃ̃��C���[���Ǝ����}�b�v�[�����r���A�t�]����ӏ������
		Depth_Layer += DepthPerLayer;
		CurrentUV += vParallaxPerLayer;
		Depth_DispMap = TexMap[3].SampleLevel(Sampler, CurrentUV, 0.0f).r;
	}

	//�t�]���O�ƒ���̔�r�����王���x�N�g���̔�d�����߂�
	const float AfterDepth = Depth_Layer - Depth_DispMap;
	const float2 PrevUV = CurrentUV - vParallaxPerLayer;											//�t�]���O��UV���W
	const float BeforeDepth = TexMap[3].Sample(Sampler, PrevUV).r - Depth_Layer + DepthPerLayer;
	const float Weight = AfterDepth / (AfterDepth + BeforeDepth);

	//UV���W���m��
	float2 TexDisp = PrevUV * Weight + CurrentUV * (1.0f - Weight);

	//UV���W�͈͊m�F�i���ߕ��̃s�N�Z����j������j�˕��ʂ̏ꍇ�͂���ŉ���
	TexDisp = TexDisp > 1.0f ? -1.0f : TexDisp;
	TexDisp = TexDisp < 0.0f ? -1.0f : TexDisp;
	clip(TexDisp);

	return TexDisp;
}
