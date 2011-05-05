
#include "EditBitMap.h"
#include <stdlib.h>

EditBitMap::~EditBitMap()
{
	if (pImage)
		delete [] pImage;
}
void EditBitMap::NewMap( int w,int h )
{
	if (pImage)
		delete [] pImage;	
	iWidth = w;
	iHeight = h;
	lImageSize = w * h * 4;
	pImage = new c32[w*h];
}

c32* EditBitMap::GetPixel( int x, int y )
{
	int loc = (y * iWidth + x);
	return pImage + loc;
}

void EditBitMap::SetPixel( int x, int y, c32 color )
{
	int loc = (y * iWidth + x);
	c32* pos = pImage + loc;
	*pos = color;
}

EditBitMap::EditBitMap()
{
	pImage = NULL;
}

c32** EditBitMap::GetMartrix(int x, int y, int radius )
{
	int size = radius*2+1;
	c32 **matrix = new c32*[size];
	for (int i=0; i < size;i++)
	{
		matrix[i] = new c32[size];
		memset(matrix[i],0,sizeof(matrix[i]));
	}
	if (checkIndex(iWidth,iHeight,x,y))
	{
		if (x < radius || y < radius || iWidth-x < radius || iHeight-y < radius)
		{
			for (int mY = 0;mY < size;mY++)
			{
				size = radius*2;
				for (int mX = 0;mX < size;mX++)
				{
					matrix[mY][mX] = *GetPixel(x-radius+mX,y-radius+mY);
				}
			}
		}
		else
		{
			int boundX = radius;
			int boundY = radius;
			if (iWidth-x < radius)
				boundX -= (iWidth-x);
			else if (x < radius)
				boundX -= x;
			else
			{
				boundX=-1;
			}
			if (y < radius)
				boundY -= y;
			else if (iHeight-y < radius)
				boundY -= (iHeight-y);
			else
			{
				boundY=-1;
			}
			int size = radius*2;
			for (int mY = 0;mY < size;mY++)
			{
				for (int mX = 0;mX < size;mX++)
				{
					int testX = x-radius+mX;
					int testY = y-radius+mY;
					if (checkIndex(iWidth,iHeight,testX,testY))
						matrix[mY][mX] = *GetPixel(x-radius+mX,y-radius+mY);
				}
			}
			//水平複製
			if (boundX != -1)
			{
				for (int mY = 0;mY < size;mY++)
				{
					for (int mX = 0;mX < size;mX++)
					{
						if (matrix[mY][mX] == 0)
						{
							if (mX > boundX)
							{
								if (matrix[mY][2*boundX-mX+1] != 0)
								{
									matrix[mY][mX] = matrix[mY][2*boundX-mX+1];
								}
							}
							else
							{
								if (matrix[mY][2*boundX-mX-1] != 0)
								{
									matrix[mY][mX] = matrix[mY][2*boundX-mX-1];
								}
							}
						}
					}
				}
			}
			//垂直複製
			if (boundY != -1)
			{
				for (int mY = 0;mY < size;mY++)
				{
					for (int mX = 0;mX < size;mX++)
					{
						if (matrix[mY][mX] == 0)
						{
							if (mX > boundX)
							{
								if (matrix[mY][2*boundX-mX+1] != 0)
								{
									matrix[mY][mX] = matrix[mY][2*boundX-mX+1];
								}
							}
							else
							{
								if (matrix[mY][2*boundX-mX-1] != 0)
								{
									matrix[mY][mX] = matrix[mY][2*boundX-mX-1];
								}
							}
						}
					}
				}
			}
		}
	}
	else
		return NULL;
	c32Matrix data = {matrix, radius * 2 + 1};
	m_MatrixMange.push_back(data);
	return matrix;
}

void EditBitMap::ReleaseMartrix()
{
	for (m_MatrixMangePos = m_MatrixMange.begin();m_MatrixMangePos != m_MatrixMange.end();m_MatrixMangePos++)
	{
		for (int i=0; i < m_MatrixMangePos->size;i++)
			delete[] *(m_MatrixMangePos->lp);
		delete[] m_MatrixMangePos->lp;
	}
}
