
//===== �C���N���[�h�� =====
#include <Geometry/Model/Model.h>
#include <Geometry/Model/Mesh.h>
#include <GraphicApp/Binder/BinderRef.h>

namespace dx = DirectX;

//===== �N���X���� =====
MODEL::MODEL(APP& App, MODEL_MGR::MODEL_ID id) noexcept :
	m_Gfx(App.GetGfxPack()), m_ID(id), m_FileData(m_Gfx.m_ModelMgr.GetModelPack(id)), m_aMesh(m_FileData.aMesh.size()),
	m_InstanceNum(0), m_aInstanceData(m_InstanceNum), m_aMtxWorld(m_InstanceNum), m_bStatic(true), m_aAnimData(0)
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
void MODEL::Update()
{
	//��O����
	if (m_InstanceNum < 1)
		return;

	//�A�j���[�V�����X�V
	if (!m_bStatic) {
		for (auto& a : m_aAnimData) {
			if (!a.bBlendAnim)
				UpdateAnimation(a);				//�u�����h�Ȃ�
			else
				UpdateAnimationBlending(a);		//�u�����h����i1�b�ԁj
		}
	}

	//�C���X�^���X�X�V
	auto pInst = &m_aInstanceData[0];
	auto pMtxW = &m_aMtxWorld[0];
	for (size_t i = 0, Cnt = m_aInstanceData.size(); i < Cnt; i++) {
		pInst->MtxWorld = m_FileData.InitMtxWorld;						//�����s��
		gMath::MtxMultiply4x4_AVX(&pInst->MtxWorld._11, &pMtxW->_11);	//���[���h�s��
		gMath::MtxTranspose4x4_SSE(&pInst->MtxWorld._11);				//�]�u����

		//�A�j���[�V�����X�V
		if (!m_bStatic) {
			pInst->AnimFrame = 0;
			pInst->AnimBlendFrame = 0;							//�t���[�����Z�b�g
			auto pAnim = &m_aAnimData[i];

			//�t���[���X�V
			auto pFrame = &m_FileData.aAnimFrame[0];
			for (int j = 0, jCnt = pAnim->ID; j < jCnt; j++) {	//ID�Ɏ���܂ł̃t���[���������Z
				pInst->AnimFrame += *pFrame;
				pFrame++;
			}
			pInst->AnimFrame += pAnim->CurrentFrame;			//���݂̃t���[���������Z

			//�t���[���X�V�i�u�����h�p�j
			if (pAnim->bBlendAnim) {
				pFrame = &m_FileData.aAnimFrame[0];
				for (int j = 0, jCnt = pAnim->BlendID; j < jCnt; j++) {
					pInst->AnimBlendFrame += *pFrame;
					pFrame++;
				}
				pInst->AnimBlendFrame += pAnim->BlendCurrentFrame;
				pInst->AnimLerp = pAnim->AnimLerp;
			}
			else
				pInst->AnimLerp = 0.0f;
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
void MODEL::Draw() noexcept
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
		AnimData.ID = rand() % 3;
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
void MODEL::UpdateAnimation(ANIM_PACK& AnimData) const noexcept
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
	//�t���[���X�V�i�u�����h��j
	if (!m_FileData.aIsFPS_30[AnimData.BlendID]) {

		//60FPS
		AnimData.BlendCurrentFrame++;

		//�t���[���J�E���g���Z�b�g
		if (AnimData.BlendFrameCnt > 0)
			AnimData.BlendFrameCnt = 0;
	}
	else {

		//30FPS
		AnimData.BlendFrameCnt++;
		if (AnimData.BlendFrameCnt > 1) {
			AnimData.BlendFrameCnt = 0;

			//60FPS�Ɠ�������
			AnimData.BlendCurrentFrame++;
		}
	}

	//�t���[������
	if (AnimData.BlendCurrentFrame > m_FileData.aAnimFrame[AnimData.BlendID] - 1)
		AnimData.BlendCurrentFrame = 0;

	//�t���[���X�V�i�u�����h�O�j
	if (!AnimData.bBlendSync)
		UpdateAnimation(AnimData);
	else {

		//�t���[���̓�������
		float FrameRatio = AnimData.BlendCurrentFrame / (m_FileData.aAnimFrame[AnimData.BlendID] - 1.0f);	//�t���[�������p�̊����擾
		AnimData.CurrentFrame = static_cast<int>((m_FileData.aAnimFrame[AnimData.ID] - 1) * FrameRatio);	//�u�����h�O�̃t���[���Z�o
	}

	//���`��Ԍv�Z
	AnimData.AnimLerp = static_cast<float>(AnimData.BlendTimer) / AnimData.BlendTime;

	//�u�����h�I��
	AnimData.BlendTimer++;
	if (AnimData.BlendTimer > AnimData.BlendTime) {

		//�A�j���[�V�������㏑��
		AnimData.ID = AnimData.BlendID;
		AnimData.CurrentFrame = AnimData.BlendCurrentFrame;
		AnimData.FrameCnt = AnimData.BlendFrameCnt;

		//�u�����h�I��
		AnimData.bBlendAnim = false;
		AnimData.BlendID = 0;
		AnimData.BlendCurrentFrame = 0;
		AnimData.AnimLerp = 0.0f;
	}
}
