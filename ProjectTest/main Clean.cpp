/* main.cpp
Main entry point for the Card application*/

/* Before we can start to program the main application we must 
first declare all of the other classes that we wish to directly 
make instances and call the functions of.*/
#include "cD3DManager.h"
#include "cD3DXSpriteMgr.h"
#include "cExplosion.h"
#include "cPlatform.h"
#include "cKnight.h"
#include "cBat.h"
#include "cD3DXFont.h"
#include "cXAudio.h"
#include <strsafe.h>

/* We must also declare that we are using the namespace std 
so that we can use it’s simple list of functions.*/
using namespace std;

/* We must create two types of handles for windows the first
is basically a variable that windows used to identify the 
application and the second is at its simplest an integer that 
identifies the window.*/
HINSTANCE hInst; // global handle to hold the application instance
HWND wndHandle; // global variable to hold the window handle

/* Directx is not the easiest tool to use, so we then make instances
of cD3Manager and cD3DXSpriteMgr to use their functions as an 
interface to use Directx indirectly.*/
static cD3DManager* d3dMgr = cD3DManager::getInstance();
static cD3DXSpriteMgr* d3dxSRMgr = cD3DXSpriteMgr::getInstance();

//Vector2’s are the best way to store the change in position of the sprites.
D3DXVECTOR2 PlatformTrans;
D3DXVECTOR2 BatTrans;
D3DXVECTOR2 knightTrans;

//This boolein is used so that once a player has jumped they cannot jump again.
bool falling;

/* This rectangle is used to hold the size of the window for use for 
calculating the position of sprites relative to the screen.*/
RECT clientBounds;

//The float dt is used for slowing down animations to match certain frame rates.
float dt;
//To create a set of platforms and bats we create a list with an iterator to identify individual objects.
vector<cPlatform*> aPlatform;
vector<cPlatform*>::iterator iter;
vector<cBat*> aBat;
vector<cBat*>::iterator iterBat;

TCHAR szTempOutput[30];
bool gHit = false;
D3DXVECTOR3 expPos;
/*Lists are used to hold the explosions and the knight this is 
because there is no need identify explosions or the knight.*/
list<cExplosion*> gExplode;
list<cKnight*> gKnight; 
//An instance of cXAudio is needed to play sound files.
cXAudio gExplodeSound;

//This is used to hold the location of the file that is used as the background the location.
LPCSTR backgroundName="Images\\eulogyPlus.png";
LPDIRECT3DSURFACE9 aSurface;				// the Direct3D surface
//These booleans are used for scene management.
bool Menu=true;
bool finish=false;
bool start=false;
int Health=30;
char gScore[50];
char gMenu[50];
int scor=0;

/*
==================================================================
* LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
* LPARAM lParam)
* The window procedure
==================================================================
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Check any available messages from the queue
	//This part of the code is activated by user inputs.
	switch (message)
	{
		//Used when a key is pressed
		case WM_KEYDOWN:
			{
				//When the level is active
				if(Menu==false){
					//if the escape key is pressed
					if (wParam ==VK_ESCAPE)
					{
						//Deletes all the remaining bats.
						iterBat = aBat.begin();
						while (iterBat != aBat.end())
						{
							iterBat = aBat.erase(iterBat);
						}
						iter = aPlatform.begin();
						//Deletes all of the platforms.
						while (iter != aPlatform.end())
						{
							iter = aPlatform.erase(iter);
						}
						//Deletes the knight.
						list<cKnight*>::iterator bite = gKnight.begin();
						bite = gKnight.erase(bite);
						//adds a button and changes the background.
						D3DXVECTOR3 PlatformPosing = D3DXVECTOR3(clientBounds.right/2-60,(float)clientBounds.bottom/2-25,0);
						aPlatform.push_back(new cPlatform(PlatformPosing,d3dMgr->getTheD3DDevice(),"Images\\ButtonInactive.png"));
						backgroundName="Images\\Background.png";
						aSurface = d3dMgr->getD3DSurfaceFromFile(backgroundName);
						Menu=true;
						finish=true;
						return 0;
					}
					// if the left key is pressed
					if (wParam == VK_LEFT)
					{
						//Gives all of the platforms a velocity to the right.
						for(iter = aPlatform.begin(); iter != aPlatform.end(); ++iter)
						{
							if ((*iter)->getTranslation().x<10.0f)
							{
								(*iter)->setTranslation(D3DXVECTOR2(20.0f ,0.0f)); 
							}
						}
						//Gives all the remaining bats a velocity to the right.
						for(iterBat = aBat.begin(); iterBat != aBat.end(); ++iterBat)
						{
							if ((*iterBat)->getTranslation().x<10.0f)
							{
								(*iterBat)->setTranslation(D3DXVECTOR2(20.0f ,0.0f)); 
							}
						}
					}
					// if the right key is pressed
					if (wParam == VK_RIGHT)
					{
						//Gives all of the platforms a velocity to the left.
						for(iter = aPlatform.begin(); iter != aPlatform.end(); ++iter)
						{
							if ((*iter)->getTranslation().x>-10.0f)
							{
								(*iter)->setTranslation(D3DXVECTOR2(-20.0f ,0.0f)); 
							}
						}
						//Gives all the remaining bats a velocity to the left.
						for(iterBat = aBat.begin(); iterBat != aBat.end(); ++iterBat)
						{
							if ((*iterBat)->getTranslation().x>-10.0f)
							{
								(*iterBat)->setTranslation(D3DXVECTOR2(-20.0f ,0.0f)); 
							}
						}
					}
					 //if the up key is pressed and the knight is not falling
					if (wParam == VK_UP && !falling)
					{
						//Moves the knight up.
						knightTrans.y -= 50.0f;
					}
				}
				return 0;
			}
		//when the level is active.
		if (Menu==false){
			//when a key is released
			case WM_KEYUP:
			{
				//if the left key is lifted
				if (wParam ==VK_LEFT)
				{
						//Stops the platforms.
						for(iter = aPlatform.begin(); iter != aPlatform.end(); ++iter)
						{
								(*iter)->setTranslation(D3DXVECTOR2(0.0f ,0.0f)); 
						}
						//Stops the bats.
						for(iterBat = aBat.begin(); iterBat != aBat.end(); ++iterBat)
						{
								(*iterBat)->setTranslation(D3DXVECTOR2(0.0f ,0.0f)); 
						}
				}
				//if the right key is lifted
					if (wParam == VK_RIGHT)
					{
						//Stops the platforms.
						for(iter = aPlatform.begin(); iter != aPlatform.end(); ++iter)
						{
							(*iter)->setTranslation(D3DXVECTOR2(0.0f ,0.0f)); 
						}
						//Stops the bats.
						for(iterBat = aBat.begin(); iterBat != aBat.end(); ++iterBat)
						{
							(*iterBat)->setTranslation(D3DXVECTOR2(0.0f ,0.0f)); 
						}
					}
					//if the up key is lifted makes falling true.
					if (wParam == VK_UP){falling=true;}
				return 0;
			}
		}
		//when the left mouse button is clicked.
		case WM_LBUTTONDOWN:
		{
			POINT mousexy;		//The point that the mouse clicked is recorded
			mousexy.x = LOWORD(lParam);
			mousexy.y = HIWORD(lParam);
			if (Menu==true&&mousexy.x>(clientBounds.right/2-100)&&mousexy.x<(clientBounds.right/2+100)&&mousexy.y>(clientBounds.bottom/2-50)&&mousexy.y<(clientBounds.bottom/2+50))
			{
				if(finish==false)
				{
					iter = aPlatform.begin();
					while (iter != aPlatform.end())
					{
						iter = aPlatform.erase(iter);
					}
					 backgroundName="Images\\SkyBackgroundSmall.png";
					 aSurface = d3dMgr->getD3DSurfaceFromFile(backgroundName);
					 Menu=false;
					 start=true;
					 return 0;
				}
				if(finish==true)
				{
					
					D3DXVECTOR3 PlatformPoser = D3DXVECTOR3(clientBounds.right/2-60,(float)clientBounds.bottom/2-25,0);
					aPlatform.push_back(new cPlatform(PlatformPoser,d3dMgr->getTheD3DDevice(),"Images\\ButtonInactive.png"));
					backgroundName="Images\\eulogyPlus.png";
					aSurface = d3dMgr->getD3DSurfaceFromFile(backgroundName);
					sprintf_s( gMenu, 50, "Play");
					sprintf_s( gScore, 100, "");
					finish=false;
					return 0;
				}
			}
			if (Menu==false)
			{
				iterBat = aBat.begin();
				while (iterBat != aBat.end())
				{
					if ( (*iterBat)->insideRect((*iterBat)->getBoundingRect(),mousexy))
					{
						iterBat = aBat.erase(iterBat);
						expPos = D3DXVECTOR3((float)mousexy.x-25,(float)mousexy.y-23, 0.0f);
						gExplode.push_back(new cExplosion(expPos,d3dMgr->getTheD3DDevice(),"Images\\explosionblood.png"));
						gExplodeSound.playSound(L"Sounds\\Splat.wav",false);
						scor+=10;
						iterBat = aBat.end();
					}
					else
					{

						++iterBat;
					}
				}
			}
			return 0;
		}

		case WM_CLOSE:
			{
			// Exit the Game
				PostQuitMessage(0);
				 return 0;
			}

		case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			}
	}
	// Always return the message to the default window
	// procedure for further processing
	return DefWindowProc(hWnd, message, wParam, lParam);
}

/*
==================================================================
* bool initWindow( HINSTANCE hInstance )
* initWindow registers the window class for the application, creates the window
==================================================================
*/
bool initWindow( HINSTANCE hInstance )
{
	WNDCLASSEX wcex;
	// Fill in the WNDCLASSEX structure. This describes how the window
	// will look to the system
	wcex.cbSize = sizeof(WNDCLASSEX); // the size of the structure
	wcex.style = CS_HREDRAW | CS_VREDRAW; // the class style
	wcex.lpfnWndProc = (WNDPROC)WndProc; // the window procedure callback
	wcex.cbClsExtra = 0; // extra bytes to allocate for this class
	wcex.cbWndExtra = 0; // extra bytes to allocate for this instance
	wcex.hInstance = hInstance; // handle to the application instance
	wcex.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_MyWindowIcon)); // icon to associate with the application
	wcex.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_GUNSIGHT));// the default cursor
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); // the background color
	wcex.lpszMenuName = NULL; // the resource name for the menu
	wcex.lpszClassName = "Platforms"; // the class name being created
	wcex.hIconSm = LoadIcon(hInstance,"KnightSprite.ico"); // the handle to the small icon

	RegisterClassEx(&wcex);
	// Create the window
	wndHandle = CreateWindow("Platforms",			// the window class to use
							 "Knight Of The Platforms",			// the title bar text
							WS_OVERLAPPEDWINDOW,	// the window style
							CW_USEDEFAULT, // the starting x coordinate
							CW_USEDEFAULT, // the starting y coordinate
							800, // the pixel width of the window
							600, // the pixel height of the window
							NULL, // the parent window; NULL for desktop
							NULL, // the menu for the application; NULL for none
							hInstance, // the handle to the application instance
							NULL); // no values passed to the window
	// Make sure that the window handle that is created is valid

	if (!wndHandle)
		return false;
	// Display the window on the screen
	ShowWindow(wndHandle, SW_SHOW);
	UpdateWindow(wndHandle);
	return true;
}

/*
==================================================================
// This is winmain, the main entry point for Windows applications
==================================================================
*/
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	// Initialize the window
	if ( !initWindow( hInstance ) )
		return false;
	// called after creating the window
	if ( !d3dMgr->initD3DManager(wndHandle) )
		return false;
	if ( !d3dxSRMgr->initD3DXSpriteMgr(d3dMgr->getTheD3DDevice()))
		return false;

	// Grab the frequency of the high def timer
	__int64 freq = 0;				// measured in counts per second;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	float sPC = 1.0f / (float)freq;			// number of seconds per count

	__int64 currentTime = 0;				// current time measured in counts per second;
	__int64 previousTime = 0;				// previous time measured in counts per second;

	float numFrames   = 0.0f;				// Used to hold the number of frames
	float timeElapsed = 0.0f;				// cumulative elapsed time

	GetClientRect(wndHandle,&clientBounds);
	float fpsRate = 1000.0f/25.0f;
	/* initialize random seed: */
	srand ( (unsigned int)time(NULL) );

	scor=0;
	
	cD3DXFont* KnightFont = new cD3DXFont(d3dMgr->getTheD3DDevice(),hInstance, "Bats&Dragons-Abaddon");
	RECT textPos;
	SetRect(&textPos, (clientBounds.right/2-30), (clientBounds.bottom/2), 800, 800);
	RECT buttonPos;
	SetRect(&buttonPos, (clientBounds.right/2-20), (clientBounds.bottom/2-20), 800, 800);
	D3DXVECTOR3 PlatformPos;
	D3DXVECTOR3 BatPos;
	/* generate random number of Platforms */
	int numPlatforms = (50);
	int numBat = (24);
	D3DXVECTOR3 knightPos;
	POINT pknightPos;
	cXAudio gStartSound;
	gStartSound.playSound(L"Sounds\\MoonlightSerenade.wav",false);
	
	PlatformPos = D3DXVECTOR3(clientBounds.right/2-60,(float)clientBounds.bottom/2-25,0);
	aPlatform.push_back(new cPlatform(PlatformPos,d3dMgr->getTheD3DDevice(),"Images\\ButtonInactive.png"));
	LPDIRECT3DSURFACE9 theBackbuffer = NULL;  // This will hold the back buffer
	SetRect(&buttonPos, (clientBounds.right/2-20), (clientBounds.bottom/2-20), 800, 800);

	MSG msg;
	ZeroMemory( &msg, sizeof( msg ) );

	// Create the background surface
	aSurface = d3dMgr->getD3DSurfaceFromFile(backgroundName);

	while( msg.message!=WM_QUIT )
	{
		// Check the message queue
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			if (Menu==true && finish==false)
			{
				SetRect(&buttonPos, (clientBounds.right/2-20), (clientBounds.bottom/2-20), 800, 800);
				sprintf_s( gScore, 100, "");
				sprintf_s( gMenu, 100, "Start");
			}
			if (Menu==false)
			{
				sprintf_s( gScore, 50, "");
				sprintf_s( gMenu, 50, "");
			}
			if (start==true)
			{
				start=false;
				scor=0;
				Health=30;				
				currentTime = 0;				// current time measured in counts per second;
				previousTime = 0;				// previous time measured in counts per second;
				numFrames   = 0.0f;				// Used to hold the number of frames
				timeElapsed = 0.0f;				// cumulative elapsed time
				PlatformTrans = D3DXVECTOR2(300,300);
				BatTrans = D3DXVECTOR2(300,300);
				knightTrans = D3DXVECTOR2(250,230);
				knightPos = D3DXVECTOR3((float)clientBounds.left/2-200,clientBounds.bottom-360,0);
				gKnight.push_back(new cKnight(knightPos,d3dMgr->getTheD3DDevice(),"Images\\KnightBodytwo.png",1,11));
				for(int loop = 0; loop < numPlatforms; loop++)
				{
					PlatformPos = D3DXVECTOR3(clientBounds.right/2-200+(loop*400),(float)clientBounds.bottom-100,0);
					aPlatform.push_back(new cPlatform(PlatformPos,d3dMgr->getTheD3DDevice(),"Images\\Platform.png"));
					aPlatform[loop]->setTranslation(D3DXVECTOR2(0.0f,0.0f));
				}
					for(int loop = 0; loop < numBat; loop++)
				{
					BatPos = D3DXVECTOR3(clientBounds.right/2+((loop+1)*800),(float)clientBounds.bottom-300,0);
					aBat.push_back(new cBat(BatPos,d3dMgr->getTheD3DDevice(),"Images\\BatFlipped.png"));
					aBat[loop]->setTranslation(D3DXVECTOR2(0.0f,0.0f));
				}
			}
			if (Menu==true && finish==true)  
			{
				SetRect(&buttonPos, (clientBounds.right/2-45), (clientBounds.bottom/2-20), 800, 800);
				sprintf_s( gScore, 100, "Your total score was: %d",scor);
				sprintf_s( gMenu, 50, "Re-Play");
			}
			iterBat = aBat.begin();
			while(iterBat != aBat.end())
			{
				// update Bat
				list<cKnight*>::iterator bite = gKnight.begin();
				if ((*iterBat)->collidedWith((*iterBat)->getBoundingRect(),(*bite)->getBoundingRect()))
				{
					Health-=10;
					expPos = D3DXVECTOR3((*iterBat)->getSpritePos2D().x+105.0f,(*iterBat)->getSpritePos2D().y+95.0f,0.0f);
					gExplode.push_back(new cExplosion(expPos,d3dMgr->getTheD3DDevice(),"Images\\explosionblood.png"));
					gExplodeSound.playSound(L"Sounds\\Splat.wav",false);
					iterBat = aBat.erase(iterBat);
					if (Health < 1)
					{
						iterBat = aBat.begin();
						while (iterBat != aBat.end())
						{
							iterBat = aBat.erase(iterBat);
						}
						iter = aPlatform.begin();
						while (iter != aPlatform.end())
						{
							iter = aPlatform.erase(iter);
						}
						list<cKnight*>::iterator bite = gKnight.begin();
						bite = gKnight.erase(bite);

						PlatformPos = D3DXVECTOR3(clientBounds.right/2-60,(float)clientBounds.bottom/2-25,0);
						aPlatform.push_back(new cPlatform(PlatformPos,d3dMgr->getTheD3DDevice(),"Images\\ButtonInactive.png"));
						backgroundName="Images\\Background.png";
						aSurface = d3dMgr->getD3DSurfaceFromFile(backgroundName);
						Menu=true;
						finish=true;
						iterBat = aBat.end();
					}
				}
				if (iterBat!=aBat.end()){++iterBat;}
			}
			for(iter = aPlatform.begin(); iter != aPlatform.end(); ++iter)
			{
				(*iter)->update();			// update Platform
			}
			QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
			dt = (currentTime - previousTime)*sPC;
			// Accumulate how much time has passed.
			timeElapsed = dt+timeElapsed;
			if(timeElapsed > fpsRate)
			{
				d3dMgr->beginRender();
				theBackbuffer = d3dMgr->getTheBackBuffer();
				
				d3dMgr->updateTheSurface(aSurface, theBackbuffer);
				d3dMgr->releaseTheBackbuffer(theBackbuffer);
				d3dxSRMgr->beginDraw();
				for(iter = aPlatform.begin(); iter != aPlatform.end(); ++iter)
				{
					d3dxSRMgr->setTheTransform((*iter)->getSpriteTransformMatrix());  
					d3dxSRMgr->drawSprite((*iter)->getTexture(),NULL,NULL,NULL,0xFFFFFFFF);
				}
				list<cKnight*>::iterator bite = gKnight.begin();
				int frog=0;
				float gravity=0;
				while(bite!=gKnight.end()){
					(*bite)->setSpritePos(knightPos);
					(*bite)->update(dt);
					d3dxSRMgr->setTheTransform((*bite)->getSpriteTransformMatrix());  
					d3dxSRMgr->drawSprite((*bite)->getTexture(),&((*bite)->getSourceRect()),NULL,NULL,0xFFFFFFFF);
					++bite;
					int pointOne=0;
					int pointTwo=0;
					float jump=100;
					iter = aPlatform.begin();
					float center = (*iter)->getSpritePos2D().x;
					pknightPos.x = knightTrans.x;
					pknightPos.y = knightTrans.y;
					for(iter = aPlatform.begin(); iter != aPlatform.end(); ++iter)
					{
						int king=center+(frog*400);
						pointOne=king-50;
						pointTwo=(king+232)-25;
						frog++;
						if (250>pointOne&&250<pointTwo)
						{
							gravity=315;
						}
					}
					if (knightPos.y<(clientBounds.top+jump)){falling=true;}
					if (knightPos.y<(clientBounds.bottom-gravity)){knightTrans.y += 30;}else{falling=false;}
					knightPos = D3DXVECTOR3(knightTrans.x,knightTrans.y,0);
					if(knightPos.y>clientBounds.bottom)
					{
						iterBat = aBat.begin();
						while (iterBat != aBat.end())
						{
							iterBat = aBat.erase(iterBat);
						}
						iter = aPlatform.begin();
						while (iter != aPlatform.end())
						{
							iter = aPlatform.erase(iter);
						}
						list<cKnight*>::iterator bite = gKnight.begin();
						bite = gKnight.erase(bite);

						PlatformPos = D3DXVECTOR3(clientBounds.right/2-60,(float)clientBounds.bottom/2-25,0);
						aPlatform.push_back(new cPlatform(PlatformPos,d3dMgr->getTheD3DDevice(),"Images\\ButtonInactive.png"));
						backgroundName="Images\\Background.png";
						aSurface = d3dMgr->getD3DSurfaceFromFile(backgroundName);
						Menu=true;
						finish=true;
					}
					bite=gKnight.end();
				}
				for(iterBat = aBat.begin(); iterBat != aBat.end(); ++iterBat)
				{
					D3DXVECTOR2 show =(*iterBat)->getTranslation();
					(*iterBat)->update(dt);
					knightPos = D3DXVECTOR3(knightTrans.x,knightTrans.y,0);
					d3dxSRMgr->setTheTransform((*iterBat)->getSpriteTransformMatrix());  
					d3dxSRMgr->drawSprite((*iterBat)->getTexture(),&((*iterBat)->getSourceRect()),NULL,NULL,0xFFFFFFFF);
				}
				list<cExplosion*>::iterator iter = gExplode.begin();
				while(iter != gExplode.end())
				{
					if((*iter)->isActive() == false)
					{
						iter = gExplode.erase(iter);
						}
					else
					{
						(*iter)->update(dt);
						d3dxSRMgr->setTheTransform((*iter)->getSpriteTransformMatrix());  
						d3dxSRMgr->drawSprite((*iter)->getTexture(),&((*iter)->getSourceRect()),NULL,NULL,0xFFFFFFFF);
						++iter;
					}
				}
				previousTime = currentTime;
				d3dxSRMgr->endDraw();
				KnightFont->printText(gScore,textPos);
				KnightFont->printText(gMenu,buttonPos);
			}
			d3dMgr->endRender();
		}
	}
	d3dxSRMgr->cleanUp();
	d3dMgr->clean();
	return (int) msg.wParam;
}