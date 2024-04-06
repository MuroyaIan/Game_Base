
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
WIN_WINDOW::WIN_WINDOW(LPCWSTR WindowName, int nWndWidth, int nWndHeight, int nWndPosX, int nWndPosY) : IF_WINDOW(),
	m_hAppInst(GetModuleHandle(nullptr)), m_PosX(nWndPosX), m_PosY(nWndPosY), m_Width(nWndWidth), m_Height(nWndHeight), m_hWindow(nullptr),
	m_bDrawCursor(true), m_RawBuffer(0), m_useImgui(false)
{
	//ウィンドウクラス登録
	WNDCLASSEX WindowClass{};
	WindowClass.cbSize = static_cast<UINT>(sizeof(WNDCLASSEX));
	WindowClass.style = CS_OWNDC;
	WindowClass.lpfnWndProc = WIN_WINDOW::WndProc_Init;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = m_hAppInst;
	WindowClass.hIcon = LoadIcon(m_hAppInst, MAKEINTRESOURCE(IDI_ICON_BASE));			//アイコン
	WindowClass.hCursor = LoadCursor(m_hAppInst, MAKEINTRESOURCE(IDC_CURSOR_BASE));		//カーソル
	WindowClass.hbrBackground = nullptr;
	WindowClass.lpszMenuName = nullptr;
	WindowClass.lpszClassName = CLASS_NAME;												//クラス名
	WindowClass.hIconSm = LoadIcon(m_hAppInst, MAKEINTRESOURCE(IDI_ICON_BASE));			//小アイコン
	if (!RegisterClassEx(&WindowClass))
		throw ERROR_DEFAULT();

	//サイズ算出
	DWORD dwExStyle = 0;
	DWORD dwStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
	RECT rcWnd = { 0L, 0L, static_cast<LONG>(m_Width), static_cast<LONG>(m_Height) };	//Client領域サイズ設定
	if (!AdjustWindowRectEx(&rcWnd, dwStyle, false, dwExStyle))							//Windowサイズ算出
		throw ERROR_DEFAULT();
	int nWidth = rcWnd.right - rcWnd.left;
	int nHeight = rcWnd.bottom - rcWnd.top;												//幅・高さを設定

	//Instance作成
	m_hWindow = CreateWindowEx(
		dwExStyle, CLASS_NAME, WindowName, dwStyle,
		m_PosX, m_PosY, nWidth, nHeight,
		nullptr, nullptr, m_hAppInst, this);
	if (!m_hWindow)
		throw ERROR_DEFAULT();

	//Window表示
	ShowWindow(m_hWindow, SW_SHOW);
	UpdateWindow(m_hWindow);

	//IME無効化
	ImmAssociateContext(m_hWindow, nullptr);

	//RawInputデバイス登録
	RAWINPUTDEVICE rid{};
	rid.usUsagePage = 0x01;		//マウス識別用
	rid.usUsage = 0x02;			//マウス識別用
	rid.dwFlags = 0;
	rid.hwndTarget = nullptr;
	if (!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
		throw ERROR_EX2("RawInput初期化失敗");

#ifdef IMGUI

	//IMGUI初期化
	if (!ImGui_ImplWin32_Init(m_hWindow))
		throw ERROR_EX2("IMGUI初期化失敗");

#endif // IMGUI

}

WIN_WINDOW::~WIN_WINDOW() noexcept(false)
{

#ifdef IMGUI

	//IMGUI終了
	ImGui_ImplWin32_Shutdown();

#endif // IMGUI

	//Instance破棄
	if(!DestroyWindow(m_hWindow))
		throw ERROR_DEFAULT();

	//ウィンドウクラス登録解除
	if(!UnregisterClass(CLASS_NAME, m_hAppInst))
		throw ERROR_DEFAULT();
}

//トランスフォーム
void WIN_WINDOW::Transform(int nWndPosX, int nWndPosY, int nWndWidth, int nWndHeight)
{
	//サイズ取得
	RECT rcClient;								//クライアント領域
	if (!GetClientRect(m_hWindow, &rcClient))
		throw ERROR_DEFAULT();
	rcClient.right -= rcClient.left;
	rcClient.bottom -= rcClient.top;
	RECT rcWindow;								//非クライアント領域
	if (!GetWindowRect(m_hWindow, &rcWindow))
		throw ERROR_DEFAULT();
	rcWindow.right -= rcWindow.left;
	rcWindow.bottom -= rcWindow.top;

	//サイズ更新
	int nWidth = 0;
	int nHeight = 0;
	if (nWndWidth <= 0)
		nWidth = rcWindow.right;
	else {
		nWidth = rcWindow.right - rcClient.right + nWndWidth;
		m_Width = nWndWidth;
	}
	if (nWndHeight <= 0)
		nHeight = rcWindow.bottom;
	else {
		nHeight = rcWindow.bottom - rcClient.bottom + nWndHeight;
		m_Height = nWndHeight;
	}

	//Window移動
	if (!SetWindowPos(m_hWindow, HWND_TOP, nWndPosX, nWndPosY, nWidth, nHeight, SWP_SHOWWINDOW))
		throw ERROR_DEFAULT();
}

//タイトル出力
void WIN_WINDOW::TitlePrint(const std::string& Text) const
{
	std::string TitleName = Text;
	if (!SetWindowTextA(m_hWindow, TitleName.c_str()))
		throw ERROR_DEFAULT();
}

//タイトル出力（マウス座標）
void WIN_WINDOW::TitlePrint_MousePos() const
{
	//メッセージボックス表示バグあり
	auto [x, y] = m_Mouse.GetPos();
	std::ostringstream oss;
	oss << "MousePos:(" << x << ", " << y << ")";
	TitlePrint(oss.str());
}

//タイトル出力（ホイール値）
void WIN_WINDOW::TitlePrint_WheelVal()
{
	while (!m_Mouse.IsEmpty()) {

		//変数宣言
		const CT_MOUSE_EVENTS Event = m_Mouse.ReadBuffer();
		static int nCnt = 0;
		std::ostringstream oss;

		//更新処理
		switch (Event.GetType()) {
			case ET_MOUSE_STATUS::me_WheelUp:
				nCnt++;
				oss << "ホイール値：" << nCnt;
				TitlePrint(oss.str());
				break;
			case ET_MOUSE_STATUS::me_WheelDown:
				nCnt--;
				oss << "ホイール値：" << nCnt;
				TitlePrint(oss.str());
				break;
			default:
				oss << "ホイール値：" << nCnt;
				TitlePrint(oss.str());
				break;
		}
	}
}

//マウス使用・不使用
void WIN_WINDOW::EnableCursor() noexcept
{
	m_bDrawCursor = true;
	ShowCursor();
	UnlockCursor();

#ifdef IMGUI

	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;

#endif // IMGUI

}

void WIN_WINDOW::DisableCursor() noexcept
{
	m_bDrawCursor = false;
	HideCursor();
	LockCursor();

#ifdef IMGUI

	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;

#endif // IMGUI

}

//マウス使用状態確認
bool WIN_WINDOW::IsUsingCursor() const noexcept
{
	return m_bDrawCursor;
}

//WndProc初期化
LRESULT CALLBACK WIN_WINDOW::WndProc_Init(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	//Instance作成時
	if (uMsg == WM_NCCREATE)
	{
		//Instanceポインタを取得
		const CREATESTRUCT* const pCreateInfo = reinterpret_cast<CREATESTRUCT*>(lParam);
		WIN_WINDOW* const pGameWnd = static_cast<WIN_WINDOW*>(pCreateInfo->lpCreateParams);

		//InstanceポインタをWinAPIのユーザデータとして登録（WinAPIはクラスのメンバ関数を認識できない為）
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pGameWnd));

		//初期化以後のWndProc呼び出し関数を設定
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WIN_WINDOW::WndProc_Call));

		//WndProc本処理
		return pGameWnd->WndProc(hWnd, uMsg, wParam, lParam);
	}

	//WM_NCCREATE以前の処理先
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//WndProc呼び出し
LRESULT CALLBACK WIN_WINDOW::WndProc_Call(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	//Instanceポインタを取得
	WIN_WINDOW* const pGameWnd = reinterpret_cast<WIN_WINDOW*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	//WndProc本処理
	return pGameWnd->WndProc(hWnd, uMsg, wParam, lParam);
}

//WndProc本処理
LRESULT WIN_WINDOW::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{

#ifdef IMGUI

	//IMGUI用
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	const ImGuiIO& io = ImGui::GetIO();

#endif // IMGUI

	switch (uMsg) {
		case WM_CLOSE:
			if (MessageBox(hWnd, L"ウィンドウを閉じますか？", L"終了確認", MB_OKCANCEL | MB_ICONQUESTION) != IDOK)
				return 0;
			PostQuitMessage(0);				//WM_QUITを生成
			return 0;						//デストラクタ呼び出し

		//ウィンドウアクティブ判定
		case WM_ACTIVATE:

			//マウス状態更新
			if (!m_bDrawCursor) {
				if (wParam & WA_ACTIVE) {
					LockCursor();			//アクティブ⇒マウスロック
					HideCursor();
				}
				else {
					UnlockCursor();			//非アクティブ⇒マウスロック解除
					ShowCursor();
				}
			}
			break;

		//キーボード処理
		case WM_KILLFOCUS:
			m_Keyboard.ClearState();		//ウィンドウ非アクティブ⇒入力リセット
			break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:													//「WM_SYSKEY」⇒「ALT」と「F10」を対応

#ifdef IMGUI

			if (io.WantCaptureKeyboard) {									//IMGUI入力切替
				m_useImgui = true;
				break;
			}

#endif // IMGUI

			m_useImgui = false;
			switch (wParam) {
				case VK_ESCAPE:												//「ESC」⇒ウィンドウ終了
					PostMessage(hWnd, WM_CLOSE, 0, 0);
			}
			if (!(lParam & 0x40000000))
				m_Keyboard.KeyPressed(static_cast<unsigned char>(wParam));	//キーを押した
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:

#ifdef IMGUI

			if (io.WantCaptureKeyboard) {									//IMGUI入力切替
				m_useImgui = true;
				break;
			}

#endif // IMGUI

			m_useImgui = false;
			m_Keyboard.KeyReleased(static_cast<unsigned char>(wParam));		//キーを離した
			break;
		case WM_CHAR:

#ifdef IMGUI

			if (io.WantCaptureKeyboard) {									//IMGUI入力切替
				m_useImgui = true;
				break;
			}

#endif // IMGUI

			m_useImgui = false;
			m_Keyboard.CharInput(static_cast<unsigned char>(wParam));		//テキストを入力した
			break;

		//マウス処理
		case WM_MOUSEMOVE:

			//マウス非表示の場合
			if (!m_bDrawCursor) {
				if (!m_Mouse.IsInWindow()) {	//ウィンドウに入った初回ループ⇒マウスがウィンドウ外判定の場合
					SetCapture(hWnd);
					m_Mouse.EnterWindow();		//キャプチャーオン、マウスメッセージ更新
					HideCursor();				//マウス非表示
				}
				break;							//マウス座標処理をスキップ
			}

#ifdef IMGUI

			if (io.WantCaptureMouse) {												//IMGUI入力切替
				m_useImgui = true;
				break;
			}

#endif // IMGUI

			m_useImgui = false;
			{
				POINTS pt = MAKEPOINTS(lParam);
				if (pt.x >= 0 && pt.x < m_Width && pt.y >= 0 && pt.y < m_Height) {	//ウィンドウ内の場合
					m_Mouse.MouseMove(pt.x, pt.y);									//座標記録
					if (!m_Mouse.IsInWindow()) {									//マウスキャプチャーオン
						SetCapture(hWnd);
						m_Mouse.EnterWindow();
					}
				}
				else {																//ウィンドウ外の場合
					if (wParam & (MK_LBUTTON | MK_RBUTTON))							//クリックしてる場合
						m_Mouse.MouseMove(pt.x, pt.y);								//座標記録
					else {															//クリックしてない場合
						ReleaseCapture();											//マウスキャプチャーオフ
						m_Mouse.LeaveWindow();
					}
				}
			}
			break;
		case WM_LBUTTONDOWN:
			SetForegroundWindow(hWnd);												//ウィンドウをアクティブ
			if (!m_bDrawCursor) {													//マウス状態更新
				LockCursor();
				HideCursor();
			}

#ifdef IMGUI

			if (io.WantCaptureMouse) {												//IMGUI入力切替
				m_useImgui = true;
				break;
			}

#endif // IMGUI

			m_useImgui = false;
			m_Mouse.LeftPressed();
			break;
		case WM_LBUTTONUP:

#ifdef IMGUI

			if (io.WantCaptureMouse) {												//IMGUI入力切替
				m_useImgui = true;
				break;
			}

#endif // IMGUI

			m_useImgui = false;
			{
				POINTS pt = MAKEPOINTS(lParam);
				if (pt.x < 0 || pt.x >= m_Width || pt.y < 0 || pt.y >= m_Height) {	//ウィンドウ外の場合
					ReleaseCapture();												//マウスキャプチャーオフ
					m_Mouse.LeaveWindow();
				}
			}
			m_Mouse.LeftReleased();
			break;
		case WM_RBUTTONDOWN:

#ifdef IMGUI

			if (io.WantCaptureMouse) {												//IMGUI入力切替
				m_useImgui = true;
				break;
			}

#endif // IMGUI

			m_useImgui = false;
			m_Mouse.RightPressed();
			break;
		case WM_RBUTTONUP:

#ifdef IMGUI

			if (io.WantCaptureMouse) {												//IMGUI入力切替
				m_useImgui = true;
				break;
			}

#endif // IMGUI

			m_useImgui = false;
			{
				POINTS pt = MAKEPOINTS(lParam);
				if (pt.x < 0 || pt.x >= m_Width || pt.y < 0 || pt.y >= m_Height) {	//ウィンドウ外の場合
					ReleaseCapture();												//マウスキャプチャーオフ
					m_Mouse.LeaveWindow();
				}
			}
			m_Mouse.RightReleased();
			break;
		case WM_MOUSEWHEEL:

#ifdef IMGUI

			if (io.WantCaptureMouse) {												//IMGUI入力切替
				m_useImgui = true;
				break;
			}

#endif // IMGUI

			m_useImgui = false;
			{
				const int nDelta = GET_WHEEL_DELTA_WPARAM(wParam);					//ホイール操作量取得(メッセージ一回で+-120)
				m_Mouse.WheelProc(nDelta);
			}
			break;

		//RawInput処理
		case WM_INPUT:
		{
			if (!m_Mouse.IsUsingRawInput())
				break;
			UINT size{};

			//入力情報のサイズを取得
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)) == -1)
				break;	//エラーメッセージ
			m_RawBuffer.resize(size);

			//入力情報読込
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, m_RawBuffer.data(), &size, sizeof(RAWINPUTHEADER)) != size)
				break;	//エラーメッセージ

			//データ受取
			auto& ri = reinterpret_cast<const RAWINPUT&>(*m_RawBuffer.data());
			if (ri.header.dwType == RIM_TYPEMOUSE && (ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
				m_Mouse.GetRawDelta(ri.data.mouse.lLastX, ri.data.mouse.lLastY);

			break;
		}

		default:
			break;
	}

	//基本動作をサポート
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//マウス非表示・表示
void WIN_WINDOW::HideCursor() noexcept
{
	while (::ShowCursor(FALSE) >= 0);
}

void WIN_WINDOW::ShowCursor() noexcept
{
	while (::ShowCursor(TRUE) < 0);
}

//マウスをウィンドウ内にロック・ロック解除
void WIN_WINDOW::LockCursor() noexcept
{
	RECT rect;
	GetClientRect(m_hWindow, &rect);
	MapWindowPoints(m_hWindow, nullptr, reinterpret_cast<POINT*>(&rect), 2);	//ウィンドウ内座標をスクリーン座標へ変換
	ClipCursor(&rect);															//マウスを指定範囲内にロックする
}

void WIN_WINDOW::UnlockCursor() noexcept
{
	ClipCursor(nullptr);

	//カーソルを画面中心へ戻す
	RECT rect;
	GetClientRect(m_hWindow, &rect);
	MapWindowPoints(m_hWindow, nullptr, reinterpret_cast<POINT*>(&rect), 2);	//ウィンドウ内座標をスクリーン座標へ変換
	int x = rect.left + m_Width / 2;
	int y = rect.top + m_Height / 2;
	SetCursorPos(x, y);
}
