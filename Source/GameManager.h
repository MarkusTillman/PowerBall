#pragma once

/* 
Gets control of the main thread from MainMenu after play is initiated. 
Takes care of playing a single game and upon the end of the game returns the thread back to MainMenu.
*/

#include "stdafx.h"
#include "Platform.h"
#include "Ball.h"
class GameManager
{
private:
	int			mNumPlayers;
	Platform*	mPlatform;
	Ball**		mBalls; //size = mNumPlayers (intialized in Initialize();)
public:
	//constructors and destructors
				GameManager();
	virtual		~GameManager();

	/*! Starts the game with the assigned amount of players. */
	bool		Play(const int numPlayers);

private:

	/*! Initializes the game. (First function-call in Play) */
	void		Initialize();

};