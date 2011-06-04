
#include "FlatRenderDx9.h"
#include <algorithm>
#include <cassert>

#define SAFE_RELEASE(x) if (x) { x->Release(); x=NULL; }
#define RELEASE_ARRAY(x) if (x) { delete [] x; x=NULL; }


FlatRenderDx9::FlatRenderDx9(LPDIRECT3DDEVICE9 device)
{
	SetDevice(device);
}

void FlatRenderDx9::SetRenderRect( Rectf rect )
{
	m_Width = abs(rect.right - rect.left);
	m_Height = abs(rect.bottom - rect.top);
	m_HandledWidth = 2 / (float)m_Width;
	m_HandledHeight = 2 / (float)m_Height;
	m_rect = rect;
	if (m_rect.left<0)
		m_fix_x = -m_rect.left/m_Width*2;
	else
		m_fix_x = -m_rect.left/m_Width;
	if (m_rect.top<0)
		m_fix_y = m_rect.top/m_Height*2;
	else
		m_fix_y = m_rect.top/m_Height;
}

bool FlatRenderDx9::LoadPicture(const wchar_t* filepath, std::size_t hashid)
{
	TextureMapIter MapPos = m_TextureMap.find(hashid);
	if (MapPos == m_TextureMap.end())
	{
		//先載入材質
		LPDIRECT3DTEXTURE9 tmpTexture;
		if(D3DXCreateTextureFromFileEx(m_device, filepath, 
			D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, 
			D3DPOOL_MANAGED, D3DX_DEFAULT, 
			D3DX_DEFAULT, 0, NULL, NULL,
			&tmpTexture) != D3D_OK) return false;
		//把id記下
		m_TextureMap.insert(std::make_pair(hashid,tmpTexture));
	}	
	return true;
}

bool FlatRenderDx9::LoadPicture( const wchar_t* filepath, std::string str )
{
	return LoadPicture(filepath, strhasher(str));
}
bool FlatRenderDx9::LoadMapMem( EditBitMap &bitmap, std::size_t hashid )
{
	TextureMapIter MapPos = m_TextureMap.find(hashid);
	if (MapPos == m_TextureMap.end())
	{
		//先載入材質
		LPDIRECT3DTEXTURE9 tmpTexture;
		if ( D3D_OK!=m_device->CreateTexture(bitmap.iWidth, bitmap.iHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &tmpTexture, NULL) )
			return false;
		// 拷具貼圖資料
		D3DLOCKED_RECT locked_rect;
		tmpTexture->LockRect(0, &locked_rect, NULL, 0);
		memcpy(locked_rect.pBits,bitmap.pImage,sizeof(unsigned char)*bitmap.lImageSize);
		tmpTexture->UnlockRect(0);
		//把id記下
		m_TextureMap.insert(std::make_pair(hashid, tmpTexture));
	}	
	return true;
}

bool FlatRenderDx9::LoadBmpMem(unsigned char *BmpPic, int w, int h, int per_bytes, std::size_t hashid)
{
	TextureMapIter MapPos = m_TextureMap.find(hashid);
	if (MapPos == m_TextureMap.end())
	{
		//先載入材質
		LPDIRECT3DTEXTURE9 tmpTexture;
		if ( D3D_OK!=m_device->CreateTexture(w, h, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &tmpTexture, NULL) )
			return false;
		// 拷具貼圖資料
		D3DLOCKED_RECT locked_rect;
		tmpTexture->LockRect(0, &locked_rect, NULL, 0);
		unsigned char *target = (unsigned char *)locked_rect.pBits;
		unsigned char *source = BmpPic;
		if (per_bytes == 3)
		{
			for ( int y=0; y<h; y++ )
			{
				for ( int x=0; x<w;x++ )
				{
					target[0] = source[0];
					target[1] = source[1];
					target[2] = source[2];
					target[3] = 255;
					target+=4;
					source+=3;
				}
			}
		}
		else if (per_bytes == 4)
		{
			for ( int y=0; y<h; y++ )
			{
				for ( int x=0; x<w;x++ )
				{
					target[0] = source[0];
					target[1] = source[1];
					target[2] = source[2];
					target[3] = 255;
					target+=4;
					source+=4;
				}
			}
		}
		tmpTexture->UnlockRect(0);
		//把id記下
		m_TextureMap.insert(std::make_pair(hashid, tmpTexture));
	}	
	return true;
}

bool FlatRenderDx9::LoadTgaMem( unsigned char *TgaPic, int w, int h, int per_bytes, std::size_t hashid )
{
	TextureMapIter MapPos = m_TextureMap.find(hashid);
	if (MapPos == m_TextureMap.end())
	{
		//先載入材質
		LPDIRECT3DTEXTURE9 tmpTexture;
		if ( D3D_OK!=m_device->CreateTexture(w, h, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &tmpTexture, NULL) )
			return false;
		// 拷具貼圖資料
		D3DLOCKED_RECT locked_rect;
		tmpTexture->LockRect(0, &locked_rect, NULL, 0);
		unsigned char *target = (unsigned char *)locked_rect.pBits;
		unsigned char *source = TgaPic;
		if (per_bytes == 4)
		{
			for ( int y=0; y<h; y++ )
			{
				for ( int x=0; x<w; x++ )
				{
					// D3DFMT_A8R8G8B8的記憶體排列順序是BGRA
					// 要對用RGBA排列的記憶體做轉換
					target[0] = source[2];
					target[1] = source[1];
					target[2] = source[0];
					target[3] = source[3];

					target+=4;
					source+=4;
				}
			}
		}
		else if (per_bytes == 3)
		{
			for ( int y=0; y<h; y++ )
			{
				for ( int x=0; x<w; x++ )
				{
					// D3DFMT_A8R8G8B8的記憶體排列順序是BGRA
					// 要對用RGBA排列的記憶體做轉換
					target[0] = source[2];
					target[1] = source[1];
					target[2] = source[0];
					target[3] = 255;

					target+=4;
					source+=3;
				}
			}
		}
		tmpTexture->UnlockRect(0);
		//把id記下
		m_TextureMap.insert(std::make_pair(hashid, tmpTexture));
	}	
	return true;
}

void FlatRenderDx9::ClearLoadPic()
{
	for (TextureMapIter MapPos = m_TextureMap.begin();MapPos != m_TextureMap.end();MapPos++)
	{
		SAFE_RELEASE(MapPos->second);
	}
	m_TextureMap.clear();
}


void FlatRenderDx9::ObjectSet( PicObject* renderobject )
{
	if (renderobject->m_flag[PicObject::HAS_BLEND])
	{
		stPicD3DVertex_Blend objData[4];
		SetPointData(objData, renderobject);
		for (int i=0; i<4; i++)
		{
			objData[i].color_a = renderobject->blend_colors[0];
			objData[i].color_r = renderobject->blend_colors[1];
			objData[i].color_g = renderobject->blend_colors[2];
			objData[i].color_b = renderobject->blend_colors[3];
			objData[i].x=objData[i].x*m_HandledWidth-1+m_fix_x;
			objData[i].y=-objData[i].y*m_HandledHeight+1+m_fix_y;
		}
		m_vPicInfo_Blend.push_back(PicInfo_Blend(objData, renderobject));
	}
	else
	{
		stPicD3DVertex objData[4];
		SetPointData(objData, renderobject);
		for (int i=0; i<4; i++)
		{
			objData[i].x = objData[i].x * m_HandledWidth-1+m_fix_x;
			objData[i].y = -objData[i].y * m_HandledHeight+1+m_fix_y;
		}
		m_vPicInfo.push_back(PicInfo(objData, renderobject));
	}
}


void FlatRenderDx9::MulObjectSet( PicObject* renderobject )
{
	if (renderobject->m_flag[PicObject::HAS_BLEND])
	{
		stPicD3DVertex_Blend objData[4];
		SetMulPointData(objData, renderobject);
		for (int i=0; i<4; i++)
		{
			objData[i].color_a = renderobject->blend_colors[0];
			objData[i].color_r = renderobject->blend_colors[1];
			objData[i].color_g = renderobject->blend_colors[2];
			objData[i].color_b = renderobject->blend_colors[3];
			objData[i].x=objData[i].x*m_HandledWidth-1+m_fix_x;
			objData[i].y=-objData[i].y*m_HandledHeight+1+m_fix_y;
		}
		m_vPicInfo_Blend.push_back(PicInfo_Blend(objData, renderobject));
	}
	else
	{
		stPicD3DVertex objData[4];
		SetMulPointData(objData, renderobject);
		for (int i=0; i<4; i++)
		{
			objData[i].x = objData[i].x * m_HandledWidth-1+m_fix_x;
			objData[i].y = -objData[i].y * m_HandledHeight+1+m_fix_y;
		}
		m_vPicInfo.push_back(PicInfo(objData, renderobject));
	}
}

//每幀的渲染
void FlatRenderDx9::RenderFrame()
{
	if (m_Monopolize)
		RenderBegin();
	// 開啟混色功能
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	// source_blend_factor = source_color.a
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	// dest_blend_factor = (1 - source_color.a)
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// 混色公式 = source_color.rgba * (source_color.a) + dest_color.rgba * (1 - source_color.a)
	// direct3D內定會直接輸出貼圖alpha值, 把它改成跟頂點alpha值相乘.
	m_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	sort(m_vPicInfo.begin(), m_vPicInfo.end());
	if (!m_vPicInfo.empty())
	{
		m_device->SetFVF(D3DFVF_VERTEX);
		UINT uSize = sizeof(stPicD3DVertex) * 4 * m_vPicInfo.size();
		m_device->CreateVertexBuffer(uSize, 0,
			D3DFVF_VERTEX, D3DPOOL_DEFAULT, &m_lpVertexBuffer, NULL);		
		stPicD3DVertex *ptr;
		m_lpVertexBuffer->Lock(0, uSize,(void**)&ptr, 0);
		int iMove = 0;
		for (m_vPicInfoPos = m_vPicInfo.begin();m_vPicInfoPos != m_vPicInfo.end();m_vPicInfoPos++)
		{
			if (m_vPicInfoPos->layer <= 0) continue;
			memcpy(ptr + iMove, &(*m_vPicInfoPos->stPoint), sizeof(stPicD3DVertex) * 4);
			iMove += 4;
		}
		m_lpVertexBuffer->Unlock();
		m_device->SetStreamSource(0, m_lpVertexBuffer, 0, sizeof(stPicD3DVertex));
		iMove = 0;
		for (m_vPicInfoPos = m_vPicInfo.begin();m_vPicInfoPos != m_vPicInfo.end();m_vPicInfoPos++)
		{
			if (m_vPicInfoPos->layer <= 0) continue;
			m_device->SetTexture(0, m_vPicInfoPos->lpTexture);
			m_device->DrawPrimitive(D3DPT_TRIANGLEFAN, iMove, 2);
			iMove += 4;
		}			
		m_lpVertexBuffer->Release();
	}
	sort(m_vPicInfo_Blend.begin(), m_vPicInfo_Blend.end());
	if (!m_vPicInfo_Blend.empty())
	{
		m_device->SetFVF(D3DFVF_VERTEX_BLEND);
		UINT uSize = sizeof(stPicD3DVertex_Blend) * 4 * m_vPicInfo_Blend.size();
		m_device->CreateVertexBuffer(uSize, 0,
			D3DFVF_VERTEX_BLEND, D3DPOOL_DEFAULT, &m_lpVertexBuffer, NULL);		
		stPicD3DVertex_Blend *ptr;
		m_lpVertexBuffer->Lock(0, uSize,(void**)&ptr, 0);
		int iMove = 0;
		for (m_vPicInfoPos_Blend = m_vPicInfo_Blend.begin(); 
			m_vPicInfoPos_Blend != m_vPicInfo_Blend.end();m_vPicInfoPos_Blend++)
		{
			if (m_vPicInfoPos_Blend->layer <= 0) continue;
			memcpy(ptr + iMove, &(*m_vPicInfoPos_Blend->stPoint), sizeof(stPicD3DVertex_Blend) * 4);
			iMove += 4;
		}
		m_lpVertexBuffer->Unlock();
		m_device->SetStreamSource(0, m_lpVertexBuffer, 0, sizeof(stPicD3DVertex_Blend));
		iMove = 0;
		for (m_vPicInfoPos_Blend = m_vPicInfo_Blend.begin();
			m_vPicInfoPos_Blend != m_vPicInfo_Blend.end();m_vPicInfoPos_Blend++)
		{
			if (m_vPicInfoPos_Blend->layer <= 0) continue;
			m_device->SetTexture(0,m_vPicInfoPos_Blend->lpTexture);
			m_device->DrawPrimitive(D3DPT_TRIANGLEFAN, iMove, 2);
			iMove += 4;
		}			
		m_lpVertexBuffer->Release();
	}
	sort(m_SpotLightInfo.begin(), m_SpotLightInfo.end());
	if (!m_SpotLightInfo.empty())
	{
		m_device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
		for (m_SpotLightInfoPos = m_SpotLightInfo.begin();
			m_SpotLightInfoPos != m_SpotLightInfo.end();m_SpotLightInfoPos++)
		{
			RenderSpotLight(&(*m_SpotLightInfoPos));
		}
	}
	if (m_Monopolize)
		RenderEnd();
}
void FlatRenderDx9::AddColorShade(unsigned char *blend_colors) //順序 a,r,g,b 要再加一個SetColorShade
{
	m_device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	stLightPolygon g[]={
		{-1.0f, -1.0f, 0.0f, blend_colors[3], blend_colors[2], blend_colors[1], blend_colors[0]},
		{-1.0f, 1.0f, 0.0f, blend_colors[3], blend_colors[2], blend_colors[1], blend_colors[0]},
		{1.0f, -1.0f, 0.0f, blend_colors[3], blend_colors[2], blend_colors[1], blend_colors[0]},
		{1.0f, -1.0f, 0.0f, blend_colors[3], blend_colors[2], blend_colors[1], blend_colors[0]},
		{1.0f, 1.0f, 0.0f, blend_colors[3], blend_colors[2], blend_colors[1], blend_colors[0]},
		{-1.0f, 1.0f, 0.0f, blend_colors[3], blend_colors[2], blend_colors[1], blend_colors[0]}
	};
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);	
	m_device->SetRenderState(D3DRS_BLENDFACTOR, D3DCOLOR_ARGB(blend_colors[0],blend_colors[0],blend_colors[0],blend_colors[0]) );
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BLENDFACTOR );
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	
	m_device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, g, sizeof(stLightPolygon));
}
//-256~255
void FlatRenderDx9::AddLightShade(short level) //要再加一個SetLightShade
{
	unsigned char mete;
	if (level < 0)
		mete = (unsigned char)(level);
	else
		mete = (unsigned char)(256 + level);
	m_device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	stLightPolygon g[]={
		{-1.0f, -1.0f, 0.0f, mete, mete, mete, mete},
		{-1.0f, 1.0f, 0.0f, mete, mete, mete, mete},
		{1.0f, -1.0f, 0.0f, mete, mete, mete, mete},
		{1.0f, -1.0f, 0.0f, mete, mete, mete, mete},
		{1.0f, 1.0f, 0.0f, mete, mete, mete, mete},
		{-1.0f, 1.0f, 0.0f, mete, mete, mete, mete}
	};
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);		
	if (level < 0)
	{
		m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
		m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVDESTALPHA);
	}
	else
	{
		m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTALPHA);
		m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);
	}
	m_device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, g, sizeof(stLightPolygon));
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}
FlatRenderDx9::~FlatRenderDx9()
{
	for (TextureMapIter MapPos = m_TextureMap.begin();MapPos != m_TextureMap.end();MapPos++)
	{
		SAFE_RELEASE(MapPos->second);
	}
}	
void FlatRenderDx9::SetDevice( LPDIRECT3DDEVICE9 device )
{
	m_device = device;
	m_device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
}

//以向量的方式對點旋轉
void FlatRenderDx9::VectorRotate(Posf p1, Posf p2, Posf &picPoints, float angle )
{
	if (fabs(angle)<0.001f){
		//如果角度太小或近似於0就不轉
		picPoints.x=p2.x;
		picPoints.y=p2.y;
	}else{
		float vectorX,vectorY;
		//先求出旋轉點隊員的的向量
		vectorX =p2.x -p1.x;
		vectorY =p2.y -p1.y;
		//用像樣旋轉公式算出旋轉後的向量,再加上原點就是旋轉後的點位置
		picPoints.x =vectorX * cos(angle) - vectorY * sin(angle) + p1.x;
		picPoints.y =vectorX * sin(angle) + vectorY * cos(angle) + p1.y;
	}
}
void FlatRenderDx9::ClearPicObject()
{
	m_vPicInfo.clear();
	m_vPicInfo_Blend.clear();
	m_SpotLightInfo.clear();
}

void FlatRenderDx9::SetPointData(stPicD3DVertex_Blend *pointdata, PicObject* renderobject)
{
	Posf tPoints[4];
	switch (renderobject->reverse)
	{
	case HORIZONTAL:
		//水平翻轉,把正常的貼圖座標的x反向(1 <-> 0)
		tPoints[0].x=1.0f; tPoints[0].y=0.0f;
		tPoints[1].x=1.0f; tPoints[1].y=1.0f;
		tPoints[2].x=0.0f; tPoints[2].y=0.0f;
		tPoints[3].x=0.0f; tPoints[3].y=1.0f;
		break;
	case VERTICAL:
		//垂直翻轉,把正常的貼圖座標的y反向(1 <-> 0)
		tPoints[0].x=0.0f; tPoints[0].y=1.0f;
		tPoints[1].x=0.0f; tPoints[1].y=0.0f;
		tPoints[2].x=1.0f; tPoints[2].y=1.0f;
		tPoints[3].x=1.0f; tPoints[3].y=0.0f;
		break;
	case DIAGONAL:
		//對角翻轉,把正常的貼圖座標的x,y反向(1 <-> 0)
		tPoints[0].x=1.0f; tPoints[0].y=1.0f;
		tPoints[1].x=1.0f; tPoints[1].y=0.0f;
		tPoints[2].x=0.0f; tPoints[2].y=1.0f;
		tPoints[3].x=0.0f; tPoints[3].y=0.0f;
		break;
	default:
		tPoints[0].x=0.0f; tPoints[0].y=0.0f;
		tPoints[1].x=0.0f; tPoints[1].y=1.0f;
		tPoints[2].x=1.0f; tPoints[2].y=0.0f;
		tPoints[3].x=1.0f; tPoints[3].y=1.0f;
	}
	Posf picPoints[4];
	Posf p1, p2;
	//輸入的是度度量,而內建三角函數是用徑度量,所以先做角度轉換
	float angle=renderobject->angle*3.1415926f/180.0f;
	//p1為原點在畫面上的座標,所以要用origin(以圖的左上為0,0的座標)+圖左上的座標
	p1.x=renderobject->origin.x+renderobject->points[0].x;
	p1.y=renderobject->origin.y+renderobject->points[0].y;
	//將p2分別代入要旋轉的點座標,並呼叫VectorRotate來旋轉該點
	for (int i=0; i<4; i++)
	{
		p2.x=renderobject->points[i].x;
		p2.y=renderobject->points[i].y;
		VectorRotate(p1, p2, picPoints[i], angle);
	}
	//將座標填入各自的位置
	for(int i=0;i<4;i++){

		pointdata[i].x=picPoints[i].x;
		pointdata[i].y=picPoints[i].y;
		pointdata[i].z=0;
		pointdata[i].tu=tPoints[i].x;
		pointdata[i].tv=tPoints[i].y;
	}
	std::swap(pointdata[2], pointdata[3]);
}

void FlatRenderDx9::SetPointData(stPicD3DVertex *pointdata, PicObject* renderobject)
{
	Posf tPoints[4];
	switch (renderobject->reverse)
	{
	case HORIZONTAL:
		//水平翻轉,把正常的貼圖座標的x反向(1 <-> 0)
		tPoints[0].x=1.0f; tPoints[0].y=0.0f;
		tPoints[1].x=1.0f; tPoints[1].y=1.0f;
		tPoints[2].x=0.0f; tPoints[2].y=0.0f;
		tPoints[3].x=0.0f; tPoints[3].y=1.0f;
		break;
	case VERTICAL:
		//垂直翻轉,把正常的貼圖座標的y反向(1 <-> 0)
		tPoints[0].x=0.0f; tPoints[0].y=1.0f;
		tPoints[1].x=0.0f; tPoints[1].y=0.0f;
		tPoints[2].x=1.0f; tPoints[2].y=1.0f;
		tPoints[3].x=1.0f; tPoints[3].y=0.0f;
		break;
	case DIAGONAL:
		//對角翻轉,把正常的貼圖座標的x,y反向(1 <-> 0)
		tPoints[0].x=1.0f; tPoints[0].y=1.0f;
		tPoints[1].x=1.0f; tPoints[1].y=0.0f;
		tPoints[2].x=0.0f; tPoints[2].y=1.0f;
		tPoints[3].x=0.0f; tPoints[3].y=0.0f;
		break;
	default:
		tPoints[0].x=0.0f; tPoints[0].y=0.0f;
		tPoints[1].x=0.0f; tPoints[1].y=1.0f;
		tPoints[2].x=1.0f; tPoints[2].y=0.0f;
		tPoints[3].x=1.0f; tPoints[3].y=1.0f;
	}

	Posf picPoints[4];
	Posf p1, p2;
	//輸入的是度度量,而內建三角函數是用徑度量,所以先做角度轉換
	float angle=renderobject->angle*3.1415926f/180;
	//p1為原點在畫面上的座標,所以要用origin(以圖的左上為0,0的座標)+圖左上的座標
	p1.x=renderobject->origin.x+renderobject->points[0].x;
	p1.y=renderobject->origin.y+renderobject->points[0].y;
	//將p2分別代入要旋轉的點座標,並呼叫VectorRotate來旋轉該點
	for (int i=0; i<4; i++)
	{
		p2.x=renderobject->points[i].x;
		p2.y=renderobject->points[i].y;
		VectorRotate(p1, p2, picPoints[i], angle);
	}
	//將座標填入各自的位置
	for(int ix=0;ix<4;ix++){

		pointdata[ix].x=picPoints[ix].x;
		pointdata[ix].y=picPoints[ix].y;
		pointdata[ix].z=0;
		pointdata[ix].tu=tPoints[ix].x;
		pointdata[ix].tv=tPoints[ix].y;
	}
	std::swap(pointdata[2], pointdata[3]);
}

void FlatRenderDx9::AddFillPicture( std::size_t hashid, int layer )
{
	stPicD3DVertex_Blend objData[4] = {
		{-1.0f,-1.0f,0.0f,255,255,255,255,0,1},
		{1.0f,-1.0f,0.0f,255,255,255,255,1,1},
		{1.0f,1.0f,0.0f,255,255,255,255,1,0},
		{-1.0f,1.0f,0.0f,255,255,255,255,0,0}
	};
	TextureMapIter MapPos = m_TextureMap.find(hashid);
	if (MapPos != m_TextureMap.end())
	{
		m_vPicInfo_Blend.push_back(PicInfo_Blend(objData, MapPos->second, layer));
	}
}

void FlatRenderDx9::AddFillPicture( std::string str, int layer )
{
	AddFillPicture(strhasher(str), layer);
}

void FlatRenderDx9::SetMulPointData(stPicD3DVertex *pointdata, PicObject* renderobject)
{
	Posf tPoints[4];
	PicPos& picpos = renderobject->picpos;
	picpos.SelfPos();
	//以左上的(x,y)為(1,1)作為標準轉換的正常貼圖座標
	tPoints[0].x=(picpos.pos.x-1.0f)/picpos.size.x; tPoints[0].y=(picpos.pos.y-1.0f)/picpos.size.y;
	tPoints[1].x=tPoints[0].x; tPoints[1].y=picpos.pos.y/(float)picpos.size.y;
	tPoints[2].x=picpos.pos.x/(float)picpos.size.x; tPoints[2].y=tPoints[0].y;
	tPoints[3].x=tPoints[2].x; tPoints[3].y=tPoints[1].y;

	switch (renderobject->reverse)
	{
	case HORIZONTAL:
		//水平翻轉,把左右點的 x 交換
		std::swap(tPoints[0].x,tPoints[2].x);
		std::swap(tPoints[1].x,tPoints[3].x);
		break;
	case VERTICAL:
		//垂直翻轉,把上下點的 y 交換
		std::swap(tPoints[0].y,tPoints[1].y);
		std::swap(tPoints[2].y,tPoints[3].y);
		break;
	case DIAGONAL:
		//對角翻轉=水平翻轉+垂直翻轉
		std::swap(tPoints[0].x,tPoints[2].x);
		std::swap(tPoints[1].x,tPoints[3].x);
		std::swap(tPoints[0].y,tPoints[1].y);
		std::swap(tPoints[2].y,tPoints[3].y);
		break;
	}
	//以下旋轉部分跟SetPointData一樣
	Posf picPoints[4];
	float angle=renderobject->angle*MATH_PI/180;
	Posf p1 = renderobject->origin/* + renderobject->points[0]*/;
	for (int i=0; i<4; i++)
	{
		VectorRotate(p1, renderobject->points[i], picPoints[i], angle);
	}
	for(int j=0;j<4;j++){
		pointdata[j].x=picPoints[j].x;
		pointdata[j].y=picPoints[j].y;
		pointdata[j].z=0;
		pointdata[j].tu=tPoints[j].x;
		pointdata[j].tv=tPoints[j].y;
	}
	std::swap(pointdata[2], pointdata[3]);
}

void FlatRenderDx9::SetMulPointData(stPicD3DVertex_Blend *pointdata, PicObject* renderobject)
{
	Posf tPoints[4];
	PicPos& picpos = renderobject->picpos;
	picpos.SelfPos();
	//以左上的(x,y)為(1,1)作為標準轉換的正常貼圖座標
	tPoints[0].x=(picpos.pos.x-1.0f)/picpos.size.x; tPoints[0].y=(picpos.pos.y-1.0f)/picpos.size.y;
	tPoints[1].x=tPoints[0].x; tPoints[1].y=(float)picpos.pos.y/picpos.size.y;
	tPoints[2].x=(float)picpos.pos.x/picpos.size.x; tPoints[2].y=tPoints[0].y;
	tPoints[3].x=tPoints[2].x; tPoints[3].y=tPoints[1].y;
	switch (renderobject->reverse)
	{
	case HORIZONTAL:
		//水平翻轉,把左右點的 x 交換
		std::swap(tPoints[0].x,tPoints[2].x);
		std::swap(tPoints[1].x,tPoints[3].x);
		break;
	case VERTICAL:
		//垂直翻轉,把上下點的 y 交換
		std::swap(tPoints[0].y,tPoints[1].y);
		std::swap(tPoints[2].y,tPoints[3].y);
		break;
	case DIAGONAL:
		//對角翻轉=水平翻轉+垂直翻轉
		std::swap(tPoints[0].x,tPoints[2].x);
		std::swap(tPoints[1].x,tPoints[3].x);
		std::swap(tPoints[0].y,tPoints[1].y);
		std::swap(tPoints[2].y,tPoints[3].y);
		break;
	}
	//以下旋轉部分跟SetPointData一樣
	Posf picPoints[4];
	float angle = renderobject->angle*MATH_PI/180;
	Posf p1 = renderobject->origin/* + renderobject->points[0]*/;
	for (int i=0; i<4; i++)
	{
		VectorRotate(p1, renderobject->points[i], picPoints[i], angle);
	}
	for(int j=0;j<4;j++){
		pointdata[j].x=picPoints[j].x;
		pointdata[j].y=picPoints[j].y;
		pointdata[j].z=0;
		pointdata[j].tu=tPoints[j].x;
		pointdata[j].tv=tPoints[j].y;
	}
	std::swap(pointdata[2], pointdata[3]);
}
//以下為聚光燈實作
void FlatRenderDx9::SetSpotLight(int layer, Posf begPos, float direction, unsigned char begColors[3],
				 short begAlpha, unsigned char endColors[3], short endAlpha, float angle, float lightlen)
{
	Posf newpoint[3];
	TurnTrianglePoint(newpoint, begPos, direction, lightlen, angle);
	stLightPolygon tempsp[3] =
	{
		{newpoint[0].x * m_HandledWidth - 1, -newpoint[0].y * m_HandledHeight + 1, 0.0f, begColors[2], begColors[1], begColors[0], begAlpha},
		{newpoint[1].x * m_HandledWidth - 1, -newpoint[1].y * m_HandledHeight + 1, 0.0f, endColors[2], endColors[1], endColors[0], endAlpha},
		{newpoint[2].x * m_HandledWidth - 1, -newpoint[2].y * m_HandledHeight + 1, 0.0f, endColors[2], endColors[1], endColors[0], endAlpha}
	};
	m_SpotLightInfo.push_back(stSpotLight(tempsp, layer));
}

void FlatRenderDx9::SetSpotLight(int layer, Posf begPos, Posf endPos,unsigned char begColors[3],
				 short begAlpha, unsigned char endColors[3], short endAlpha, float angle)
{
	float lightlen, direction;
	lightlen = sqrt(pow(begPos.x-endPos.x, 2.0f) - pow(begPos.y-endPos.y, 2.0f));
	//用起始點和終點算方向
	//如果起始顛和結束點的 x 差近似於0代表+-90度
	if (begPos.x - endPos.x < 0.00001f)
	{
		if (endPos.y > begPos.y)
			direction = 90.0f;
		else
			direction = -90.0f;
	}
	else
	{
		direction = atan((endPos.y - begPos.y) / (endPos.x - begPos.x)) * 360.0f / MATH_PI;
		//在第二項限
		if (endPos.x - begPos.x < 0.001f && endPos.y - begPos.y > 0.001f)
			direction += 180.0f;
		//在第三項限
		else if(endPos.x - begPos.x < 0.001f && endPos.y - begPos.y < 0.001f)
			direction -= 180.0f;
	}
	SetSpotLight(layer, begPos, direction, begColors, begAlpha, endColors, endAlpha, angle, lightlen);
}

_inline void FlatRenderDx9::RenderSpotLight(stSpotLight* lightInfo)
{
	unsigned char beg_a, end_a;
	unsigned char nbeg_c[3];
	unsigned char nend_c[3];
	if (lightInfo->stPoint[0].color_a >= 0 && lightInfo->stPoint[1].color_a >= 0)
	{
		beg_a = (unsigned char)(lightInfo->stPoint[0].color_a );
		end_a = (unsigned char)(lightInfo->stPoint[1].color_a );
		nbeg_c[0] = lightInfo->stPoint[0].color_r;
		nbeg_c[1] = lightInfo->stPoint[0].color_g;
		nbeg_c[2] = lightInfo->stPoint[0].color_b;
		nend_c[0] = lightInfo->stPoint[1].color_r;
		nend_c[1] = lightInfo->stPoint[1].color_g;
		nend_c[2] = lightInfo->stPoint[1].color_b;
		stLightPolygon g[] = {
			{lightInfo->stPoint[0].x, lightInfo->stPoint[0].y, 0.0f, nbeg_c[2], nbeg_c[1], nbeg_c[0], beg_a},
			{lightInfo->stPoint[1].x, lightInfo->stPoint[1].y, 0.0f, nend_c[2], nend_c[1], nend_c[0], end_a},
			{lightInfo->stPoint[2].x, lightInfo->stPoint[2].y, 0.0f, nend_c[2], nend_c[1], nend_c[0], end_a},
		};	
		m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);	
		m_device->SetRenderState(D3DRS_BLENDFACTOR, D3DCOLOR_ARGB(beg_a, beg_a, beg_a, beg_a));
		m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BLENDFACTOR );
		m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	
		m_device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, g, sizeof(stLightPolygon));
	}
	else
	{
		beg_a = (unsigned char)(256 + lightInfo->stPoint[0].color_a );
		end_a = (unsigned char)(256 + lightInfo->stPoint[1].color_a );
		for (int i = 0; i < 3; i++)
		{
			nbeg_c[i] = beg_a;
			nend_c[i] = end_a;
		}
		stLightPolygon g[] = {
			{lightInfo->stPoint[0].x, lightInfo->stPoint[0].y, 0.0f, nbeg_c[2], nbeg_c[1], nbeg_c[0], beg_a},
			{lightInfo->stPoint[1].x, lightInfo->stPoint[1].y, 0.0f, nend_c[2], nend_c[1], nend_c[0], end_a},
			{lightInfo->stPoint[2].x, lightInfo->stPoint[2].y, 0.0f, nend_c[2], nend_c[1], nend_c[0], end_a},
		};	
		m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
		m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVDESTALPHA);
		m_device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, g, sizeof(stLightPolygon));
	}
}

void FlatRenderDx9::TurnTrianglePoint(Posf tripoint[], Posf begPos, float direction,
				      float lightlen, float angle)
{
	//從起點向direct方向延伸lightlen長度的點
	Posf endpoint;
	angle = angle * MATH_PI / 180.0f;
	direction = direction * MATH_PI / 180.0f;
	tripoint[0].x = begPos.x;
	tripoint[0].y = begPos.y;
	endpoint.x = begPos.x + lightlen * cos(direction);
	endpoint.y = begPos.y - lightlen * sin(direction);
	//endpoint以源點為圓心分別轉+-夾角的一半就是另外2點
	VectorRotate(tripoint[0], endpoint, tripoint[1], -angle/2);
	VectorRotate(tripoint[0], endpoint, tripoint[2], angle/2);
}

void FlatRenderDx9::SaveNow( int w, int h, int picFormat, TCHAR* path )
{
	LPDIRECT3DSURFACE9 surface = NULL;
	m_device->CreateOffscreenPlainSurface((unsigned int)w, (unsigned int)h, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &surface, NULL);
	m_device->GetBackBuffer(0,0, D3DBACKBUFFER_TYPE_MONO, &surface);
	switch (picFormat)
	{
	case SAVE_FORMAT_BMP:
		D3DXSaveSurfaceToFile(path, D3DXIFF_BMP, surface, NULL, NULL);
		break;
	case SAVE_FORMAT_TGA:
		D3DXSaveSurfaceToFile(path, D3DXIFF_TGA, surface, NULL, NULL);
		break;
	case SAVE_FORMAT_PNG:
		D3DXSaveSurfaceToFile(path, D3DXIFF_PNG, surface, NULL, NULL);
		break;
	case SAVE_FORMAT_JPG:
		D3DXSaveSurfaceToFile(path, D3DXIFF_JPG, surface, NULL, NULL);
		break;
	case SAVE_FORMAT_DDS:
		D3DXSaveSurfaceToFile(path, D3DXIFF_DDS, surface, NULL, NULL);
		break;
	}
	surface->Release();
}

void RectToPoint( const Rectf& rect, Posf* posv )
{
	posv[0] = Posf(rect.left, rect.top);
	posv[1] = Posf(rect.left, rect.bottom);
	posv[2] = Posf(rect.right, rect.top);
	posv[3] = Posf(rect.right, rect.bottom);
}

void PointToRect( Rectf& rect, const Posf* posv )
{
	float width = fabs(posv[0].x-posv[2].x);
	float height = fabs(posv[0].y-posv[1].y);
	float mx = (posv[0].x+posv[2].x)/2;
	float my = (posv[0].y+posv[1].y)/2;
	rect = Rectf(mx-width/2, my-height/2, mx+width/2, my+height/2);
}

void FlatRenderDx9::AddPicObject(PicObject* renderobject )
{
	MulObjectSet(renderobject);
}

void FlatRenderDx9::SetPicObject( PicObject* renderobject )
{
	if (renderobject->m_flag[PicObject::HAS_BLEND])
	{
		int index = 0, len = m_vPicInfo_Blend.size();
		for (;index < len;index++)
		{
			if (m_vPicInfo_Blend[index].orgin_data == renderobject)
			{
				break;
			}
		}
		if (index == len) return;
		stPicD3DVertex_Blend objData[4];
		SetMulPointData(objData, renderobject);
		for (int i=0; i<4; i++)
		{
			objData[i].color_a = renderobject->blend_colors[0];
			objData[i].color_r = renderobject->blend_colors[1];
			objData[i].color_g = renderobject->blend_colors[2];
			objData[i].color_b = renderobject->blend_colors[3];
			objData[i].x=objData[i].x*m_HandledWidth-1+m_fix_x;
			objData[i].y=-objData[i].y*m_HandledHeight+1+m_fix_y;
		}
		m_vPicInfo_Blend[index] = PicInfo_Blend(objData, renderobject);
		return;
	}
	else
	{
		int index = 0, len = m_vPicInfo.size();
		for (;index < len;index++)
		{
			if (m_vPicInfo[index].orgin_data == renderobject)
			{
				break;
			}
		}
		if (index == len) return;
		stPicD3DVertex objData[4];
		SetMulPointData(objData, renderobject);
		for (int i=0; i<4; i++)
		{
			objData[i].x = objData[i].x * m_HandledWidth-1+m_fix_x;
			objData[i].y = -objData[i].y * m_HandledHeight+1+m_fix_y;
		}
		m_vPicInfo[index] = PicInfo(objData, renderobject);
		return;
	}
}

PicObject* FlatRenderDx9::DelPicObject( std::size_t id )
{
	int index = 0, len = m_vPicInfo.size();
	for (;index < len;index++)
	{
		if (m_vPicInfo[index].orgin_data->id == id)
		{
			PicObject* result = m_vPicInfo_Blend[index].orgin_data;
			m_vPicInfo.erase(m_vPicInfo.begin()+index);
			return result;
		}
	}
	len = m_vPicInfo_Blend.size();
	index = 0;
	for (;index < len;index++)
	{
		if (m_vPicInfo_Blend[index].orgin_data->id == id)
		{
			PicObject* result = m_vPicInfo_Blend[index].orgin_data;
			m_vPicInfo_Blend.erase(m_vPicInfo_Blend.begin()+index);
			return result;
		}
	}
	return NULL;
}

PicObject* FlatRenderDx9::GetPicObject( std::size_t id )
{
	int index = 0, len = m_vPicInfo.size();
	for (;index < len;index++)
	{
		if (m_vPicInfo[index].orgin_data->id == id)
		{
			PicObject* result = m_vPicInfo_Blend[index].orgin_data;
			return result;
		}
	}
	len = m_vPicInfo_Blend.size();
	index = 0;
	for (;index < len;index++)
	{
		if (m_vPicInfo_Blend[index].orgin_data->id == id)
		{
			PicObject* result = m_vPicInfo_Blend[index].orgin_data;
			return result;
		}
	}
	return NULL;
}


void FlatRenderDx9::DelPicObject( PicObject* renderobject )
{
	int index = 0, len = m_vPicInfo.size();
	for (;index < len;index++)
	{
		if (m_vPicInfo[index].orgin_data == renderobject)
		{
			m_vPicInfo.erase(m_vPicInfo.begin()+index);
			return;
		}
	}
	len = m_vPicInfo_Blend.size();
	for (index = 0;index < len;index++)
	{
		if (m_vPicInfo_Blend[index].orgin_data == renderobject)
		{
			m_vPicInfo_Blend.erase(m_vPicInfo_Blend.begin()+index);
			return;
		}
	}
}

void FlatRenderDx9::HidePicObject( PicObject* renderobject )
{
	int index = 0, len = m_vPicInfo.size();
	for (;index < len;index++)
	{
		if (m_vPicInfo[index].orgin_data == renderobject)
		{
			m_vPicInfo[index].layer = -abs(m_vPicInfo[index].layer);
			return;
		}
	}
	len = m_vPicInfo_Blend.size();
	index = 0;
	for (;index < len;index++)
	{
		if (m_vPicInfo_Blend[index].orgin_data == renderobject)
		{
			m_vPicInfo_Blend[index].layer = -abs(m_vPicInfo_Blend[index].layer);
			return;
		}
	}
}

void FlatRenderDx9::ShowPicObject( PicObject* renderobject )
{
	int index = 0, len = m_vPicInfo.size();
	for (;index < len;index++)
	{
		if (m_vPicInfo[index].orgin_data == renderobject)
		{
			m_vPicInfo[index].layer = abs(m_vPicInfo[index].layer);
			return;
		}
	}
	len = m_vPicInfo_Blend.size();
	index = 0;
	for (;index < len;index++)
	{
		if (m_vPicInfo_Blend[index].orgin_data == renderobject)
		{
			m_vPicInfo_Blend[index].layer = abs(m_vPicInfo_Blend[index].layer);
			return;
		}
	}
}

void FlatRenderDx9::HideAll()
{
	int index = 0, len = m_vPicInfo.size();
	for (;index < len;index++)
	{
		m_vPicInfo[index].layer = -abs(m_vPicInfo[index].layer);
	}
	len = m_vPicInfo_Blend.size();
	index = 0;
	for (;index < len;index++)
	{
		m_vPicInfo_Blend[index].layer = -abs(m_vPicInfo_Blend[index].layer);
	}
}

void FlatRenderDx9::ShowAll()
{
	int index = 0, len = m_vPicInfo.size();
	for (;index < len;index++)
	{
		m_vPicInfo[index].layer = abs(m_vPicInfo[index].layer);
	}
	len = m_vPicInfo_Blend.size();
	index = 0;
	for (;index < len;index++)
	{
		m_vPicInfo_Blend[index].layer = abs(m_vPicInfo_Blend[index].layer);
	}
}

void FlatRenderDx9::SetMonopolize( bool flag )
{
	m_Monopolize = flag;
}

bool FlatRenderDx9::GetMonopolize()
{
	return m_Monopolize;
}

void PicObject::HReverse()
{
	switch (reverse)
	{
	case NOREVERSE:
		reverse = HORIZONTAL;
		break;
	case HORIZONTAL:
		reverse = DIAGONAL;
		break;
	case VERTICAL:
		reverse = DIAGONAL;
		break;
	case DIAGONAL:
		reverse = VERTICAL;
		break;
	}
}

void PicObject::VReverse()
{
	switch (reverse)
	{
	case NOREVERSE:
		reverse = VERTICAL;
		break;
	case HORIZONTAL:
		reverse = DIAGONAL;
		break;
	case VERTICAL:
		reverse = NOREVERSE;
		break;
	case DIAGONAL:
		reverse = HORIZONTAL;
		break;
	}
}

bool PicObject::SetTexture( FlatRenderDx9* render, std::string texture_name )
{
	FlatRenderDx9::TextureMapIter MapPos = render->m_TextureMap.find(strhasher(texture_name));
	if (MapPos != render->m_TextureMap.end())
	{
		texture = MapPos->second;
		return true;
	}
	return false;
}

bool PicObject::SetTexture( FlatRenderDx9* render, std::size_t texture_id )
{
	FlatRenderDx9::TextureMapIter  MapPos = render->m_TextureMap.find(texture_id);
	if (MapPos != render->m_TextureMap.end())
	{
		texture = MapPos->second;
		return true;
	}
	return false;
}

void PicObject::SetFillScreen(FlatRenderDx9* render)
{
	RectToPoint(render->m_rect, points);
}

void PicObject::SetRect( const Rectf& _rect )
{
	RectToPoint(_rect, points);
	origin.x = (_rect.left + _rect.right)*0.5;
	origin.y = (_rect.top + _rect.bottom)*0.5;
}

PicObject::PicObject(std::size_t _id, int _layer, const Posf* ps ) :
id(_id),
layer(_layer),
reverse(NOREVERSE),
angle(0),
origin(Posf(0,0)),
picpos(PicPos())
{
	memcpy(points, ps, sizeof(points));
	m_flag[HAS_BLEND] = false;
}

PicObject::PicObject(std::size_t _id, int _layer, const Rectf& _rect ) :
id(_id),
layer(_layer),
reverse(NOREVERSE),
angle(0),
origin(Posf((_rect.left+_rect.right)/2,(_rect.top+_rect.bottom)/2)),
picpos(PicPos())
{
	RectToPoint(_rect, points);
	m_flag[HAS_BLEND] = false;
}

void PicObject::SetBlend( unsigned char a,unsigned char r, unsigned char g, unsigned char b )
{
	blend_colors[0] = a;
	blend_colors[1] = r;
	blend_colors[2] = g;
	blend_colors[3] = b;
}

void PicObject::SetAlpha( unsigned char a )
{
	blend_colors[0] = a;
}

void PicObject::Move( float mx, float my )
{
	for (int i=0;i<4;i++)
	{
		points[i].x += mx;
		points[i].y += my;
	}
	origin.x += mx;
	origin.y += my;
}

void PicObject::MoveTo( float mx, float my )
{
	Rectf rect;
	PointToRect(rect, points);
	rect.Move(mx - origin.x, my - origin.y);
	SetRect(rect);
}

stSpotLight::stSpotLight( stLightPolygon* stlight, int _layer ) :layer(_layer)
{
	memcpy(&stPoint, stlight, sizeof(stLightPolygon) * 3);
}

PicInfo_Blend::PicInfo_Blend( const stPicD3DVertex_Blend* data, LPDIRECT3DTEXTURE9 tex, int _layer ) :lpTexture(tex),
layer(_layer),
orgin_data(NULL)
{
	memcpy(&stPoint, data, sizeof(stPicD3DVertex_Blend)*4);
}

PicInfo_Blend::PicInfo_Blend( const stPicD3DVertex_Blend* data, PicObject* _orgin_data ) :lpTexture(_orgin_data->texture),
layer(_orgin_data->layer),
orgin_data(_orgin_data)
{
	memcpy(&stPoint, data, sizeof(stPicD3DVertex_Blend)*4);
}

PicInfo::PicInfo( const stPicD3DVertex* data, PicObject* _orgin_data ) :lpTexture(_orgin_data->texture),
layer(_orgin_data->layer),
orgin_data(_orgin_data)
{
	memcpy(&stPoint, data, sizeof(stPicD3DVertex)*4);
}

void PicPos::SelfPos()
{
	if (pos.x>size.x) pos.x=size.x;
	if (pos.y>size.y) pos.y=size.y;
	if (pos.x<1) pos.x = 1;
	if (pos.y<1) pos.y = 1;
}
