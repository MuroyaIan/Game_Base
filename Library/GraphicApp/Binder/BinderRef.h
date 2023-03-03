/**
 * @file BinderRef.h
 * @brief バインダ処理まとめ
 * @author 室谷イアン
 * @date 2022/06/25
 * @履歴 2022/06/25：ファイル作成
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <GraphicApp/Binder/VertexBuffer.h>
#include <GraphicApp/Binder/IndexBuffer.h>
#include <GraphicApp/Binder/InputLayout.h>
#include <GraphicApp/Binder/Topology.h>

#include <GraphicApp/Binder/VertexShader.h>
#include <GraphicApp/Binder/PixelShader.h>

#include <GraphicApp/Binder/Sampler.h>
#include <GraphicApp/Binder/srvMgr.h>
#include <GraphicApp/Binder/Texture.h>
#include <GraphicApp/Binder/Texture_Model.h>
#include <GraphicApp/Binder/Texture_Anim.h>

#include <GraphicApp/Binder/ConstantBuffer.h>
#include <GraphicApp/Binder/cbMgr.h>
#include <GraphicApp/Binder/cbMatrix_WVP.h>
#include <GraphicApp/Binder/cbMatrix_VP.h>
#include <GraphicApp/Binder/cbMatrix_LWVP.h>
#include <GraphicApp/Binder/cbBone.h>
#include <GraphicApp/Binder/cbMaterial.h>
