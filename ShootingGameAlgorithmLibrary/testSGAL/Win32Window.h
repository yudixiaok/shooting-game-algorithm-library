/*! @brief 這是簡單創造視窗的類別
*/

#ifndef Win32Window_Im40eda407m1229c278a93mm7e5f_H
#define Win32Window_Im40eda407m1229c278a93mm7e5f_H

/** 
*@file Windows 標頭檔
*
* Windows 使用動作函式如下
*/ 
#include <windows.h>

class Win32Window
{	
public:
	Win32Window();
	~Win32Window();
	/**@brief 
	*創造一個視窗
	*/
	HWND	ToCreateWindow(int x, int y, int width, int height, const wchar_t *title, LRESULT (_stdcall *wndporc)(HWND, UINT, WPARAM, LPARAM) = &Win32Window::Proc);
	HWND	ToFullScreen(int width, int height, WNDPROC = &Win32Window::Proc);
	MSG	HandlePeekMessage();
	/**
	* @brief
	* Handle 住視窗
	*/
	HWND		GetHandle();
	HINSTANCE	GetInstance();
	RECT		GetRect();
	/**
	* @brief
	* 得到滑鼠狀態
	*/
	void	GetMouseState(int &x, int &y, int button[3]);
	/**
	* @brief
	* 顯示視窗
	*/
	void	ToShow();
	/**
	* @brief
	* 隱藏視窗
	*/
	void	ToHide();
	/**
	* @brief
	* 移動的中心
	*/
	void	ToMoveCenter();
	/**
	* @brief
	* 移動到區域
	*/
	void	ToMove(UINT x, UINT y, UINT nWidth, UINT nHeight, bool bRepaint = true);
	
	/*! 得到window的Style */
	DWORD	GetStyle() {return m_style;}
	/*! 設定window的Style */
	void	SetStyle(DWORD style) {m_style = style;}
	/*! 得到window的ExStyle */
	DWORD	GetExStyle() {return m_EX_style;}
	/*! 設定window的ExStyle */
	void	SetExStyle(DWORD style) {m_EX_style = style;}
	/*! 得到視窗的長寬 */
	void	Get_W_H(int &w, int &h) {w = m_Width; h = m_Height;}
	/*! 預設的事件處理函數 */
	static LRESULT CALLBACK Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_DESTROY:
			PostQuitMessage(WM_QUIT);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}
private:
	int m_Height,m_Width;
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	DWORD m_style;
	DWORD m_EX_style;
};


#endif // Win32Window_Im40eda407m1229c278a93mm7e5f_H

