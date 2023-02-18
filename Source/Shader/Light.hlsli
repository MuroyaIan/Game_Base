/**
 * @file Light.hlsli
 * @brief ���C�g��`
 * @author ���J�C�A��
 * @date 2023/02/10
 * @���� 2023/02/10�F�t�@�C���쐬
 */

//�\���̐錾
struct LIGHT_DIRECTIONAL	//���s����
{
	float4 Pos;				//���[���h���W
	float4 Color_D;			//���̐F�i�g�U�F, a�͋��x�j
};

struct LIGHT_POINT			//�_����
{
	float4 Pos;				//���[���h���W
	float4 Color_D;			//���̐F�i�g�U�F, a�͋��x�j

	float AttConst;
	float AttLinear;
	float AttQuadratic;		//���̌���
	float Pad;				//���u��
};

struct LIGHT_VECTOR						//�����v�Z�p�x�N�g���i���s�����j
{
	float3 vNor_Model : NORMAL;			//���f���̖@���x�N�g��
	float3 vNor_ToCamera : CAM_NOR;		//�J�����ւ̒P�ʃx�N�g��
	float3 vDir_ToLight : LIGHT_DIR;	//���ւ̕����x�N�g��
	float3 vNor_ToLight : LIGHT_NOR;	//���ւ̒P�ʃx�N�g��
};

struct LIGHT_VECTOR_PT					//�����v�Z�p�x�N�g���i�_�����A�����̕��s�����j
{
	float3 vNor_Model : NORMAL;			//���f���̖@���x�N�g��
	float3 vNor_ToCamera : CAM_NOR;		//�J�����ւ̒P�ʃx�N�g��
};

//�֐��錾

//���s�����̌v�Z
float3 CalcDirectionalLight(LIGHT_VECTOR LightVec, float3x3 ModelColor, float3 LightColor, float Shininess)
{
	//�g�U�F�Z�o
	const float3 Diffuse = LightColor * max(0.0f, dot(LightVec.vNor_ToLight, LightVec.vNor_Model)) * ModelColor._11_12_13;

	//���ʔ��ːF�Z�o
	const float3 vRef = LightVec.vNor_Model * dot(LightVec.vDir_ToLight, LightVec.vNor_Model) * 2.0f - LightVec.vDir_ToLight;	//���ʔ��˃x�N�g��
	const float3 Specular = LightColor * pow(max(0.0f, dot(normalize(vRef), LightVec.vNor_ToCamera)), Shininess) * ModelColor._21_22_23;

	//�����̌v�Z
	const float3 Ambient = LightColor * ModelColor._31_32_33;

	//�ŏI�̏o�͐F�v�Z
	return Diffuse + Specular + Ambient;
}

//�_�����̌v�Z
float3 CalcPointLight(LIGHT_VECTOR_PT LightVec, float3x3 ModelColor, LIGHT_POINT PointLight, float Shininess, float3 PosWV, float4x4 mtxView)
{
	//�o�͗p�ϐ�
	float3 Color = float3(0.0f, 0.0f, 0.0f);

	//���C�g���W�̃r���[�ϊ�
	const float4 LightPosWV = mul(PointLight.Pos, mtxView);

	//���ւ̒P�ʃx�N�g��
	const float3 vToLight = LightPosWV.xyz - PosWV;
	const float dLight = length(vToLight);
	const float3 vNor_ToLight = vToLight / dLight;

	//���̌����v�Z
	const float Att = 1.0f / (PointLight.AttConst + PointLight.AttLinear * dLight + PointLight.AttQuadratic * (dLight * dLight));

	//�_�����̐F
	const float3 Light = PointLight.Color_D.rgb * PointLight.Color_D.a * Att;

	//�g�U�F�Z�o
	const float3 Diffuse = Light * max(0.0f, dot(vNor_ToLight, LightVec.vNor_Model)) * ModelColor._11_12_13;

	//���ʔ��ːF�Z�o
	const float3 vRef = LightVec.vNor_Model * dot(vToLight, LightVec.vNor_Model) * 2.0f - vToLight;		//���ʔ��˃x�N�g��
	//const float PowerS = pow(2.0f, Texture._24 * 13.0f);												//���`�����l���t���X�y�L�����}�b�v?
	const float3 Specular = Light * pow(max(0.0f, dot(normalize(vRef), LightVec.vNor_ToCamera)), Shininess) * ModelColor._21_22_23;

	//�����̌v�Z
	const float3 Ambient = Light * ModelColor._31_32_33;

	//�F�����Z
	Color += Diffuse;
	Color += Specular;
	Color += Ambient;

	//�ŏI�̏o�͐F�v�Z
	return Color;
}
