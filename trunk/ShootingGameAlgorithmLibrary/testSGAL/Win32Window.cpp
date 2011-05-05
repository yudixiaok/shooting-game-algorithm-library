/**
 * @file Win32Window 詳細說明
 *
 * 這是一個專門產生視窗的類別，像下面這樣，處理訊息的函數，必需從外面提供給ToCreateWindow，否則會使用預設的函數。
 *LRESULT CALLBACK Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
 *		{
 *			switch (message)
 *				{
 *					case WM_DESTROY:
 *					PostQuitMessage(0);
 *					break;
 *					default:
 *					return DefWindowProc(hWnd, message, wParam, lParam);
 *				}
 *			return 0;
 *		}
 *ToCreateWindow的前五項函數是用來描述視窗的，第六項就是要像上面的函數, 記得要儲存回傳的HWND控制碼。
 *GetHandle只能得到最後一個視窗的控制碼。
 *注意！！！在無限迴圈中必需自行呼叫HandlePeekMessage來處理訊息。
 *GetMouseState可以獲得滑鼠狀態。
 *
 * 
 */

#include "Win32Window.h"
#include "resource.h"

Win32Window::Win32Window(void):m_style(WS_POPUPWINDOW|WS_CAPTION),m_EX_style(WS_EX_OVERLAPPEDWINDOW)
{
	m_hInstance = GetModuleHandle(NULL);
}

HWND Win32Window::ToCreateWindow(int x, int y, int width, int height, const wchar_t *title, 
				 LRESULT (_stdcall *wndporc)(HWND, UINT, WPARAM, LPARAM)/*= &Win32Window::Proc*/)
{
	if (x < 0 || y < 0)
	{
		int w = GetSystemMetrics(SM_CXSCREEN);
		x = w/2-width/2;
		int h = GetSystemMetrics(SM_CYSCREEN);
		y = h/2-height/2;
	}
	m_Height = height;
	m_Width = width;
	WNDCLASS	window_class;
	memset(&window_class, 0, sizeof(WNDCLASS));///<設定視窗型態。

	window_class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;///<設定訊息處理函式為WndProc。

	window_class.lpfnWndProc = wndporc;///<取得目前執行的程式。

	window_class.hInstance = m_hInstance;///<使用內定的滑鼠游標。

	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);///<背景設為黑色, 其實有沒有設都沒差.。

	window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);///<給定一個註冊視窗時使用的名字。
	window_class.hIcon = NULL;
	//window_class.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_ICON1));
	window_class.lpszClassName = title;///<跟作業系統註冊一個新的視窗。

	if (RegisterClass(&window_class) == 0)
	{
		return false;
	}
	///<計算視窗的邊線會佔掉的點數, 必要時要增加視窗大小。
	RECT window_rect;
	SetRect(&window_rect, x, y, x+width, y+height);
	AdjustWindowRect(&window_rect, m_style, FALSE); ///<WS_POPUPWINDOW是單框線視窗。
   /**
     * @brief
     *
     * 準備工作完成, 開啟一個視窗
     */
	HWND window_handle = CreateWindowEx(
		m_EX_style, ///<WS_EX_TOPMOST
		title,
		title,
		m_style,
		window_rect.left,	///< x
		window_rect.top,	///< y
		window_rect.right - window_rect.left,	///< width
		window_rect.bottom - window_rect.top,	///< height
		NULL,
		NULL,
		window_class.hInstance,
		NULL
		);
	
	if (window_handle == NULL)
		return false;
	m_hWnd = window_handle;	
	return window_handle;
}

HWND Win32Window::ToFullScreen(int width, int height, WNDPROC wndproc/*= &Win32Window::Proc*/)
{
	wchar_t *title = L"fullscreen";
	DEVMODE dmScreenSettings;								///< Device Mode
	memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	///< Makes Sure Memory's Cleared
	dmScreenSettings.dmSize=sizeof(dmScreenSettings);		///< Size Of The Devmode Structure
	dmScreenSettings.dmPelsWidth	= width;				///< Selected Screen Width
	dmScreenSettings.dmPelsHeight	= height;				///< Selected Screen Height
	dmScreenSettings.dmBitsPerPel	= 32;					///< Selected Bits Per Pixel
	dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

	///< Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
	if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
	{
		///< If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
		return false;
	}


	WNDCLASS	window_class;

	memset(&window_class, 0, sizeof(WNDCLASS));

	///< 設定視窗型態
	window_class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	///< 設定訊息處理函式為WndProc
	window_class.lpfnWndProc = wndproc;
	///< 取得目前執行的程式
	window_class.hInstance = GetModuleHandle(NULL);
	///< 使用內定的滑鼠游標
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	///< 背景設為黑色, 其實有沒有設都沒差
	window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	///< 給定一個註冊視窗時使用的名字
	window_class.lpszClassName = title;
	///< 跟作業系統註冊一個新的視窗
	if (RegisterClass(&window_class) == 0)
		return 0;
#ifdef _DEBUG
	DWORD dwExStyle=WS_EX_APPWINDOW;	// Window Extended Style
#else
	DWORD dwExStyle=WS_EX_APPWINDOW;	// can't see other windows
#endif	
	DWORD dwStyle=WS_POPUP;			///< Windows Style
	ShowCursor(FALSE);			///< Hide Mouse Pointer

	RECT	WindowRect;			///< Grabs Rectangle Upper Left / Lower Right Values

	WindowRect.left=(long)0;		///< Set Left Value To 0
	WindowRect.right=(long)width;		///< Set Right Value To Requested Width
	WindowRect.top=(long)0;			///< Set Top Value To 0
	WindowRect.bottom=(long)height;		///< Set Bottom Value To Requested Height

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		///< Adjust Window To True Requested Size

	HWND window_handle = CreateWindowEx(	
		dwExStyle,
		L"fullscreen",
		title,
		dwStyle |
		WS_CLIPSIBLINGS |
		WS_CLIPCHILDREN,
		0, 0,
		WindowRect.right-WindowRect.left,
		WindowRect.bottom-WindowRect.top,
		NULL,
		NULL,
		window_class.hInstance,
		NULL
		);

	if (window_handle == NULL)
		return false;

	
	m_hWnd = window_handle;///< 記錄下用來代表視窗跟目前執行程式的物件, 以後可以再用到
	m_hInstance = window_class.hInstance;
	ShowWindow(window_handle, SW_SHOWNORMAL);///< 顯示視窗
	SetActiveWindow(window_handle);
	SetForegroundWindow(window_handle);	
	SetFocus(window_handle);

	return m_hWnd;
}

MSG Win32Window::HandlePeekMessage()
{
	MSG msg;
	// 主訊息迴圈:
	if ( PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE) )
	{
		
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}///< 有訊息的話, 就把它送給訊息處理函式
	return msg;
}

HWND Win32Window::GetHandle(void)
{
	return m_hWnd;
}
HINSTANCE Win32Window::GetInstance(void)
{
	return m_hInstance;
}
void Win32Window::GetMouseState(int &x, int &y, int button[3])
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(m_hWnd, &p);
	x = p.x;
	y = p.y;
	button[0] = GetKeyState(VK_LBUTTON) & 0x80 ? 1 : 0;
	button[1] = GetKeyState(VK_MBUTTON) & 0x80 ? 1 : 0;
	button[2] = GetKeyState(VK_RBUTTON) & 0x80 ? 1 : 0;
}

void Win32Window::ToShow(void)
{
	ShowWindow(m_hWnd, SW_SHOWNORMAL);///< 顯示視窗
	SetActiveWindow(m_hWnd);
}
void Win32Window::ToHide(void)
{
	
	ShowWindow(m_hWnd, SW_HIDE);///< 顯示視窗
	SetActiveWindow(m_hWnd);
}

void Win32Window::ToMove( UINT x, UINT y, UINT nWidth, UINT nHeight, bool bRepaint /*= true*/ )
{
	MoveWindow(m_hWnd , x, y, nWidth, nHeight, bRepaint);
}

RECT Win32Window::GetRect()
{
	RECT rect;
	GetWindowRect(m_hWnd, &rect);
	return rect;
}

Win32Window::~Win32Window()
{
	DestroyWindow( m_hWnd );
}

void Win32Window::ToMoveCenter()
{
	int w = GetSystemMetrics(SM_CXSCREEN);
	int x = w/2-m_Width/2;
	int h = GetSystemMetrics(SM_CYSCREEN);
	int y = h/2-m_Height/2;
	MoveWindow(m_hWnd , x, y, m_Width, m_Height, true);
}
