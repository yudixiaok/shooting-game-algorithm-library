

#ifndef GrphaicDirectX9_Im5fa52b0cm122a159526cmm7e5f_H
#define GrphaicDirectX9_Im5fa52b0cm122a159526cmm7e5f_H

#define DIRECTINPUT_VERSION 0x0800
#include <d3d9.h>
#include <d3dx9.h>
#include <boost/thread/detail/singleton.hpp>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) if (x) { x->Release(); x=NULL; }
#define RELEASE_ARRAY(x) if (x) { delete [] x; x=NULL; }
#endif

class GrphaicDirectX9 
{
public:
	GrphaicDirectX9();
	~GrphaicDirectX9(void);
	/// 初始化device
	int InitDevice(HWND hWnd);
	/// 重新初始化device
	bool ResetDevice(void);
	/// 釋放device
	bool ReleaseGraphicsDevice(void);
	/// 傳回device資源
	LPDIRECT3DDEVICE9 GetGraphicsDevice(void);
	/// 回傳d3d9實體
	LPDIRECT3D9 GetD3D9(void);
	LPDIRECT3DVERTEXSHADER9 LoadVertexShader_HLSL(const wchar_t *fullpath, const char *entry, const char *profile);
	LPDIRECT3DPIXELSHADER9 LoadPixelShader_HLSL(const wchar_t *fullpath, const char *entry, const char *profile);
	LPDIRECT3DVERTEXSHADER9 LoadVertexShader_ASM(const wchar_t *fullpath);
	LPDIRECT3DPIXELSHADER9 LoadPixelShader_ASM(const wchar_t *fullpath);
	LPD3DXEFFECT LoadFXShader(const wchar_t *filename);
private:
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	D3DPRESENT_PARAMETERS m_pD3DPresent;
	HWND m_hWnd;
};
typedef boost::detail::thread::singleton<GrphaicDirectX9> GrphaicDirectX9S;

#endif // GrphaicDirectX9_Im5fa52b0cm122a159526cmm7e5f_H

