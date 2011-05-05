#include <iostream>
#include "algo/nway.h"
#include "DX9/GrphaicDirectX9.h"
#include "DX9/FlatRenderDx9.h"
#include "Win32Window.h"
#include "ball/straight.h"

int main()
{
	// init
	Win32Window w32;
	w32.ToCreateWindow(100, 100, 800, 800, L"SGAL");
	GrphaicDirectX9 gdx;
	gdx.InitDevice(w32.GetHandle());
	FlatRenderDx9 fdx;
	fdx.SetDevice(gdx.GetGraphicsDevice());
	fdx.SetRenderRect(Rectf(0.0f, 0.0f, 800-20, 800-40));
	w32.ToShow();
	w32.ToMoveCenter();
	// create ball behavior
	Behavior_Sptr straight = Behavior_Sptr(new Straight);
	// create track
	Nway nway(11, Ogre::Vector3(400, 400, 0), Ogre::Vector3(0, 1, 0));
	nway.SetRadiationAngle(90);
	nway.SetBehavior(straight);
	BallVector balls = nway.GenerateBallVector();
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
	
	for (;;)
	{
		w32.HandlePeekMessage();
		fdx.RenderBegin();
		for (int i=0;i < (int)balls.size();i++)
		{
			// update time
			balls[i].Update(0.3f);
			// update position
			pics[i].SetRect(Rectf(balls[i].mPosition.x, balls[i].mPosition.y, balls[i].mPosition.x+10, balls[i].mPosition.y+10));
			fdx.SetPicObject(&pics[i]);
		}
		fdx.RenderFrame();
	}
	
}
