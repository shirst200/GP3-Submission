/*
=================
cPlatform.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cPlatform.h"
/*
=================
- Data constructor initializes the cPlatform to the data passed to 
- the constructor from the paramater sPosition.
- Is always called, thus ensures all sprite objects are in a consistent state.
=================
*/
cPlatform::cPlatform(): cSprite() 			// Default constructor
{
	/* initialize random seed: */
	srand ( (unsigned int)time(NULL) );
	/* generate secret number: */
	mScore = (rand() % 10 + 1)*10;
	mDirection = static_cast<eDirection>(rand() % 2);
}
cPlatform::cPlatform(D3DXVECTOR3 sPosition, LPDIRECT3DDEVICE9 pd3dDevice, LPCSTR theFilename): cSprite(sPosition,pd3dDevice,theFilename)  // Constructor
{
	/* initialize random seed: */
	srand ( (unsigned int)time(NULL) );
	/* generate secret number: */
	mScore = (rand() % 10 + 1)*10;
	mDirection = static_cast<eDirection>(rand() % 2);
}
/*
=================
- update the position etc of the sprite.
=================
*/
void cPlatform::update() 					// update the Platforms position based on the translation
{
	D3DXVECTOR2 tempV2;
	D3DXVECTOR3 tempV3;
	tempV2 = cPlatform::getSpritePos2D() + cPlatform::getTranslation();
	tempV3 = D3DXVECTOR3(tempV2.x,tempV2.y,0.0f);
	cPlatform::setSpritePos(tempV3);
	cSprite::update();
}
/*
=================
- Returns the current direction of the sprite.
=================
*/
eDirection cPlatform::getDirection()       // Rturns the current direction of the sprite
{
	return cPlatform::mDirection;
}
/*
=================
- Sets the current direction for the sprite.
=================
*/
void cPlatform::setDirection(eDirection theDirection)  // Sets the current direction for the sprite.
{
	cPlatform::mDirection = theDirection;
}
