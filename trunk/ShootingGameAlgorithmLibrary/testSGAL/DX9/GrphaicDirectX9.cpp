
#include "GrphaicDirectX9.h"

GrphaicDirectX9::GrphaicDirectX9():m_pD3D(NULL),m_pD3DDevice(NULL)
{
}

GrphaicDirectX9::~GrphaicDirectX9( void )
{
	ReleaseGraphicsDevice();
}

int GrphaicDirectX9::InitDevice(HWND hWnd)
{
	m_hWnd = hWnd;
	RECT rect;
	GetWindowRect(m_hWnd, &rect);

	LONG w = rect.right - rect.left;
	LONG h = rect.bottom - rect.top;
	if ( w==0 || h==0 )
		return false;

	int multisamples = 0;

	// `取得一個D3D9物件, 它的唯一功用是去開啟真正可以拿來繪圖的D3D9 Device.`
	if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return false;

	ZeroMemory( &m_pD3DPresent, sizeof(m_pD3DPresent) );
	m_pD3DPresent.Windowed = TRUE;
	m_pD3DPresent.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_pD3DPresent.BackBufferFormat = D3DFMT_UNKNOWN;
	m_pD3DPresent.BackBufferCount = 1; // `提供一塊backbuffer`
	m_pD3DPresent.EnableAutoDepthStencil = TRUE;
	m_pD3DPresent.AutoDepthStencilFormat = D3DFMT_D24S8;
	m_pD3DPresent.MultiSampleType = (D3DMULTISAMPLE_TYPE) multisamples;
	m_pD3DPresent.hDeviceWindow = m_hWnd;
	m_pD3DPresent.BackBufferWidth = 1280;
	m_pD3DPresent.BackBufferHeight = 800;
	bool device_initialized = false;

	/*

	`試著用4種不同方法來開啟d3d9`
	1. D3DDEVTYPE_HAL + D3DCREATE_HARDWARE_VERTEXPROCESSING 
	`完全交由GPU處理`
	2. D3DDEVTYPE_HAL + D3DCREATE_MIXED_VERTEXPROCESSING	
	`Vertex可由CPU或GPU來處理, 其它是GPU.`
	3. D3DDEVTYPE_HAL + D3DCREATE_SOFTWARE_VERTEXPROCESSING 
	`Vertex是CPU, 其它是GPU.`
	4. D3DDEVTYPE_REF + D3DCREATE_SOFTWARE_VERTEXPROCESSING	
	`完全由CPU處理, 這是個非常慢的模式.`

	*/

	const int device_types = 4;

	struct sDeviceType
	{
		D3DDEVTYPE type;
		DWORD behavior;
	};

	sDeviceType device_type[device_types] = 
	{
		{D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING},
		{D3DDEVTYPE_HAL, D3DCREATE_MIXED_VERTEXPROCESSING},
		{D3DDEVTYPE_HAL, D3DCREATE_SOFTWARE_VERTEXPROCESSING},
		{D3DDEVTYPE_REF, D3DCREATE_SOFTWARE_VERTEXPROCESSING}
	};

	for ( int type=0; type<device_types; type++ )
	{
		// `試著去開啟一個Direct3D9裝置`
		if( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, device_type[type].type, m_hWnd,
			device_type[type].behavior, &m_pD3DPresent, &m_pD3DDevice )==D3D_OK )
		{
			device_initialized = true;
			break;
		}
	}

	return device_initialized;
}

bool GrphaicDirectX9::ResetDevice( void )
{
	RECT rect;
	GetWindowRect( m_hWnd, &rect);
	
	LONG w = rect.right - rect.left;
	LONG h = rect.bottom - rect.top;
	if ( w==0 || h==0 )
		return false;

	m_pD3DPresent.BackBufferWidth = 0;
	m_pD3DPresent.BackBufferHeight = 0;
	m_pD3DPresent.BackBufferCount = 1; // `提供一塊backbuffer`

	if ( D3D_OK==m_pD3DDevice->Reset(&m_pD3DPresent) )
		return true;
	else
		return false;
}

bool GrphaicDirectX9::ReleaseGraphicsDevice( void )
{
	if ( m_pD3DDevice )
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}
	if ( m_pD3D )
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
	return true;
}

LPDIRECT3DDEVICE9 GrphaicDirectX9::GetGraphicsDevice( void )
{
	return m_pD3DDevice;
}

LPDIRECT3DVERTEXSHADER9 GrphaicDirectX9::LoadVertexShader_HLSL( const wchar_t *fullpath, const char *entry, const char *profile )
{
	LPD3DXBUFFER pShader = NULL;
	LPD3DXBUFFER pErrorMsg = NULL;
	DWORD flags = 0;
#ifdef _DEBUG
	flags |= D3DXSHADER_DEBUG;
#endif

	HRESULT result = D3DXCompileShaderFromFileW(
		fullpath,
		NULL,
		NULL,
		entry,
		profile,
		flags,
		&pShader,
		&pErrorMsg,
		NULL
		);

	if ( result != S_OK )
	{
		if ( pErrorMsg )
		{
			pErrorMsg->Release();
		}
		return NULL;
	}

	LPDIRECT3DVERTEXSHADER9 pVertexShader = NULL;
	result = m_pD3DDevice->CreateVertexShader( (DWORD*)pShader->GetBufferPointer(), &pVertexShader);
	if ( result!=S_OK )
	{
	}

	pShader->Release();

	return pVertexShader;
}

LPDIRECT3DPIXELSHADER9 GrphaicDirectX9::LoadPixelShader_HLSL( const wchar_t *fullpath, const char *entry, const char *profile )
{
	LPD3DXBUFFER pShader = NULL;
	LPD3DXBUFFER pErrorMsg = NULL;
	DWORD flags = 0;
#ifdef _DEBUG
	flags |= D3DXSHADER_DEBUG;
#endif

	HRESULT result = D3DXCompileShaderFromFileW(
		fullpath,
		NULL,
		NULL,
		entry,
		profile,
		flags,
		&pShader,
		&pErrorMsg,
		NULL
		);

	if ( result != S_OK )
	{
		if ( pErrorMsg )
		{
			pErrorMsg->Release();
		}
		return NULL;
	}

	LPDIRECT3DPIXELSHADER9 pPixelShader = NULL;
	result = m_pD3DDevice->CreatePixelShader( (DWORD*)pShader->GetBufferPointer(), &pPixelShader);
	if ( result!=S_OK )
	{
	}

	pShader->Release();

	return pPixelShader;
}

LPDIRECT3DVERTEXSHADER9 GrphaicDirectX9::LoadVertexShader_ASM( const wchar_t *fullpath )
{
	LPD3DXBUFFER pShader = NULL;
	LPD3DXBUFFER pErrorMsg = NULL;
	DWORD flags = 0;
#ifdef _DEBUG
	flags |= D3DXSHADER_DEBUG;
#endif

	HRESULT result = D3DXAssembleShaderFromFile(
		fullpath,
		NULL,
		NULL,
		flags,
		&pShader,
		&pErrorMsg
		);

	if ( result != S_OK )
	{
		if ( pErrorMsg )
		{
			pErrorMsg->Release();
		}
	}

	LPDIRECT3DVERTEXSHADER9 pVertexShader = NULL;
	result = m_pD3DDevice->CreateVertexShader( (DWORD*)pShader->GetBufferPointer(), &pVertexShader);
	if ( result!=S_OK )
	{
	}

	pShader->Release();

	return pVertexShader;
}

LPDIRECT3DPIXELSHADER9 GrphaicDirectX9::LoadPixelShader_ASM( const wchar_t *fullpath )
{
	LPD3DXBUFFER pShader = NULL;
	LPD3DXBUFFER pErrorMsg = NULL;
	DWORD flags = 0;
#ifdef _DEBUG
	flags |= D3DXSHADER_DEBUG;
#endif

	HRESULT result = D3DXAssembleShaderFromFile(
		fullpath,
		NULL,
		NULL,
		flags,
		&pShader,
		&pErrorMsg
		);

	if ( result != S_OK )
	{
		if ( pErrorMsg )
		{
			pErrorMsg->Release();
		}
	}

	LPDIRECT3DPIXELSHADER9 pPixelShader = NULL;
	result = m_pD3DDevice->CreatePixelShader( (DWORD*)pShader->GetBufferPointer(), &pPixelShader);
	if ( result!=S_OK )
	{
	}

	pShader->Release();

	return pPixelShader;
}

LPD3DXEFFECT GrphaicDirectX9::LoadFXShader( const wchar_t *fullpath )
{
	LPD3DXEFFECT pEffect;
	LPD3DXBUFFER pErrorMsg = NULL;
	DWORD flags = 0;
#ifdef _DEBUG
	flags |= D3DXSHADER_DEBUG;
#endif

	if ( D3D_OK!=D3DXCreateEffectFromFile(m_pD3DDevice, fullpath, NULL, NULL, flags, NULL, &pEffect, &pErrorMsg) )
	{
		if ( pErrorMsg )
		{
		}
	}

	SAFE_RELEASE(pErrorMsg);

	return pEffect;
}


LPDIRECT3D9 GrphaicDirectX9::GetD3D9( void )
{
	return m_pD3D;
}
