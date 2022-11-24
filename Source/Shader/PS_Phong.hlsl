
//���͗p�\����
struct PS_IN
{
	float3 posWV : POSITION;    //���W�i�ϊ���j
	float2 tex : TEXCOORD;      //UV���W
	float3 normalWV : NORMAL;   //�@���i�ϊ���j

	matrix mtxView : MTX_V;     //�r���[�s��
};

//�\���̐錾
struct LIGHT_DIRECTIONAL    //���s����
{
	float4 Pos;             //���[���h���W
	float4 Color_D;         //���̐F�i�g�U�F, a�͋��x�j
};

struct LIGHT_POINT			//�_����
{
	float4 Pos;             //���[���h���W
	float4 Color_D;         //���̐F�i�g�U�F, a�͋��x�j

	float AttConst;
	float AttLinear;
	float AttQuadratic;     //���̌���
	float Pad;              //���u��
};

//�T���v���[
SamplerState Sampler : register(s0);

//�e�N�X�`��
Texture2D TexMap[3] : register(t0);		//0:Diffuse, 1:Specular, 2:Normal

//�萔�o�b�t�@�i���C�g���j
cbuffer CB_LIGHT : register(b1)
{
	LIGHT_DIRECTIONAL DirectionalLight;     //���s����
	LIGHT_POINT PointLight[16];             //�_����
	float4 AmbientLight;                    //����
}

//�萔�o�b�t�@�i�}�e���A���j
cbuffer CB_MATERIAL : register(b2)
{
	float4 mAmbient;        //����
	float4 mDiffuse;        //�g�U���ˌ�
	float4 mEmissive;       //���ˌ�
	float4 mTransparent;    //���ߓx
	float4 mSpecular;       //���ʔ��ˌ�
	float mShininess;       //����
	float mPad1;
	float mPad2;
	float mPad3;            //���u��
};

//�v���g�^�C�v�錾
float3 CalcDirectionalLight(PS_IN psi, float3 ModelNormal);     //���s�����̌v�Z
float3 CalcPointLight(PS_IN psi, float3 ModelNormal);           //�_�����̌v�Z

//�G���g���[�|�C���g
float4 main(PS_IN psi) : SV_Target
{
	//�@���v�Z
	const float3 vNor_Model = normalize(psi.normalWV);

	//���s�����̌v�Z
	const float3 Directional = CalcDirectionalLight(psi, vNor_Model);

	//�_�����̌v�Z
	const float3 Point = CalcPointLight(psi, vNor_Model);

	//�O���[�o�������̌v�Z
	const float3 gAmbient = AmbientLight.rgb * AmbientLight.a;

	//�ŏI�̏o�͐F�v�Z
	//return float4(saturate((Directional + Point + Ambient) * (float3) psi.material), 1.0f);
	return float4(saturate(Directional + Point + gAmbient), 1.0f) * TexMap[0].Sample(Sampler, psi.tex);
}

//���s�����̌v�Z
float3 CalcDirectionalLight(PS_IN psi, float3 ModelNormal)
{
	//���ւ̒P�ʃx�N�g��
	const float3 vToLight = mul(DirectionalLight.Pos.xyz, (float3x3) psi.mtxView);
	const float3 vNor_ToLight = normalize(vToLight);

	//���s�����̐F
	const float3 Light = DirectionalLight.Color_D.rgb * DirectionalLight.Color_D.a;

	//�g�U�F�Z�o
	const float3 Diffuse = Light * max(0.0f, dot(vNor_ToLight, ModelNormal)) * mDiffuse.rgb;

	//���ʔ��ːF�Z�o
	const float3 vRef = ModelNormal * dot(vToLight, ModelNormal) * 2.0f - vToLight;     //���ʔ��˃x�N�g��
	const float3 Specular = Light * pow(max(0.0f, dot(normalize(vRef), normalize(-psi.posWV))), mShininess) * mSpecular.rgb;

	//�����̌v�Z
	const float3 Ambient = Light * mAmbient.rgb;

	//�ŏI�̏o�͐F�v�Z
	return Diffuse + Specular + Ambient;
}

//�_�����̌v�Z
float3 CalcPointLight(PS_IN psi, float3 ModelNormal)
{
	//�o�͗p�ϐ�
	float3 Color = float3(0.0f, 0.0f, 0.0f);

	//�v�Z���[�v
	[unroll]
	for (int i = 0; i < 16; i++)
	{
		//���C�g���W�̃r���[�ϊ�
		const float4 LightPosWV = mul(PointLight[i].Pos, psi.mtxView);

		//���ւ̒P�ʃx�N�g��
		const float3 vToLight = LightPosWV.xyz - psi.posWV;
		const float dLight = length(vToLight);
		const float3 vNor_ToLight = vToLight / dLight;

		//���̌����v�Z
		const float Att = 1.0f / (PointLight[i].AttConst + PointLight[i].AttLinear * dLight + PointLight[i].AttQuadratic * (dLight * dLight));

		//�_�����̐F
		const float3 Light = PointLight[i].Color_D.rgb * PointLight[i].Color_D.a * Att;

		//�g�U�F�Z�o
		const float3 Diffuse = Light * max(0.0f, dot(vNor_ToLight, ModelNormal)) * mDiffuse.rgb;

		//���ʔ��ːF�Z�o
		const float3 vRef = ModelNormal * dot(vToLight, ModelNormal) * 2.0f - vToLight;     //���ʔ��˃x�N�g��
		const float3 Specular = Light * pow(max(0.0f, dot(normalize(vRef), normalize(-psi.posWV))), mShininess) * mSpecular.rgb;

		//�����̌v�Z
		const float3 Ambient = Light * mAmbient.rgb;

		//�F�����Z
		Color += Diffuse;
		Color += Specular;
		Color += Ambient;
	}

	//�ŏI�̏o�͐F�v�Z
	return Color;
}
