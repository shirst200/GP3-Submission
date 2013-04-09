/*
=================
cBat.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cBat.h"
/*
=================
- Data constructor initializes the cBalloon to the data passed to 
- the constructor from the paramater sPosition.
- Is always called, thus ensures all sprite objects are in a consistent state.
=================
*/
cBat::cBat(): cSprite() 			// Default constructor
{
	noAlong=15;
	noUp=1;
	mActive = true;
	mNoFrames = 3;
	mCurrentFrame = 0;
	mRate = 1000.0f/25.0f;
	SetRect(&mSourceRect,0,0,cBat::getSTWidth(),cBat::getSTHeight());
}
cBat::cBat(D3DXVECTOR3 sPosition, LPDIRECT3DDEVICE9 pd3dDevice, LPCSTR theFilename): cSprite(sPosition,pd3dDevice,theFilename)  // Constructor
{
	noAlong=15;
	noUp=1;
	mActive = true;
	mNoFrames = 3;
	mCurrentFrame = 0;
	mRate = 1000.0f/25.0f;
	SetRect(&mSourceRect,0,0,cBat::getSTWidth(),cBat::getSTHeight());
}
/*
=================
- update the position etc of the sprite.
=================
*/
void cBat::update(float dt) 					// update the Balloons position based on the translation
{
	cSprite::update();
	D3DXVECTOR2 tempV2;
	D3DXVECTOR3 tempV3;
	tempV2 = cBat::getSpritePos2D() + cBat::getTranslation();
	tempV3 = D3DXVECTOR3(tempV2.x,tempV2.y,0.0f);
	cBat::setSpritePos(tempV3);
	if(dt < mRate)
	{
		SetRect(&mSourceRect,mCurrentFrame*191,0,(mCurrentFrame*191)+191,211);
		if(mCurrentFrame > mNoFrames)
		{
			mCurrentFrame = 0;
			//mActive = false;
		}
		else{mCurrentFrame++;}
	}
	
}
/*
=================
- Return the current segment of the sprite sheet to draw.
=================
*/
RECT cBat::getSourceRect() 					// Return the current segment of the sprite sheet to draw.
{
	return mSourceRect;
}
/*
=================
- Return the current frame.
=================
*/
int cBat::getCurrentFrame() 					// Return the current frame.
{
	return mCurrentFrame;
}
/*
=================
- Set the sprite to active.
=================
*/
void cBat::setActive(bool sActive) 			// Set the sprite to active.
{
	mActive = sActive;
}
/*
=================
- Determine if the sprite is active.
=================
*/
bool cBat::isActive() 						// Determine if the sprite is active.
{
	return mActive;
}
