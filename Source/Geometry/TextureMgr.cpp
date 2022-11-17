
//===== �C���N���[�h�� =====
#include <Geometry/TextureMgr.h>
#include <GraphicApp/Binder/BinderRef.h>

//===== �ÓI�����o =====
std::string TEXTURE_MGR::aFilePath[static_cast<int>(TEX_ID::ID_MAX)] = {
	"test.png",
	"test2.png"
};

//===== �\���̎��� =====
TEXTURE_MGR::TEX_PACK::TEX_PACK() noexcept : TexData(), pBinder(), nUsedCount(0)
{
}

TEXTURE_MGR::TEX_PACK::~TEX_PACK() noexcept
{
}

//===== �N���X���� =====
TEXTURE_MGR::TEXTURE_MGR(GRAPHIC& Gfx) noexcept : m_aTexPack(static_cast<int>(TEX_ID::ID_MAX)), m_DX(Gfx)
{
	//�e�N�X�`���Ǎ�
	std::string FilePath = "Asset/Texture/";
	std::ostringstream oss;
	for (size_t i = 0, Cnt = m_aTexPack.size(); i < Cnt; i++) {
		oss << FilePath << aFilePath[i];
		m_aTexPack[i].TexData = TEX_LOADER::LoadTexture(oss.str().c_str());
		oss.str("");
	}
}

TEXTURE_MGR::~TEXTURE_MGR() noexcept
{
	//���������
	for (auto& d : m_aTexPack) {
		if (d.pBinder != nullptr)
			d.pBinder.reset();								//�o�b�t�@���
		TEX_LOADER::ReleaseTexture(d.TexData.pImageData);	//�e�N�X�`�����
	}
}

//�o�b�t�@���p�J�n�E�I��
void TEXTURE_MGR::SetTextureOn(TEX_ID id)
{
	//�Q�Ɛ��X�V
	m_aTexPack[static_cast<int>(id)].nUsedCount++;

	//�o�b�t�@���쐬�̏ꍇ�ˍ쐬
	if (m_aTexPack[static_cast<int>(id)].pBinder == nullptr)
		m_aTexPack[static_cast<int>(id)].pBinder = std::make_unique<TEXTURE>(m_DX, m_aTexPack[static_cast<int>(id)].TexData);
}

void TEXTURE_MGR::SetTextureOff(TEX_ID id) noexcept
{
	//�Q�Ɛ��X�V
	m_aTexPack[static_cast<int>(id)].nUsedCount--;
	if (m_aTexPack[static_cast<int>(id)].nUsedCount < 0)
		m_aTexPack[static_cast<int>(id)].nUsedCount = 0;

	//�Q�Ɛ������˃o�b�t�@���
	if (m_aTexPack[static_cast<int>(id)].nUsedCount == 0)
		m_aTexPack[static_cast<int>(id)].pBinder.reset();
}

//�o�C���h����
void TEXTURE_MGR::Bind(TEX_ID id) const
{
	//��O����
	if (m_aTexPack[static_cast<int>(id)].pBinder == nullptr)
		throw ERROR_EX2("�e�N�X�`����null�ł��B");

	m_aTexPack[static_cast<int>(id)].pBinder->Bind(m_DX);
}
