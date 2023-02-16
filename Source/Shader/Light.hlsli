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

struct LIGHT_VECTOR						//�����v�Z�p�x�N�g��
{
	float3 vNor_Model : NORMAL;			//���f���̖@���x�N�g��
	float3 vNor_ToCamera : CAM_NOR;		//�J�����ւ̒P�ʃx�N�g��
	float3 vDir_ToLight : LIGHT_DIR;	//���ւ̕����x�N�g��
	float3 vNor_ToLight : LIGHT_NOR;	//���ւ̒P�ʃx�N�g��
};

//�֐��錾

//���s�����̌v�Z
float3 CalcDirectionalLight(LIGHT_VECTOR LightVec, float3x3 ModelColor, float3 LightColor, float Shininess)
{
	//�g�U�F�Z�o
	const float3 Diffuse = LightColor * max(0.0f, dot(LightVec.vNor_ToLight, LightVec.vNor_Model)) * ModelColor._11_12_13;

	//���ʔ��ːF�Z�o
	const float3 vRef = LightVec.vNor_Model * dot(LightVec.vDir_ToLight, LightVec.vNor_Model) * 2.0f - LightVec.vDir_ToLight;	//���ʔ��˃x�N�g��
	//const float PowerS = pow(2.0f, Texture._24 * 13.0f);																		//���`�����l���t���X�y�L�����}�b�v?
	const float3 Specular = LightColor * pow(max(0.0f, dot(normalize(vRef), LightVec.vNor_ToCamera)), Shininess) * ModelColor._21_22_23;

	//�����̌v�Z
	const float3 Ambient = LightColor * ModelColor._31_32_33;

	//�ŏI�̏o�͐F�v�Z
	return Diffuse + Specular + Ambient;
}
