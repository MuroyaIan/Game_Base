
//===== �C���N���[�h�� =====
#include <GraphicApp/Drawer/Drawer.h>
#include <typeinfo>								//�|�C���^���f�[�^�̌^�m�F
#include <GraphicApp/Binder/IndexBuffer.h>
#include <GraphicApp/Binder/VertexBuffer.h>

//===== �N���X���� =====
DRAWER::DRAWER(const GRAPHIC& Gfx) noexcept : m_Gfx(Gfx), m_aBinder(),
	m_pIndexBuffer(nullptr), m_pVertexBuffer(nullptr), m_InstanceNum(0)
{
}

DRAWER::~DRAWER() noexcept
{
}

//�����ݏ���
void DRAWER::Draw(int InstanceNum) const noexcept
{
	for (auto& b : m_aBinder)											//�o�C���h����
		b->Bind(m_Gfx);
	for (auto& sb : GetStaticBind())									//�o�C���h�����i�ÓI�j
		sb->Bind(m_Gfx);

	if (m_pIndexBuffer != nullptr) {
		if (InstanceNum < 0)
			m_Gfx.DrawIndexed(m_pIndexBuffer->GetIndexNum());			//�t���[���o�b�t�@������
		else
			m_Gfx.DrawInstanced(m_pIndexBuffer->GetIndexNum(), InstanceNum);
	}
	else {
		if (InstanceNum < 0)
			m_Gfx.DrawIndexed(GetStaticIndexBuffer().GetIndexNum());	//�C���f�b�N�X�o�b�t�@�Œ�̏ꍇ
		else
			m_Gfx.DrawInstanced(GetStaticIndexBuffer().GetIndexNum(), InstanceNum);
	}
}

//�o�C���_�o�^
void DRAWER::AddBind(std::unique_ptr<BINDER> pBinder)
{
	//��O����
	if (pBinder == nullptr)
		throw ERROR_EX2("�y�x���z�|�C���^��NULL�ł��I");

	//�����C���f�b�N�X�̃o�C���h��h�~
	if (typeid(*pBinder) == typeid(INDEX_BUFFER)) {
		if (m_pIndexBuffer != nullptr)
			throw ERROR_EX2("�y�x���zm_pIndexBuffer�͏㏑������܂��I");
		m_pIndexBuffer = dynamic_cast<INDEX_BUFFER*>(pBinder.get());
	}

	//���_�o�b�t�@�̃|�C���^���擾
	if (typeid(*pBinder) == typeid(VERTEX_BUFFER)) {
		if (m_pVertexBuffer != nullptr)
			throw ERROR_EX2("�y�x���zm_pVertexBuffer�͏㏑������܂��I");
		m_pVertexBuffer = dynamic_cast<VERTEX_BUFFER*>(pBinder.get());
	}

	//���L���ڍs
	m_aBinder.push_back(std::move(pBinder));
}

//�C���f�b�N�X���擾
UINT DRAWER::GetIndexNum() const noexcept
{
	if (m_pIndexBuffer != nullptr)
		return m_pIndexBuffer->GetIndexNum();
	else
		return GetStaticIndexBuffer().GetIndexNum();
}
