#include <cstdlib>
#include <iostream>
#include "algo/nway.h"
#include "algo/RandomWay.h"
#include "DX9/GrphaicDirectX9.h"
#include "DX9/FlatRenderDx9.h"
#include "Win32Window.h"
#include "ball/straight.h"
#include "comnhdr.h"
#include "Timer/Timer.hpp"
#include "algo/BallManager.h"

#define GC_NOT_DLL
#include <gc_cpp.h>

bool g_exit = false;

static LRESULT CALLBACK Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		g_exit = true;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

int main()
{
	timer::rdtsc mytimer;
	// init
	Win32Window w32;
	w32.ToCreateWindow(100, 100, 800, 800, L"SGAL", Proc);
	GrphaicDirectX9 gdx;
	gdx.InitDevice(w32.GetHandle());
	FlatRenderDx9 fdx;
	fdx.SetDevice(gdx.GetGraphicsDevice());
	fdx.SetRenderRect(Rectf(0.0f, 0.0f, 800-20, 800-40));
	w32.ToShow();
	w32.ToMoveCenter();
	// create ball behavior
	Behavior* straight = new(GC) Straight;
	// create track
	RandomWay nway(10, Ogre::Vector3(400, 400, 0), Ogre::Vector3(0, 5, 0));
	nway.SetRadiationAngle(90);
	nway.SetBehavior(straight);
	BallManager bm(4); // 多執行緒更新管理器
	bm.AddTrajectory(&nway);
	BallVector& balls = nway.GetBallVector();
	printf("%d", balls[0].GetClassSize());
	// load pic
	bool ok = fdx.LoadPicture(L"check0.png", wstrhasher(L"check0.png"));
	// init picobject to show
	std::vector<PicObject> pics;
	pics.resize(balls.size(), PicObject(wstrhasher(L"check0.png"), 1, Rectf()));
	for (int i=0;i < (int)balls.size();i++)
	{
		// set picture to correct posiction
		pics[i].SetRect(Rectf(balls[i].mPosition.x, balls[i].mPosition.y, balls[i].mPosition.x+10, balls[i].mPosition.y+10));
		pics[i].SetTexture(&fdx, wstrhasher(L"check0.png"));
		// add object to show
		fdx.AddPicObject(&pics[i]);
	}
	MSG msg = w32.HandlePeekMessage();
	for (;!g_exit;msg = w32.HandlePeekMessage())
	{
		if (VK_ESCAPE == msg.wParam )
			break;
		fdx.RenderBegin();
		BenchTicks_t bt = BenchTicksGetCurrent();   
		// update time
		bm.Update(0.3f);	
		for (int i=0;i < (int)balls.size();i++)
		{
			//balls[i].Update(0.3f);
		}
		bt = BenchTicksGetCurrent() - bt;
		printf("compute time = %s\n", BenchTicksToString(bt, true));
		bt = BenchTicksGetCurrent();
		for (int i=0;i < (int)balls.size();i++)
		{
			// update time
			//balls[i].Update(0.3f);
			// update position
			pics[i].SetRect(Rectf(balls[i].mPosition.x, balls[i].mPosition.y, balls[i].mPosition.x+10, balls[i].mPosition.y+10));
			pics[i].angle = balls[i].mDirection.angleBetween(Ogre::Vector3::UNIT_X).valueDegrees();
			fdx.SetPicObject(&pics[i]);
		}
		fdx.RenderFrame();
		bt = BenchTicksGetCurrent() - bt;
		printf("render time = %s\n", BenchTicksToString(bt, true));
	}
}
