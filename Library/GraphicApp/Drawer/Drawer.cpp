
//===== �C���N���[�h�� =====
#include <GraphicApp/Drawer/Drawer.h>
#include <typeinfo>								//�|�C���^���f�[�^�̌^�m�F
#include <GraphicApp/Binder/IndexBuffer.h>
#include <GraphicApp/Binder/VertexBuffer.h>

//===== �N���X���� =====
DRAWER::DRAWER(const CT_GRAPHIC& Gfx) noexcept : m_Gfx(Gfx), m_aBinder(),
	m_pIndexBuffer(nullptr), m_pVertexBuffer(nullptr), m_MaxInstanceNum(0)
{
}

DRAWER::~DRAWER() noexcept
{
}

//�`�揈��
void DRAWER::Draw(int InstanceNum)
{
	DrawProc(InstanceNum);
}

//�o�C���_�o�^
void DRAWER::AddBind(std::unique_ptr<CT_BINDER> pBinder)
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
	return m_pIndexBuffer->GetIndexNum();
}

//�`�揈���i�ŏI�����j
void DRAWER::DrawProc(int InstanceNum) const noexcept
{
	//�o�C���h����
	for (auto& b : m_aBinder)
		b->Bind(m_Gfx);

	//�`�揈��
	if (InstanceNum < 1)
		m_Gfx.DrawIndexed(m_pIndexBuffer->GetIndexNum());
	else
		m_Gfx.DrawInstanced(m_pIndexBuffer->GetIndexNum(), static_cast<UINT>(InstanceNum));
}
