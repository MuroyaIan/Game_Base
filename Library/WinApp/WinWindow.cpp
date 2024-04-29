//===== インクルード部 =====
#include <WinApp/WinWindow.h>
#include <resource.h>
#include <Startup.h>

#ifdef IMGUI
#
#	include <3rd_Party/ImGui/backends/imgui_impl_win32.h>
#
#endif // IMGUI

//===== 追加ライブラリ =====
#pragma comment(lib, "imm32")

//===== クラス実装 =====
CT_IW_WIN::CT_IW_WIN(const LPCWSTR& windowName, const int& nWndWidth, const int& nWndHeight, const int& nWndPosX, const int& nWndPosY)
	: CT_IF_WINDOW()
	, m_AppInst(GetModuleHandle(nullptr))
	, m_PosX(nWndPosX)
	, m_PosY(nWndPosY)
	, m_Width(nWndWidth)
	, m_Height(nWndHeight)
	, m_WinHandle(nullptr)
	, m_bDrawCursor(true)
	, m_RawBuffer(0)
	, m_bUseImgui(false)
{
	//ウィンドウクラス登録
	WNDCLASSEX l_WindowClass{};
	l_WindowClass.cbSize = static_cast<UINT>(sizeof(WNDCLASSEX));
	l_WindowClass.style = CS_OWNDC;
	l_WindowClass.lpfnWndProc = WndProc_Init;
	l_WindowClass.cbClsExtra = 0;
	l_WindowClass.cbWndExtra = 0;
	l_WindowClass.hInstance = m_AppInst;
	l_WindowClass.hIcon = LoadIcon(m_AppInst, MAKEINTRESOURCE(IDI_ICON_BASE));       //アイコン
	l_WindowClass.hCursor = LoadCursor(m_AppInst, MAKEINTRESOURCE(IDC_CURSOR_BASE)); //カーソル
	l_WindowClass.hbrBackground = nullptr;
	l_WindowClass.lpszMenuName = nullptr;
	l_WindowClass.lpszClassName = c_Class_Name;                                  //クラス名
	l_WindowClass.hIconSm = LoadIcon(m_AppInst, MAKEINTRESOURCE(IDI_ICON_BASE)); //小アイコン
	if (!RegisterClassEx(&l_WindowClass))
		throw ERROR_DEFAULT();

	//サイズ算出
	DWORD l_ExStyle = 0;
	DWORD l_Style = WS_CAPTION | WS_SYSMENU;
	RECT l_WinRect = {0L, 0L, static_cast<LONG>(m_Width), static_cast<LONG>(m_Height)}; //Client領域サイズ設定
	if (!AdjustWindowRectEx(&l_WinRect, l_Style, false, l_ExStyle))                                //Windowサイズ算出
		throw ERROR_DEFAULT();
	int l_Width = l_WinRect.right - l_WinRect.left;
	int l_Height = l_WinRect.bottom - l_WinRect.top; //幅・高さを設定

	//Instance作成
	m_WinHandle = CreateWindowEx(l_ExStyle, c_Class_Name, windowName, l_Style, m_PosX, m_PosY, l_Width, l_Height, nullptr, nullptr, m_AppInst, this);
	if (!m_WinHandle)
		throw ERROR_DEFAULT();

	//Window表示
	ShowWindow(m_WinHandle, SW_SHOW);
	UpdateWindow(m_WinHandle);

	//IME無効化
	ImmAssociateContext(m_WinHandle, nullptr);

	//RawInputデバイス登録
	RAWINPUTDEVICE l_rid{};
	l_rid.usUsagePage = 0x01; //マウス識別用
	l_rid.usUsage = 0x02;     //マウス識別用
	l_rid.dwFlags = 0;
	l_rid.hwndTarget = nullptr;
	if (!RegisterRawInputDevices(&l_rid, 1, sizeof(l_rid)))
		throw ERROR_EX2("RawInput : Fail to init");

#ifdef IMGUI

	//IMGUI初期化
	if (!ImGui_ImplWin32_Init(m_WinHandle))
		throw ERROR_EX2("IMGUI : Fail to init");

#endif // IMGUI
}

CT_IW_WIN::~CT_IW_WIN() noexcept(false)
{
#ifdef IMGUI

	//IMGUI終了
	ImGui_ImplWin32_Shutdown();

#endif // IMGUI

	//Instance破棄
	if (!DestroyWindow(m_WinHandle))
		throw ERROR_DEFAULT();

	//ウィンドウクラス登録解除
	if (!UnregisterClass(c_Class_Name, m_AppInst))
		throw ERROR_DEFAULT();
}

/**
 * トランスフォーム
 *
 * \param nWndPosX
 * \param nWndPosY
 * \param nWndWidth
 * \param nWndHeight
 * \return void
 */
void CT_IW_WIN::Transform(const int& nWndPosX, const int& nWndPosY, const int& nWndWidth, const int& nWndHeight)
{
	//サイズ取得
	RECT l_ClientRect{}; //クライアント領域
	if (!GetClientRect(m_WinHandle, &l_ClientRect))
		throw ERROR_DEFAULT();
	l_ClientRect.right -= l_ClientRect.left;
	l_ClientRect.bottom -= l_ClientRect.top;
	RECT l_WindowRect{}; //非クライアント領域
	if (!GetWindowRect(m_WinHandle, &l_WindowRect))
		throw ERROR_DEFAULT();
	l_WindowRect.right -= l_WindowRect.left;
	l_WindowRect.bottom -= l_WindowRect.top;

	//サイズ更新
	int l_Width, l_Height;
	if (nWndWidth <= 0)
	{
		l_Width = l_WindowRect.right;
		m_Width = l_ClientRect.right;
	}
	else
	{
		l_Width = l_WindowRect.right - l_ClientRect.right + nWndWidth;
		m_Width = nWndWidth;
	}
	if (nWndHeight <= 0)
	{
		l_Height = l_WindowRect.bottom;
		m_Height = l_ClientRect.bottom;
	}
	else
	{
		l_Height = l_WindowRect.bottom - l_ClientRect.bottom + nWndHeight;
		m_Height = nWndHeight;
	}
	m_PosX = nWndPosX;
	m_PosY = nWndPosY;

	//Window移動
	if (!SetWindowPos(m_WinHandle, HWND_TOP, nWndPosX, nWndPosY, l_Width, l_Height, SWP_SHOWWINDOW))
		throw ERROR_DEFAULT();
}

/**
 * タイトル出力
 *
 * \param text
 * \return void
 */
void CT_IW_WIN::TitlePrint(const std::string& text) const
{
	const std::string& l_TitleName{text};
	if (!SetWindowTextA(m_WinHandle, l_TitleName.c_str()))
		throw ERROR_DEFAULT();
}

/**
 * タイトル出力（マウス座標）
 *
 * \param
 * \return void
 */
void CT_IW_WIN::TitlePrint_MousePos() const
{
	//メッセージボックス表示バグあり
	auto [x, y] = m_Mouse.GetPos();
	std::ostringstream l_oss{};
	l_oss << "MousePos:(" << x << ", " << y << ")";
	TitlePrint(l_oss.str());
}

/**
 * タイトル出力（ホイール値）
 *
 * \param
 * \return void
 */
void CT_IW_WIN::TitlePrint_WheelVal()
{
	while (!m_Mouse.IsEmpty())
	{
		//変数宣言
		const CT_MOUSE_EVENTS l_Event = m_Mouse.ReadBuffer();
		static int l_Cnt{0};
		std::ostringstream l_oss{};

		//更新処理
		switch (l_Event.GetType())
		{
			case ET_MOUSE_STATUS::me_WheelUp:
				l_Cnt++;
				l_oss << "Wheel Val : " << l_Cnt;
				TitlePrint(l_oss.str());
				break;
			case ET_MOUSE_STATUS::me_WheelDown:
				l_Cnt--;
				l_oss << "Wheel Val : " << l_Cnt;
				TitlePrint(l_oss.str());
				break;
			default:
				l_oss << "Wheel Val : " << l_Cnt;
				TitlePrint(l_oss.str());
				break;
		}
	}
}

/**
 * マウス使用
 *
 * \param
 * \return void
 */
void CT_IW_WIN::EnableCursor() noexcept
{
	m_bDrawCursor = true;
	ShowCursor();
	UnlockCursor();

#ifdef IMGUI

	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;

#endif // IMGUI
}

/**
 * マウス不使用
 *
 * \param
 * \return void
 */
void CT_IW_WIN::DisableCursor() noexcept
{
	m_bDrawCursor = false;
	HideCursor();
	LockCursor();

#ifdef IMGUI

	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;

#endif // IMGUI
}

/**
 * マウス使用状態確認
 *
 * \param
 * \return bool
 */
bool CT_IW_WIN::IsUsingCursor() const noexcept
{
	return m_bDrawCursor;
}

/**
 * WndProc初期化
 *
 * \param hWnd
 * \param uMsg
 * \param wParam
 * \param lParam
 * \return LRESULT
 */
LRESULT CALLBACK CT_IW_WIN::WndProc_Init(const HWND hWnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam) noexcept
{
	//Instance作成時
	if (uMsg == WM_NCCREATE)
	{
		//Instanceポインタを取得
		const CREATESTRUCT* const l_pCreateInfo = reinterpret_cast<CREATESTRUCT*>(lParam);
		const auto l_pGameWnd = static_cast<CT_IW_WIN*>(l_pCreateInfo->lpCreateParams);

		//InstanceポインタをWinAPIのユーザデータとして登録（WinAPIはクラスのメンバ関数を認識できない為）
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(l_pGameWnd));

		//初期化以後のWndProc呼び出し関数を設定
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&CT_IW_WIN::WndProc_Call));

		//WndProc本処理
		return l_pGameWnd->WndProc(hWnd, uMsg, wParam, lParam);
	}

	//WM_NCCREATE以前の処理先
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/**
 * WndProc呼び出し
 *
 * \param hWnd
 * \param uMsg
 * \param wParam
 * \param lParam
 * \return LRESULT
 */
LRESULT CALLBACK CT_IW_WIN::WndProc_Call(const HWND hWnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam) noexcept
{
	//Instanceポインタを取得
	const auto l_pGameWnd = reinterpret_cast<CT_IW_WIN*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	//WndProc本処理
	return l_pGameWnd->WndProc(hWnd, uMsg, wParam, lParam);
}

/**
 * WndProc本処理
 *
 * \param hWnd
 * \param uMsg
 * \param wParam
 * \param lParam
 * \return LRESULT
 */
LRESULT CT_IW_WIN::WndProc(const HWND& hWnd, const UINT& uMsg, const WPARAM& wParam, const LPARAM& lParam) noexcept
{
#ifdef IMGUI

	//IMGUI用
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	const ImGuiIO& l_Imgui = ImGui::GetIO();

#endif // IMGUI

	switch (uMsg)
	{
		case WM_CLOSE:
			if (MessageBox(hWnd, L"ウィンドウを閉じますか？", L"終了確認", MB_OKCANCEL | MB_ICONQUESTION) != IDOK)
				return 0;
			PostQuitMessage(0); //WM_QUITを生成
			return 0;                   //デストラクタ呼び出し

		//ウィンドウアクティブ判定
		case WM_ACTIVATE:

			//マウス状態更新
			if (!m_bDrawCursor)
			{
				if (wParam & WA_ACTIVE)
				{
					LockCursor();   //アクティブ⇒マウスロック
					HideCursor();
				}
				else
				{
					UnlockCursor(); //非アクティブ⇒マウスロック解除
					ShowCursor();
				}
			}
			break;

		//キーボード処理
		case WM_KILLFOCUS:
			m_Keyboard.ClearState(); //ウィンドウ非アクティブ⇒入力リセット
			break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:          //「WM_SYSKEY」⇒「ALT」と「F10」を対応

#ifdef IMGUI

			if (l_Imgui.WantCaptureKeyboard)
			{
				//IMGUI入力切替
				m_bUseImgui = true;
				break;
			}

#endif // IMGUI

			m_bUseImgui = false;
			if (wParam == VK_ESCAPE)
				PostMessage(hWnd, WM_CLOSE, 0, 0);         //「ESC」⇒ウィンドウ終了
			if (!(lParam & 0x40000000))
				m_Keyboard.KeyPressed(static_cast<unsigned char>(wParam)); //キーを押した
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:

#ifdef IMGUI

			if (l_Imgui.WantCaptureKeyboard)
			{
				//IMGUI入力切替
				m_bUseImgui = true;
				break;
			}

#endif // IMGUI

			m_bUseImgui = false;
			m_Keyboard.KeyReleased(static_cast<unsigned char>(wParam));    //キーを離した
			break;
		case WM_CHAR:

#ifdef IMGUI

			if (l_Imgui.WantCaptureKeyboard)
			{
				//IMGUI入力切替
				m_bUseImgui = true;
				break;
			}

#endif // IMGUI

			m_bUseImgui = false;
			m_Keyboard.CharInput(static_cast<unsigned char>(wParam));      //テキストを入力した
			break;

		//マウス処理
		case WM_MOUSEMOVE:

			//マウス非表示の場合
			if (!m_bDrawCursor)
			{
				if (!m_Mouse.IsInWindow())
				{
					//ウィンドウに入った初回ループ⇒マウスがウィンドウ外判定の場合
					SetCapture(hWnd);
					m_Mouse.EnterWindow(); //キャプチャーオン、マウスメッセージ更新
					HideCursor();          //マウス非表示
				}
				break;                     //マウス座標処理をスキップ
			}

#ifdef IMGUI

			if (l_Imgui.WantCaptureMouse)
			{
				//IMGUI入力切替
				m_bUseImgui = true;
				break;
			}

#endif // IMGUI

			m_bUseImgui = false;
			{
				const POINTS l_Points = MAKEPOINTS(lParam);
				if (l_Points.x >= 0 && l_Points.x < m_Width && l_Points.y >= 0 && l_Points.y < m_Height)
				{
					//ウィンドウ内の場合
					m_Mouse.MouseMove(l_Points.x, l_Points.y);     //座標記録
					if (!m_Mouse.IsInWindow())
					{
						//マウスキャプチャーオン
						SetCapture(hWnd);
						m_Mouse.EnterWindow();
					}
				}
				else
				{
					//ウィンドウ外の場合
					if (wParam & (MK_LBUTTON | MK_RBUTTON))        //クリックしてる場合
						m_Mouse.MouseMove(l_Points.x, l_Points.y); //座標記録
					else
					{
						//クリックしてない場合
						ReleaseCapture();                          //マウスキャプチャーオフ
						m_Mouse.LeaveWindow();
					}
				}
			}
			break;
		case WM_LBUTTONDOWN:
			SetForegroundWindow(hWnd); //ウィンドウをアクティブ
			if (!m_bDrawCursor)
			{
				//マウス状態更新
				LockCursor();
				HideCursor();
			}

#ifdef IMGUI

			if (l_Imgui.WantCaptureMouse)
			{
				//IMGUI入力切替
				m_bUseImgui = true;
				break;
			}

#endif // IMGUI

			m_bUseImgui = false;
			m_Mouse.LeftPressed();
			break;
		case WM_LBUTTONUP:

#ifdef IMGUI

			if (l_Imgui.WantCaptureMouse)
			{
				//IMGUI入力切替
				m_bUseImgui = true;
				break;
			}

#endif // IMGUI

			m_bUseImgui = false;
			{
				const POINTS l_Points = MAKEPOINTS(lParam);
				if (l_Points.x < 0 || l_Points.x >= m_Width || l_Points.y < 0 || l_Points.y >= m_Height)
				{
					//ウィンドウ外の場合
					ReleaseCapture(); //マウスキャプチャーオフ
					m_Mouse.LeaveWindow();
				}
			}
			m_Mouse.LeftReleased();
			break;
		case WM_RBUTTONDOWN:

#ifdef IMGUI

			if (l_Imgui.WantCaptureMouse)
			{
				//IMGUI入力切替
				m_bUseImgui = true;
				break;
			}

#endif // IMGUI

			m_bUseImgui = false;
			m_Mouse.RightPressed();
			break;
		case WM_RBUTTONUP:

#ifdef IMGUI

			if (l_Imgui.WantCaptureMouse)
			{
				//IMGUI入力切替
				m_bUseImgui = true;
				break;
			}

#endif // IMGUI

			m_bUseImgui = false;
			{
				const POINTS l_Points = MAKEPOINTS(lParam);
				if (l_Points.x < 0 || l_Points.x >= m_Width || l_Points.y < 0 || l_Points.y >= m_Height)
				{
					//ウィンドウ外の場合
					ReleaseCapture(); //マウスキャプチャーオフ
					m_Mouse.LeaveWindow();
				}
			}
			m_Mouse.RightReleased();
			break;
		case WM_MOUSEWHEEL:

#ifdef IMGUI

			if (l_Imgui.WantCaptureMouse)
			{
				//IMGUI入力切替
				m_bUseImgui = true;
				break;
			}

#endif // IMGUI

			m_bUseImgui = false;
			{
				const int l_Delta = GET_WHEEL_DELTA_WPARAM(wParam); //ホイール操作量取得(メッセージ一回で+-120)
				m_Mouse.WheelProc(l_Delta);
			}
			break;

		//RawInput処理
		case WM_INPUT:
		{
			if (!m_Mouse.IsUsingRawInput())
				break;
			UINT l_Size{};

			//入力情報のサイズを取得
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &l_Size, sizeof(RAWINPUTHEADER)) == static_cast<UINT>(-1))
				break; //エラーメッセージ
			m_RawBuffer.resize(l_Size);

			//入力情報読込
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, m_RawBuffer.data(), &l_Size, sizeof(RAWINPUTHEADER)) != l_Size)
				break; //エラーメッセージ

			//データ受取
			auto& l_RawInput = reinterpret_cast<const RAWINPUT&>(*m_RawBuffer.data());
			if (l_RawInput.header.dwType == RIM_TYPEMOUSE && (l_RawInput.data.mouse.lLastX != 0 || l_RawInput.data.mouse.lLastY != 0))
				m_Mouse.GetRawDelta(l_RawInput.data.mouse.lLastX, l_RawInput.data.mouse.lLastY);

			break;
		}

		default:
			break;
	}

	//基本動作をサポート
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/**
 * マウス非表示
 *
 * \param
 * \return void
 */
void CT_IW_WIN::HideCursor() noexcept
{
	while (::ShowCursor(FALSE) >= 0);
}

/**
 * マウス表示
 *
 * \param
 * \return void
 */
void CT_IW_WIN::ShowCursor() noexcept
{
	while (::ShowCursor(TRUE) < 0);
}

/**
 * マウスをウィンドウ内にロック
 *
 * \param
 * \return void
 */
void CT_IW_WIN::LockCursor() const noexcept
{
	RECT l_Rect{};
	GetClientRect(m_WinHandle, &l_Rect);
	MapWindowPoints(m_WinHandle, nullptr, reinterpret_cast<POINT*>(&l_Rect), 2); //ウィンドウ内座標をスクリーン座標へ変換
	ClipCursor(&l_Rect);                                                                              //マウスを指定範囲内にロックする
}

/**
 * マウスをウィンドウ内にロック解除
 *
 * \param
 * \return void
 */
void CT_IW_WIN::UnlockCursor() const noexcept
{
	ClipCursor(nullptr);

	//カーソルを画面中心へ戻す
	RECT l_Rect{};
	GetClientRect(m_WinHandle, &l_Rect);
	MapWindowPoints(m_WinHandle, nullptr, reinterpret_cast<POINT*>(&l_Rect), 2); //ウィンドウ内座標をスクリーン座標へ変換
	const int l_X = l_Rect.left + m_Width / 2;
	const int l_Y = l_Rect.top + m_Height / 2;
	SetCursorPos(l_X, l_Y);
}
