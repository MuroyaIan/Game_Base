
//===== �C���N���[�h�� =====
#include <Geometry/Model/TestWave.h>
#include <GraphicApp/Binder/BinderRef.h>

#include <Tool/Rand.h>
#include <Tool/gMath.h>

namespace dx = DirectX;

//===== �N���X���� =====
WAVE::WAVE(APP& App) :
	DRAWER(App.GetGfxPack().m_DX), m_Gfx(App.GetGfxPack()), m_InstanceNum(22500), m_aInstanceData(m_InstanceNum), m_Material(),
	m_Input(App.GetInputMgr()), m_aInitPos(m_InstanceNum), m_WaveLength(0.5f), m_WaveCoef(1.0f)
{
	//���_���쐬
	VS_DATA<VERTEX_M> Model = VSD_SPHERE::MakeData_Model<VERTEX_M>(8, 8);
	dx::XMFLOAT4X4 mtx{};
	dx::XMStoreFloat4x4(&mtx, dx::XMMatrixScaling(1.0f, 1.0f, 1.0f));
	Model.SetVertexPos(mtx);
	for (auto& v : Model.m_Vertices) {	//���W�n�ϊ�
		v.m_Pos.x *= -1.0f;
		v.m_Normal.x *= -1.0f;
	}
	AddBind(std::make_unique<VERTEX_BUFFER>(m_Gfx.m_DX, Model.m_Vertices, m_aInstanceData));

	//�C���f�b�N�X���쐬
	AddBind(std::make_unique<INDEX_BUFFER>(m_Gfx.m_DX, Model.m_Indices));

	//�萔�o�b�t�@�쐬�i�}�e���A���j
	AddBind(std::make_unique<CB_MATERIAL>(m_Gfx.m_DX, nullptr, m_Material));

	//�e�N�X�`���o�b�t�@�쐬
	std::vector<TEX_LOADER::TEX_DATA> aData(static_cast<int>(TEXTURE_MODEL::TEX_TYPE::MaxType));
	//aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Diffuse)] = TEX_LOADER::LoadTexture("Asset/Texture/test2.png");
	aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Diffuse)] = TEX_LOADER::LoadTexture("Asset/Texture/null.png");
	aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Specular)] = TEX_LOADER::LoadTexture("Asset/Texture/null.png");
	aData[static_cast<int>(TEXTURE_MODEL::TEX_TYPE::Normal)] = TEX_LOADER::LoadTexture("Asset/Texture/null.png");
	AddBind(std::make_unique<TEXTURE_MODEL>(m_Gfx.m_DX, aData));
	for (auto& d : aData)
		TEX_LOADER::ReleaseTexture(d.pImageData);

	//�C���X�^���X��������
	SetInstanceNum(m_InstanceNum);

	//�ʒu��񏉊���
	int EdgeSize = static_cast<int>(std::sqrt(m_InstanceNum));	//�g���w�̕ӂ̒���
	float EdgeCoef = 2.0f;										//�ӂ̒����p�W��
	float EdgeHalfSize = EdgeSize * EdgeCoef * 0.5f;			//�ӂ̒����̔���
	for (int i = 0; i < m_InstanceNum; i++) {
		dx::XMMATRIX mtxT = dx::XMMatrixTranslation(0.0f + EdgeCoef * (i % EdgeSize), 0.0f, 0.0f + EdgeCoef * static_cast<float>(i / EdgeSize))		//�����ʒu�iXY���ʁj
			* dx::XMMatrixTranslation(-EdgeHalfSize, 0.0f, -EdgeHalfSize * 0.35f);																			//�g�̒��S���W���C��
		dx::XMVECTOR vPos = dx::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		dx::XMStoreFloat3(&m_aInitPos[i], dx::XMVector3Transform(vPos, mtxT));
	}

	//�}�e���A����񏉊���
	m_Material.ms_Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_Material.ms_Specular = { 0.6f, 0.6f, 0.6f, 1.0f };
	m_Material.ms_Shininess = 30.0f;
}

WAVE::~WAVE() noexcept
{
}

//�X�V����
void WAVE::Update() noexcept
{
	//�^�C�}����(���i�g)
	static float swTimer = 0.0f;
	static const float swTimeCoef = 0.01f;						//�^�C�}�p�W��
	swTimer += swTimeCoef;

	//�^�C�}����(��g)
	static float rwTimer = -1.0f;
	static const float rwTimeCoef = 0.1f;						//�^�C�}�p�W��
	static const float rwEndTime = rwTimeCoef * 60.0f * 5.0f;	//�^�C�}�I������
	if (m_Input.m_KB.GetTrigger(VK_SPACE) &&
		rwTimer == -1.0f)
		rwTimer = 0.0f;											//�g�J�n
	if (rwTimer >= 0.0f) {
		rwTimer += rwTimeCoef;

		//�g�I��
		if (rwTimer >= rwEndTime)
			rwTimer = -1.0f;
	}

	//���[���h�s��X�V
	for (int i = 0; i < m_InstanceNum; i++) {

		//���i�g���v�Z
		m_WaveLength = 50.0f;
		m_WaveCoef = 0.5f;
		float swHeight = ClacStraightWave(VECTOR2{ 1.0f, 1.0f }, VECTOR2{ m_aInitPos[i].x, m_aInitPos[i].z }, swTimer);

		//��g���v�Z
		m_WaveLength = 40.0f;
		m_WaveCoef = 4.0f;
		float rwHeight = ClacRingWave(VECTOR2{ 0.0f, 0.0f }, VECTOR2{ m_aInitPos[i].x, m_aInitPos[i].z }, rwTimer);

		//�s��X�V
		dx::XMMATRIX mtx = dx::XMMatrixTranslation(m_aInitPos[i].x, swHeight + rwHeight, m_aInitPos[i].z);	//�g�p�ϊ��s��
		dx::XMStoreFloat4x4(&m_aInstanceData[i].MtxWorld, mtx);
	}
}

//�`�揈��
void WAVE::Draw(int InstanceNum)
{
	//��O����
	(void)InstanceNum;
	if (m_InstanceNum < 1)
		return;

	//�C���X�^���X�X�V
	std::vector<VSD_INSTANCE> aInstData = m_aInstanceData;
	for (auto& i : aInstData)
		gMath::MtxTranspose4x4_SSE(&i.MtxWorld._11);
	GetVertexBuffer().UpdateBuffer(m_Gfx.m_DX, aInstData, VERTEX_BUFFER::VB_TYPE::Instance);

	//�C���X�^���X�`��
	m_Gfx.m_ShaderMgr.Bind_Instance_Phong();
	DRAWER::Draw(m_InstanceNum);
}

//�C���X�^���X�ǉ�
int WAVE::AddInstance()
{
	//�z��ǉ�
	m_InstanceNum++;
	VSD_INSTANCE InstData{};
	m_aInstanceData.push_back(InstData);

	//�C���X�^���X�o�b�t�@�Đݒ�
	GetVertexBuffer().ResetInstanceBuffer(m_Gfx.m_DX, m_aInstanceData);
	GetVertexBuffer().UpdateBuffer(m_Gfx.m_DX, m_aInstanceData, VERTEX_BUFFER::VB_TYPE::Instance);

	//�C���X�^���X���X�V
	SetInstanceNum(m_InstanceNum);

	return m_InstanceNum - 1;
}

//�����g�ɂ�鍂���v�Z
float WAVE::ClacSineWave(float Dist, float Time, bool bLoop)
{
	float Height = 0.0f;												//�g�̍���
	float Period = f2PI / m_WaveLength * Dist;							//����
	float Phase = std::sqrtf(f2PI * fGravity / m_WaveLength) * Time;	//�ʑ�

	//�����g
	if (bLoop) {
		Height = m_WaveCoef * m_WaveLength / 14.0f * std::sinf(Period - Phase);
		return Height;
	}

	//�g1��
	if (Phase < Period ||
		Phase >= f2PI + Period)
		Height = 0.0f;
	else
		Height = m_WaveCoef * m_WaveLength / 14.0f * std::sinf(Period - Phase);

	return Height;
}

//��g�ɂ�鍂���v�Z
float WAVE::ClacRingWave(VECTOR2 OriPos, VECTOR2 Pos, float Time)
{
	float Dist = gMath::VecLength(Pos - OriPos);
	return ClacSineWave(Dist, Time, false);
}

//���i�g�ɂ�鍂���v�Z
float WAVE::ClacStraightWave(VECTOR2 vDirection, VECTOR2 vPos, float Time)
{
	float Dist = gMath::VecDot(gMath::VecNormalize(vDirection), vPos);
	return ClacSineWave(Dist, Time);
}
