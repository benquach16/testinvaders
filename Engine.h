#pragma once

//////////////////////////////////////////////////////////////////////////////
// DO NOT ADD YOUR CODE IN THIS FILE!
//////////////////////////////////////////////////////////////////////////////

struct Engine
{
	// Canvas size in pixels
	static const int CanvasWidth   = 640;
	static const int CanvasHeight  = 480;

	// Sprite size in pixels (both width and height)
	static const int SpriteSize    = 32;

	// Distance between two characters in pixels when printing text
	static const int FontWidth     = 8;
	
	// Recommended distance between two rows of text
	static const int FontRowHeight = 18;
	
	struct PlayerInput
	{
		bool fire;  // space
		bool left;  // left arrow
		bool right; // right arrow
	};

	enum class Sprite : int
	{
		Player,
		Bomb,
		Rocket,
		Enemy1,
		Enemy2,

		Count
	};

	// Elapsed time since this instance was created
	double getStopwatchElapsedSeconds();

	// Clear the canvas and sample player input.
	// Quit the application when false is returned.
	bool startFrame();

	// Draw one of the predefined sprites with top left corner at x,y
	void drawSprite(Sprite sprite, int x, int y);
	
	// Draw a text message with top left corner of first character at x,y.
	// Spacing is determined by FontWidth and FontRowHeight
	void drawText(const char* message, int x, int y);

	// Return the player input which was sampled in startFrame
	PlayerInput getPlayerInput();
	
	Engine();
	~Engine();

};

