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
	//UV���W���v�Z
	float height_scale = 0.1f;

	//�T���v�����O���ݒ�i���������ɉ����ă��C���w�𕪊�����j
	const float minLayers = 8;
	const float maxLayers = 32;
	float numLayers = lerp(maxLayers, minLayers, abs(dot(float3(0.0f, 0.0f, 1.0f), psi.vNorT_ToCamera)));



	// calculate the size of each layer
	float layerDepth = 1.0f / numLayers; //�w���Ƃ̐[���̊���
	// depth of current layer
	float currentLayerDepth = 0.0f;
	// the amount to shift the texture coordinates per layer (from vector P)
	//float2 vParallax = psi.vNorT_ToCamera.xy * height_scale;
	float2 vParallax = psi.vNorT_ToCamera.xy / psi.vNorT_ToCamera.z * height_scale;
	float2 deltaTexCoords = vParallax / numLayers; //�w���Ƃ̎����x�N�g��



	// get initial values
	float2 currentTexCoords = psi.tex;
	float currentDepthMapValue = TexMap[3].Sample(Sampler, currentTexCoords).r;

	[loop][fastopt]
	while (currentLayerDepth < currentDepthMapValue)
	{
		// shift texture coordinates along direction of P
		currentTexCoords += deltaTexCoords;
		// get depthmap value at current texture coordinates
		currentDepthMapValue = TexMap[3].SampleLevel(Sampler, currentTexCoords, 0).r;
		// get depth of next layer
		currentLayerDepth += layerDepth;
	}



	// get texture coordinates before collision (reverse operations)
	float2 prevTexCoords = currentTexCoords - deltaTexCoords;

	// get depth after and before collision for linear interpolation
	float afterDepth = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = TexMap[3].Sample(Sampler, prevTexCoords).r - currentLayerDepth + layerDepth;

	// interpolation of texture coordinates
	float weight = afterDepth / (afterDepth - beforeDepth);
	float2 texDisp = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

	//float height = TexMap[3].Sample(Sampler, psi.tex).r;
	//float2 vParallax = psi.vNorT_ToCamera.xy / psi.vNorT_ToCamera.z * (height * height_scale);
	//float2 texDisp = psi.tex + vParallax;

	//UV���W�m�F�i���ߕ��̃s�N�Z����j������j�˕��ʂ̏ꍇ�͂���ŉ���
	texDisp = texDisp > 1.0f ? -1.0f : texDisp;
	texDisp = texDisp < 0.0f ? -1.0f : texDisp;
	clip(texDisp);



	//float2 texDisp = psi.tex;

	//�e�N�X�`���擾
	const float3x4 Texture = {
		TexMap[0].Sample(Sampler, texDisp),		//Diffuse
		TexMap[1].Sample(Sampler, texDisp),		//Specular
		TexMap[2].Sample(Sampler, texDisp)		//Normal
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
