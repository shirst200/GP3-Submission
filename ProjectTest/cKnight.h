/*
=================
cExplosion.h
- Header file for class definition - SPECIFICATION
- Header file for the Sprite class
=================
*/
#ifndef _CKNIGHT_H
#define _CKNIGHT_H
#include "cSprite.h"

class cKnight : public cSprite
{
private:
	bool mActive;
	int mNoFrames;
	int mCurrentFrame;
	RECT mSourceRect;
	float mRate;
	int noAlong;
	int noUp;
	float sizeUp;
	float sizeAcross;

public:
	cKnight();
	cKnight(D3DXVECTOR3 sPosition, LPDIRECT3DDEVICE9 pd3dDevice, LPCSTR theFilename,int height,int width); // Constructor
	void update(float dt);					// update the balloons position based on the translation
	RECT getSourceRect();					// Return the current segment of the sprite sheet to draw.
	int getCurrentFrame();					// Return the current frame.
	void setActive(bool sActive);			// Set the sprite to active.
	bool isActive();						// Determine if the sprite is active.
};
#endif