
//===== インクルード部 =====
#include <Geometry/TextureMgr.h>
#include <GraphicApp/Binder/BinderRef.h>

//===== 静的メンバ =====
std::string TEXTURE_MGR::aFilePath[static_cast<int>(TEX_ID::ID_Max)] = {
	"null.png",
	"test.png",
	"test2.png"
};

//===== 構造体実装 =====
TEXTURE_MGR::TEX_PACK::TEX_PACK() noexcept : TexData(), pTexBuff(), nUsedCount(0)
{}

TEXTURE_MGR::TEX_PACK::~TEX_PACK() noexcept
{}

//===== クラス実装 =====
TEXTURE_MGR::TEXTURE_MGR(GRAPHIC& Gfx) : m_aTexPack(static_cast<int>(TEX_ID::ID_Max)), m_DX(Gfx)
{
	//テクスチャ読込
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
	//メモリ解放
	for (auto& d : m_aTexPack) {
		if (d.pTexBuff != nullptr)
			d.pTexBuff.reset();								//バッファ解放
		TEX_LOADER::ReleaseTexture(d.TexData.pImageData);	//テクスチャ解放
	}
}

//バッファ利用開始・終了
ID3D11ShaderResourceView* TEXTURE_MGR::SetTextureOn(TEX_ID id)
{
	//参照数更新
	m_aTexPack[static_cast<int>(id)].nUsedCount++;

	//バッファ未作成の場合⇒作成
	if (m_aTexPack[static_cast<int>(id)].pTexBuff == nullptr)
		m_aTexPack[static_cast<int>(id)].pTexBuff = std::make_unique<TEXTURE>(m_DX, m_aTexPack[static_cast<int>(id)].TexData);

	return m_aTexPack[static_cast<int>(id)].pTexBuff->GetSrvPtr();
}

void TEXTURE_MGR::SetTextureOff(TEX_ID id) noexcept
{
	//参照数更新
	m_aTexPack[static_cast<int>(id)].nUsedCount--;
	if (m_aTexPack[static_cast<int>(id)].nUsedCount < 0)
		m_aTexPack[static_cast<int>(id)].nUsedCount = 0;

	//参照数無し⇒バッファ解放
	if (m_aTexPack[static_cast<int>(id)].nUsedCount == 0 &&
		m_aTexPack[static_cast<int>(id)].pTexBuff != nullptr)
		m_aTexPack[static_cast<int>(id)].pTexBuff.reset();
}

//バインド処理
void TEXTURE_MGR::Bind(TEX_ID id) const
{
	//例外処理
	if (m_aTexPack[static_cast<int>(id)].pTexBuff == nullptr)
		throw ERROR_EX2("テクスチャはnullです。");

	m_aTexPack[static_cast<int>(id)].pTexBuff->Bind(m_DX);
}
