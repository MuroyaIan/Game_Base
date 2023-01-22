/**
 * @file VS_Model.hlsl
 * @brief ���_�V�F�[�_�i���f�������j
 * @author ���J�C�A��
 * @date 2023/01/18
 * @���� 2023/01/18�F�t�@�C���쐬
 */

//���͗p�\����
struct VS_IN
{
	float3 pos : POSITION;			//���W
	float2 tex : TEXCOORD;			//UV���W
	float3 normal : NORMAL;			//�@��
	float3 binormal : BINORMAL;		//�]�@��
	float3 tangent : TANGENT;		//�ڐ�
	int boneID[4] : BONE;			//���ԍ�
	float boneWeight[4] : WEIGHT;	//����d
};

//�o�͗p�\����
struct VS_OUT
{
	float2 tex : TEXCOORD;				//UV���W

	float3 vDirT_ToLight : LIGHT_DIR;	//���ւ̕����x�N�g��(�ڋ�ԁj
	float3 vNorT_ToLight : LIGHT_NOR;	//���ւ̖@���x�N�g��(�ڋ�ԁj
	float3 vNorT_ToCamera : CAM_NOR;	//�J�����ւ̖@���x�N�g��(�ڋ�ԁj

	float4 pos : SV_Position;
};

//�萔�o�b�t�@�i�ϊ��s��j
cbuffer CB_MTX_LWVP : register(b0)
{
	matrix mtxLocal;	//���[�J���s��(���Ȃ����b�V���̕ϊ�)
	matrix mtxWorld;	//���[���h�s��
	matrix mtxView;		//�r���[�s��
	matrix mtxProj;		//���e�s��
};

//�萔�o�b�t�@�i�����j
cbuffer CB_BONE : register(b1)
{
	matrix mtxBone[250];	//���s��
};

//�萔�o�b�t�@�i��������j
cbuffer CB_LIGHT_CTRL : register(b2)
{
	float4 cbLightOffset;	//�����̕ω���
}

//�O���[�o���萔(���̕��s����)
static const float3 LightPos = { -1.0f, 1.0f, -1.0f };	//���s�����̌���

//�G���g���[�|�C���g
VS_OUT main(VS_IN vsi)
{
	VS_OUT vso;

	//�t���[���p���Z�o
	matrix mtxL = {
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	};
	[unroll]
	for (int i = 0; i < 4; i++)
		mtxL += mtxBone[vsi.boneID[i]] * vsi.boneWeight[i];		//�e�����鍜�s��̉��Z

	//���W�v�Z
	float3 PosWV;									//���_���W(�r���[���)�ˌ��v�Z�p
	vso.pos = mul(float4(vsi.pos, 1.0f), mtxL);
	vso.pos = mul(vso.pos, mtxLocal);
	vso.pos.x *= -1.0f;
	vso.pos = mul(vso.pos, mtxWorld);
	vso.pos = mul(vso.pos, mtxView);
	PosWV = vso.pos.xyz;
	vso.pos = mul(vso.pos, mtxProj);

	//�e�N�X�`��
	vso.tex = vsi.tex;

	//�ϊ��s��(�ڋ�ԁ˃r���[��ԁj
	//vso.mtxTtoV._11_12_13 = vsi.tangent;
	//vso.mtxTtoV._21_22_23 = vsi.binormal;
	//vso.mtxTtoV._31_32_33 = vsi.normal;
	//vso.mtxTtoV = mul(vso.mtxTtoV, (float3x3) mtxL);
	//vso.mtxTtoV = mul(vso.mtxTtoV, (float3x3) mtxLocal);
	//vso.mtxTtoV._11 *= -1.0f;
	//vso.mtxTtoV._21 *= -1.0f;
	//vso.mtxTtoV._31 *= -1.0f;
	//vso.mtxTtoV = mul(vso.mtxTtoV, (float3x3) mtxWorld);
	//vso.mtxTtoV = mul(vso.mtxTtoV, (float3x3) mtxView);

	//BTN�s����擾
	float3x3 mtxT = {
		vsi.tangent,
		vsi.binormal,
		vsi.normal
	};

	//���ւ̒P�ʃx�N�g��(�ڋ�Ԃ֕ϊ�)
	float3 PosL = LightPos.xyz;
	PosL.x += cbLightOffset.x * 2.0f;
	PosL.y -= cbLightOffset.y * 2.0f;
	PosL.z += cbLightOffset.z * 2.0f;
	vso.vDirT_ToLight = mul(PosL, (float3x3) mtxView);
	vso.vDirT_ToLight = mul(vso.vDirT_ToLight, transpose((float3x3) mtxView));
	vso.vDirT_ToLight = mul(vso.vDirT_ToLight, transpose((float3x3) mtxWorld));
	vso.vDirT_ToLight.x *= -1.0f;
	vso.vDirT_ToLight = mul(vso.vDirT_ToLight, transpose(mtxT)); //���s�����̏ꍇ�A�^���I�Ɉʒu��P�ʃx�N�g���Őݒ�
	vso.vDirT_ToLight.x *= -1.0f;
	vso.vNorT_ToLight = normalize(vso.vDirT_ToLight);	//�����ւ̖@���x�N�g��

	//���_���璍���_�ւ̕����x�N�g��(�ڋ�Ԃ֕ϊ�)
	vso.vNorT_ToCamera = mul(PosWV, transpose((float3x3) mtxView));
	vso.vNorT_ToCamera = mul(vso.vNorT_ToCamera, transpose((float3x3) mtxWorld));
	vso.vNorT_ToCamera.x *= -1.0f;
	vso.vNorT_ToCamera = mul(vso.vNorT_ToCamera, transpose(mtxT));
	vso.vNorT_ToCamera.x *= -1.0f;
	vso.vNorT_ToCamera = normalize(-vso.vNorT_ToCamera);

	//�߂�l
	return vso;
}
