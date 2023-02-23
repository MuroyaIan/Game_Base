
//===== �C���N���[�h�� =====
#include <Geometry/Model/Model.h>
#include <Geometry/Model/Mesh.h>
#include <GraphicApp/Binder/BinderRef.h>

namespace dx = DirectX;

//===== �N���X���� =====
MODEL::MODEL(APP& App, MODEL_MGR::MODEL_ID id) noexcept :
	m_Gfx(App.GetGfxPack()), m_ID(id), m_FileData(m_Gfx.m_ModelMgr.GetModelPack(id)), m_aMesh(m_FileData.aMesh.size()),
	m_InstanceNum(0), m_aInstanceData(m_InstanceNum), m_aMtxWorld(m_InstanceNum),
	m_bStatic(true), m_pBoneBuffer(), m_BoneData(), m_AnimID(1), m_AnimID_Backup(m_AnimID), m_AnimFrame(0), m_AnimFrame_Backup(m_AnimFrame), m_FrameCnt(0), m_FrameCnt_Backup(m_FrameCnt),
	m_bBlendAnim(false), m_BlendTimer(0)
{
	//�A�j���[�V�����m�F
	if (m_FileData.aAnimFrame.size() > 0)
		m_bStatic = false;

	//VS�萔�o�b�t�@�쐬�i�����j
	if (!m_bStatic)
		m_pBoneBuffer = std::make_unique<CB_BONE>(m_Gfx.m_DX, nullptr, m_BoneData);

	//���b�V��������
	for (size_t i = 0, Cnt = m_FileData.aMesh.size(); i < Cnt; i++)
		m_aMesh[i] = std::make_unique<MESH>(*this, static_cast<int>(i));
}

MODEL::~MODEL() noexcept
{
}

//�X�V����
void MODEL::Update() noexcept
{
	//��O����
	if (m_InstanceNum < 1)
		return;

	//�A�j���[�V�����X�V
	if (!m_bStatic) {

		if (!m_bBlendAnim)
			UpdateAnimation();
		else
			UpdateAnimationBlending();	//�u�����h�����i0.2s�b�ԁj

		//�������X�V
		m_pBoneBuffer->Bind(m_Gfx.m_DX);
	}

	//���[���h�s��X�V
	for (size_t i = 0, Cnt = m_aInstanceData.size(); i < Cnt; i++) {
		m_aInstanceData[i].MtxWorld = m_FileData.InitMtxWorld;
		gMath::MtxMultiply4x4_AVX(&m_aInstanceData[i].MtxWorld._11, &m_aMtxWorld[i]._11);
	}

	//���b�V���X�V
	for (auto& m : m_aMesh)
		m->Update();
}

//�����ݏ���
void MODEL::Draw() const noexcept
{
	//��O����
	if (m_InstanceNum < 1)
		return;

	//���b�V���`��
	for (auto& m : m_aMesh)
		m->Draw();
}

//�C���X�^���X�ǉ�
int MODEL::AddInstance()
{
	m_InstanceNum++;

	//�z��ǉ�
	m_aInstanceData.push_back(VSD_INSTANCE{});
	dx::XMFLOAT4X4 mtxW{};
	dx::XMStoreFloat4x4(&mtxW, dx::XMMatrixIdentity());
	m_aMtxWorld.push_back(std::move(mtxW));

	//���b�V���̃C���X�^���X��ǉ�
	for (auto& m : m_aMesh)
		m->AddInstance();

	//�C���X�^���X�̃C���f�b�N�X��Ԃ�
	return m_InstanceNum - 1;
}

//�|���S�����擾
UINT MODEL::GetPolygonNum() const noexcept
{
	UINT PolygonNum = 0;
	for (auto& m : m_aMesh)
		PolygonNum += m->GetPolygonNum();
	return PolygonNum;
}

//�A�j���[�V�����X�V
void MODEL::UpdateAnimation() noexcept
{
	//�t���[���X�V
	if (!m_FileData.aIsFPS_30[m_AnimID]) {

		//60FPS
		m_AnimFrame++;

		//�t���[���J�E���g���Z�b�g
		if (m_FrameCnt > 0)
			m_FrameCnt = 0;
	}
	else {

		//30FPS
		m_FrameCnt++;
		if (m_FrameCnt > 1) {
			m_FrameCnt = 0;

			//60FPS�Ɠ�������
			m_AnimFrame++;
		}
	}

	//�t���[������
	if (m_AnimFrame > m_FileData.aAnimFrame[m_AnimID] - 1)
		m_AnimFrame = 0;

	//�����X�V
	auto pMtxBone = &m_BoneData.mtxBone[0];
	auto pMtxBoneRef = &m_FileData.aBone[0];
	for (size_t i = 0, Cnt = m_FileData.aBone.size(); i < Cnt; i++) {
		if (pMtxBoneRef->aSkin[m_AnimID].aMatrix.size() > 0)
			*pMtxBone = pMtxBoneRef->aSkin[m_AnimID].aMatrix[m_AnimFrame];
		pMtxBone++;
		pMtxBoneRef++;
	}
}

//�A�j���[�V�����u�����h�X�V
void MODEL::UpdateAnimationBlending() noexcept
{
	m_BlendTimer++;

	//�t���[���X�V
	if (!m_FileData.aIsFPS_30[m_AnimID]) {

		//60FPS
		m_AnimFrame++;

		//�t���[���J�E���g���Z�b�g
		if (m_FrameCnt > 0)
			m_FrameCnt = 0;
	}
	else {

		//30FPS
		m_FrameCnt++;
		if (m_FrameCnt > 1) {
			m_FrameCnt = 0;

			//60FPS�Ɠ�������
			m_AnimFrame++;
		}
	}

	//�t���[������
	if (m_AnimFrame > m_FileData.aAnimFrame[m_AnimID] - 1)
		m_AnimFrame = 0;

	//�u�����h�O�A�j���[�V�����̍X�V
	float ratio = static_cast<float>(m_AnimFrame + 1) / m_FileData.aAnimFrame[m_AnimID];		//�u�����h��A�j���[�V�����̃t���[�������擾
	m_AnimFrame_Backup = static_cast<int>(m_FileData.aAnimFrame[m_AnimID_Backup] * ratio - 1);	//�u�����h�O�A�j���[�V�����̃t���[���Z�o

	//�����X�V
	auto pMtxBone = &m_BoneData.mtxBone[0];
	auto pMtxBoneRef = &m_FileData.aBone[0];
	for (size_t i = 0, Cnt = m_FileData.aBone.size(); i < Cnt; i++) {
		if (pMtxBoneRef->aSkin[m_AnimID].aMatrix.size() > 0) {
			//*pMtxBone = pMtxBoneRef->aSkin[m_AnimID].aMatrix[m_AnimFrame];
			dx::XMMATRIX mtx1 = dx::XMLoadFloat4x4(&pMtxBoneRef->aSkin[m_AnimID].aMatrix[m_AnimFrame]);
			dx::XMMATRIX mtx2 = dx::XMLoadFloat4x4(&pMtxBoneRef->aSkin[m_AnimID_Backup].aMatrix[m_AnimFrame_Backup]);
			float ratioTime = (m_BlendTimer + 1.0f) / 60.0f;
			dx::XMStoreFloat4x4(pMtxBone, mtx1 * ratioTime + mtx2 * (1.0f - ratioTime));
		}
		pMtxBone++;
		pMtxBoneRef++;
	}

	//�u�����h�I������
	if (m_BlendTimer > 59) {
		m_bBlendAnim = false;
		m_BlendTimer = 0;
	}
}
