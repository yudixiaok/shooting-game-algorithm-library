/*! @brief 與directx9溝通的繪圖底層
主要負責2D繪圖
*/

#ifndef FlatRenderDx9_Im5b0fc51m122c45fb3b7mm7e5f_H
#define FlatRenderDx9_Im5b0fc51m122c45fb3b7mm7e5f_H
#include <d3d9.h>
#include <d3dx9tex.h>
#include <functional>
#include <vector>
#include <bitset>
#include <map>
#include <algorithm>
#include <boost/functional/hash.hpp>
#include <cstring>
#include <string>
#include <boost/thread/detail/singleton.hpp>
#include "EditBitMap.h"
#include "TShape.hpp"

typedef boost::hash<std::wstring> wstrhash;
static wstrhash wstrhasher;
typedef boost::hash<std::string> strhash;
static strhash strhasher;

const int D3DFVF_VERTEX_BLEND = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
const int D3DFVF_VERTEX = (D3DFVF_XYZ | D3DFVF_TEX1);
enum Reverse {NOREVERSE, HORIZONTAL, VERTICAL, DIAGONAL};
//@file 正常版頂點結構
const float MATH_PI = 3.141592653f;
enum SAVE_FORMAT
{
	SAVE_FORMAT_BMP,SAVE_FORMAT_JPG,SAVE_FORMAT_PNG,SAVE_FORMAT_TGA,SAVE_FORMAT_DDS
};
void RectToPoint(const Rectf& rect, Posf* posv);
void PointToRect(Rectf& rect, const Posf* posv);

class FlatRenderDx9;
struct PicPos
{
	PicPos():pos(Posi(1,1)),size(Posi(1,1)){}
	Posi pos;
	Posi size;
	//為了避免設定的x,y超過最大值以照成不該有的問題,所以超過自動等於最大值
	void SelfPos();
};
struct PicObject
{
	enum {HAS_BLEND};
	std::bitset<8> m_flag;
	int layer;                                 ///< 第幾層圖層
	Posf points[4];                            ///< 要畫的四個點
	int reverse;                               ///< 翻轉
	float angle;                               ///< 角度
	Posf origin;                               ///< 旋轉中心點
	unsigned char blend_colors[4];             ///< 要混色的顏色
	PicPos picpos;                             ///< 要畫的圖片位置
	LPDIRECT3DTEXTURE9 texture;                ///< 材質
	std::size_t id;                            ///< 從map尋找

	PicObject(std::size_t id, int _layer, const Rectf& _rect);
	PicObject(std::size_t id, int _layer, const Posf* ps);
	void SetRect(const Rectf& _rect);
	void SetBlend(unsigned char a,unsigned char r, unsigned char g, unsigned char b);
	void SetAlpha(unsigned char a);
	void Move(float mx, float my);
	void MoveTo(float mx, float my);
	void VReverse();
	void HReverse();
	bool SetTexture(FlatRenderDx9* render, std::string texture_name);
	bool SetTexture(FlatRenderDx9* render, std::size_t texture_id);
	void SetFillScreen(FlatRenderDx9* render);
};
/**
 * @brief 
 *
 * 正常版頂點結構
 */
struct stPicD3DVertex
{
	float x, y, z;
	float tu, tv;
};
/**
 * @brief 
 *
 * 正常版圖片資訊
 */
struct PicInfo 
{
	PicInfo(const stPicD3DVertex* data, PicObject* _orgin_data);
	stPicD3DVertex stPoint[4];
	LPDIRECT3DTEXTURE9 lpTexture;
	int layer;
	PicObject* orgin_data;
	bool operator<(const PicInfo& rl) const
	{
		return layer < rl.layer;
	}
};
/**
 * @brief 
 *
 * 混色版頂點結構
 */
struct stPicD3DVertex_Blend
{
	float x, y, z;
	unsigned char color_b, color_g, color_r, color_a;
	float tu, tv;
};	
/**
 * @brief 
 *
 * 混色版圖片資訊
 */
struct PicInfo_Blend 
{
	PicInfo_Blend(const stPicD3DVertex_Blend* data, PicObject* _orgin_data);
	PicInfo_Blend(const stPicD3DVertex_Blend* data, LPDIRECT3DTEXTURE9 tex, int _layer);
	stPicD3DVertex_Blend stPoint[4];
	LPDIRECT3DTEXTURE9 lpTexture;
	int layer;
	PicObject* orgin_data;
	bool operator<(const PicInfo_Blend& rl) const
	{
		return layer < rl.layer;
	}
};
/**
 * @brief 
 *
 * 光點結構
 */
struct stLightPolygon
{
	float x, y, z;
	unsigned char color_b, color_g, color_r;
	short color_a;
};
struct stSpotLight
{
	stSpotLight(){}
	stSpotLight(stLightPolygon* stlight, int _layer);
	stLightPolygon stPoint[3];
	int layer;
	bool operator<(const stSpotLight& rl) const
	{
		return layer < rl.layer;
	}
};

class FlatRenderDx9
{
public:
	FlatRenderDx9():m_Monopolize(true) {};
	FlatRenderDx9(LPDIRECT3DDEVICE9 device);
	~FlatRenderDx9();
	void SetRenderRect(Rectf rect);
	bool LoadPicture(const wchar_t* filepath, std::string str);
	bool LoadPicture(const wchar_t* filepath, std::size_t hashid);
	bool LoadBmpMem(unsigned char *BmpPic, int w, int h, int per_bytes, std::size_t hashid);
	bool LoadTgaMem(unsigned char *TgaPic, int w, int h, int per_bytes, std::size_t hashid);
	bool LoadMapMem(EditBitMap &bitmap, std::size_t hashid);
	void SaveNow(int w, int h, int picFormat, TCHAR* path);
	/**
	 * @brief 
     *
     * 負責每幀的渲染，必需要在BeginRender跟EndRender之間
     */
	void RenderFrame();
	void SetMonopolize(bool flag);
	bool GetMonopolize();
	
	void SetDevice(LPDIRECT3DDEVICE9 device);
	/**
	 * @brief 
     *
     * 清除圖片設定
     */
	void ClearPicObject();
	void ClearLoadPic(); 
	// AddFillPicture, SetMulPicObject都要在BeginRender之前
	// 然後用ClearSet清空
	void AddFillPicture(std::size_t hashid, int layer);
	void AddFillPicture(std::string str, int layer);
	//id是載入時材質的id，angle是順時針旋轉的角度，reverse是要翻轉的種類，origin是要旋轉的原點，
	//blend_colors是要混的顏色
	void AddPicObject(PicObject* renderobject);
	void SetPicObject(PicObject* renderobject); // 必需保證HAS_BLEND的FLAG沒有改
	void HidePicObject(PicObject* renderobject);
	void ShowPicObject(PicObject* renderobject);
	void HideAll();
	void ShowAll();
	// 下面兩個函數功能一樣
	PicObject* DelPicObject(std::size_t id);

	PicObject* GetPicObject(std::size_t id);
	void DelPicObject(PicObject* renderobject);
	//輸入為一種顏色，感覺就像加上濾光片
	void AddColorShade(unsigned char *blend_colors);
	//輸入為-256~255，效果是純白光
	void AddLightShade(short level);
	//有輸入方向和長度版
	void SetSpotLight(int layer, Posf begPos, float direction,
		unsigned char begColors[3], short begAlpha, unsigned char endColors[3], 
		short endAlpha,	float angle, float lightlen);
	//輸入結束點版
	void SetSpotLight(int layer, Posf begPos, Posf endPos,
		unsigned char begColors[3], short begAlpha, unsigned char endColors[3], 
		short endAlpha,	float angle);
private:
	bool	m_Monopolize;
	float	m_Width,
		m_Height,
		m_HandledWidth,	
		m_HandledHeight,
		m_fix_x, 
		m_fix_y;
	Rectf	m_rect;
	HWND	m_hWnd;
	LPDIRECT3DDEVICE9 m_device;
	LPDIRECT3DVERTEXBUFFER9 m_lpVertexBuffer;
	typedef std::map<std::size_t, LPDIRECT3DTEXTURE9 > TextureMap;
	typedef TextureMap::iterator TextureMapIter;
	TextureMap m_TextureMap;	
	std::vector<PicInfo> m_vPicInfo;
	std::vector<PicInfo_Blend> m_vPicInfo_Blend;
	std::vector<PicInfo>::iterator m_vPicInfoPos;
	std::vector<PicInfo_Blend>::iterator m_vPicInfoPos_Blend;
	std::vector<PicObject> m_hideObj;
	std::vector<stSpotLight> m_SpotLightInfo;
	std::vector<stSpotLight>::iterator m_SpotLightInfoPos;

	void VectorRotate(Posf point1, Posf point2, Posf &newpoint, float angle );
	void SetPointData(stPicD3DVertex_Blend *pointdata, PicObject* renderobject);
	void SetPointData(stPicD3DVertex *pointdata, PicObject* renderobject);
	void ObjectSet(PicObject* renderobject);
	void MulObjectSet(PicObject* renderobject);
	void SetMulPointData(stPicD3DVertex *pointdata, PicObject* renderobject);
	void SetMulPointData(stPicD3DVertex_Blend *pointdata, PicObject* renderobject);
	void TurnTrianglePoint(Posf tripoint[3], Posf begPos, float direction,
		float lightlen, float angle);
	//把某個聚光燈render
	void RenderSpotLight(stSpotLight* linofo);
	friend PicObject;

public: // inline function
	inline void RenderBegin()
	{
		m_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0 );
		m_device->BeginScene();	
	}
	inline void RenderEnd()
	{
		m_device->EndScene();
		m_device->Present( NULL, NULL, NULL, NULL );
	}
};

typedef boost::detail::thread::singleton<FlatRenderDx9> FlatRenderDx9S;
#endif // FlatRenderDx9_Im5b0fc51m122c45fb3b7mm7e5f_H
