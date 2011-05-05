
#ifndef EditBitMap_H
#define EditBitMap_H

#include <vector>

struct c32
{
	unsigned char B,G,R,A;
	bool operator==(c32 &rhs)
	{
		return (B == rhs.B && G == rhs.G && R == rhs.R && A == rhs.A);
	}
	bool operator!=(c32 &rhs)
	{
		return (B != rhs.B || G != rhs.G || R != rhs.R || A != rhs.A);
	}
	bool operator==(int rhs)
	{
		return (B == (unsigned char)rhs && G == (unsigned char)rhs && R == (unsigned char)rhs && A == (unsigned char)rhs);
	}
	bool operator!=(int rhs)
	{
		return (B != (unsigned char)rhs || G != (unsigned char)rhs || R != (unsigned char)rhs || A != (unsigned char)rhs);
	}
};

class EditBitMap
{
public:
	EditBitMap();
	~EditBitMap();
	void NewMap(int w, int h);
	c32* GetPixel(int x, int y);
	void SetPixel(int x, int y, c32 color);
	c32** GetMartrix(int x, int y, int radius);
	void ReleaseMartrix();
	bool checkIndex(int w, int h,int x, int y)
	{ return (x>=0 && y>=0 && x<w && y<h); };
	int iWidth,iHeight;
	long lImageSize;
	c32 *pImage;
private:
	struct c32Matrix
	{
		c32** lp;
		int size;
	};
	std::vector<c32Matrix> m_MatrixMange;
	std::vector<c32Matrix>::iterator m_MatrixMangePos;
};

#endif //EditBitMap_H
