
//===== �C���N���[�h�� =====
#include <Geometry/Model/Model.h>
#include <Geometry/Model/Mesh.h>
#include <GraphicApp/Binder/BinderRef.h>

namespace dx = DirectX;

//===== �N���X���� =====
MODEL::MODEL(APP& App, MODEL_MGR::MODEL_ID id) noexcept :
	m_Gfx(App.GetGfxPack()), m_ID(id), m_FileData(m_Gfx.m_ModelMgr.GetModelPack(id)), m_aMesh(m_FileData.aMesh.size()),
	m_InstanceNum(0), m_aInstanceData(m_InstanceNum), m_aMtxWorld(m_InstanceNum),
	m_bStatic(true), m_aAnimData(0), m_AnimID_Backup(0), m_AnimFrame_Backup(0), m_FrameCnt_Backup(0),
	m_bBlendAnim(false), m_BlendTimer(0)
{
	//�A�j���[�V�����m�F
	if (m_FileData.aAnimFrame.size() > 0)
		m_bStatic = false;

	//�A�j���[�V�����o�b�t�@�쐬
	if (!m_bStatic)
		m_Gfx.m_ModelMgr.SetAnimTexOn(m_ID);

	//���b�V��������
	for (size_t i = 0, Cnt = m_FileData.aMesh.size(); i < Cnt; i++)
		m_aMesh[i] = std::make_unique<MESH>(*this, static_cast<int>(i));
}

MODEL::~MODEL() noexcept
{
	//�A�j���[�V�����o�b�t�@���
	if (!m_bStatic)
		m_Gfx.m_ModelMgr.SetAnimTexOff(m_ID);
}

//�X�V����
void MODEL::Update() noexcept
{
	//��O����
	if (m_InstanceNum < 1)
		return;

	//�A�j���[�V�����X�V
	if (!m_bStatic) {

		if (!m_bBlendAnim) {
			for (auto& a : m_aAnimData)
				UpdateAnimation(a);
		}
		else {
			for (auto& a : m_aAnimData)
				UpdateAnimationBlending(a);		//�u�����h�����i0.2s�b�ԁj
		}
	}

	//�C���X�^���X���X�V
	auto pInst = &m_aInstanceData[0];
	auto pMtxW = &m_aMtxWorld[0];
	for (size_t i = 0, Cnt = m_aInstanceData.size(); i < Cnt; i++) {
		pInst->MtxWorld = m_FileData.InitMtxWorld;						//�����s��
		gMath::MtxMultiply4x4_AVX(&pInst->MtxWorld._11, &pMtxW->_11);	//���[���h�s��
		gMath::MtxTranspose4x4_SSE(&pInst->MtxWorld._11);				//�]�u����

		//�A�j���[�V�����t���[���X�V
		if (!m_bStatic) {
			pInst->m_AnimFrame = 0;								//�t���[�����Z�b�g
			auto pAnim = &m_aAnimData[i];
			auto pFrame = &m_FileData.aAnimFrame[0];
			for (int j = 0, jCnt = pAnim->ID; j < jCnt; j++) {	//ID�Ɏ���܂ł̃t���[���������Z
				pInst->m_AnimFrame += *pFrame;
				pFrame++;
			}
			pInst->m_AnimFrame += pAnim->CurrentFrame;			//���݂̃t���[���������Z
		}

		//�|�C���^���Z
		pInst++;
		pMtxW++;
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
	if (!m_bStatic) {
		ANIM_PACK AnimData;
		AnimData.ID = rand() % 2;
		AnimData.CurrentFrame = rand() % 60;
		m_aAnimData.push_back(std::move(AnimData));
	}

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
void MODEL::UpdateAnimation(ANIM_PACK& AnimData) noexcept
{
	//�t���[���X�V
	if (!m_FileData.aIsFPS_30[AnimData.ID]) {

		//60FPS
		AnimData.CurrentFrame++;

		//�t���[���J�E���g���Z�b�g
		if (AnimData.FrameCnt > 0)
			AnimData.FrameCnt = 0;
	}
	else {

		//30FPS
		AnimData.FrameCnt++;
		if (AnimData.FrameCnt > 1) {
			AnimData.FrameCnt = 0;

			//60FPS�Ɠ�������
			AnimData.CurrentFrame++;
		}
	}

	//�t���[������
	if (AnimData.CurrentFrame > m_FileData.aAnimFrame[AnimData.ID] - 1)
		AnimData.CurrentFrame = 0;
}

//�A�j���[�V�����u�����h�X�V
void MODEL::UpdateAnimationBlending(ANIM_PACK& AnimData) noexcept
{
	m_BlendTimer++;

	//�t���[���X�V
	if (!m_FileData.aIsFPS_30[AnimData.ID]) {

		//60FPS
		AnimData.CurrentFrame++;

		//�t���[���J�E���g���Z�b�g
		if (AnimData.FrameCnt > 0)
			AnimData.FrameCnt = 0;
	}
	else {

		//30FPS
		AnimData.FrameCnt++;
		if (AnimData.FrameCnt > 1) {
			AnimData.FrameCnt = 0;

			//60FPS�Ɠ�������
			AnimData.CurrentFrame++;
		}
	}

	//�t���[������
	if (AnimData.CurrentFrame > m_FileData.aAnimFrame[AnimData.ID] - 1)
		AnimData.CurrentFrame = 0;

	//�u�����h�O�A�j���[�V�����̍X�V
	float ratio = static_cast<float>(AnimData.CurrentFrame + 1) / m_FileData.aAnimFrame[AnimData.ID];		//�u�����h��A�j���[�V�����̃t���[�������擾
	m_AnimFrame_Backup = static_cast<int>(m_FileData.aAnimFrame[m_AnimID_Backup] * ratio - 1);	//�u�����h�O�A�j���[�V�����̃t���[���Z�o

	//�����X�V
	//auto pMtxBone = &m_BoneData.mtxBone[0];
	//auto pMtxBoneRef = &m_FileData.aBone[0];
	//for (size_t i = 0, Cnt = m_FileData.aBone.size(); i < Cnt; i++) {
	//	if (pMtxBoneRef->aSkin[AnimData.ID].aMatrix.size() > 0) {
	//		//*pMtxBone = pMtxBoneRef->aSkin[AnimData.ID].aMatrix[AnimData.CurrentFrame];
	//		dx::XMMATRIX mtx1 = dx::XMLoadFloat4x4(&pMtxBoneRef->aSkin[AnimData.ID].aMatrix[AnimData.CurrentFrame]);
	//		dx::XMMATRIX mtx2 = dx::XMLoadFloat4x4(&pMtxBoneRef->aSkin[m_AnimID_Backup].aMatrix[m_AnimFrame_Backup]);
	//		float ratioTime = (m_BlendTimer + 1.0f) / 60.0f;
	//		dx::XMStoreFloat4x4(pMtxBone, mtx1 * ratioTime + mtx2 * (1.0f - ratioTime));
	//	}
	//	pMtxBone++;
	//	pMtxBoneRef++;
	//}

	//�u�����h�I������
	if (m_BlendTimer > 59) {
		m_bBlendAnim = false;
		m_BlendTimer = 0;
	}
}
