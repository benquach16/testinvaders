#include <cstdio>
#include <vector>
#include <algorithm>
#include "Engine.h"

using namespace std;

struct vec2
{
	int x;
	int y;
	vec2(int _x = 0, int _y = 0) : x(_x), y(_y) {}
	vec2& operator=(vec2 rhs) {
		x = rhs.x;
		y = rhs.y;
		return *this;
	}
};


//important to have arrays that are not fragmented
class Array {
	public:
	void push(int x, int y)
	{
		//we will segfault immediately if maxIdx is greater than cMaxArray
		//good thing we will make sure that it is impossible for the player to
		//create more than 256 objects in the time they have
		positions[maxIdx].x = x;
		positions[maxIdx].y = y;
		maxIdx++;
	}
	
	// this just swaps with the back end and decrements
	// so it is important to keep that in mind during the for loop
	void erase(uint32_t idx)
	{
		maxIdx--;
		swap(positions[idx], positions[maxIdx]);
	}
	
	vec2& operator[](uint32_t idx)
	{
		return positions[idx];
	}
	
	inline uint32_t getMaxIdx() const { return maxIdx; }
	private:
	static constexpr uint32_t cMaxArray = 256;
	vec2 positions[cMaxArray];
	uint32_t maxIdx;
};

namespace Aliens
{
	Array positions;
	int currentWidth = 0;
	void AddNew()
	{
		positions.push(currentWidth, Engine::SpriteSize);
		currentWidth += Engine::SpriteSize;
	}
};

namespace Rockets
{
	Array positions;
};

constexpr float cShipSpeed = 200.f;

void EngineMain()
{
	Engine engine;

	vec2 playerPosition;
	playerPosition.x = (Engine::CanvasWidth - Engine::SpriteSize) / 2;
	playerPosition.y = (Engine::CanvasHeight - Engine::SpriteSize); 
	
	float oldTimestamp = static_cast<float>(engine.getStopwatchElapsedSeconds());
	
	constexpr int maxWidthForSprite = (Engine::CanvasWidth - Engine::SpriteSize);
	
	float shootTimer = 0.0f;
	Aliens::AddNew();
	Aliens::AddNew();
	while (engine.startFrame())
	{
		float timestamp = static_cast<float>(engine.getStopwatchElapsedSeconds());
		
		float deltaTime = timestamp - oldTimestamp;
		oldTimestamp = timestamp;
		Engine::PlayerInput keys = engine.getPlayerInput();
		if (keys.left) {
			playerPosition.x -= static_cast<int>(cShipSpeed * deltaTime);
			playerPosition.x = max(0, playerPosition.x);
		}
		if (keys.right) {
			playerPosition.x += static_cast<int>(cShipSpeed * deltaTime);
			playerPosition.x = min(maxWidthForSprite, playerPosition.x);
		}
		
		if (keys.fire && shootTimer < timestamp) {
			Rockets::positions.push(playerPosition.x, playerPosition.y - (Engine::SpriteSize));
			shootTimer = timestamp + 1.f;
		}

		engine.drawSprite(
			Engine::Sprite::Player, 
			(playerPosition.x), (playerPosition.y));
		for(uint32_t i = 0; i < Aliens::positions.getMaxIdx(); ++i) {
			engine.drawSprite(
				Engine::Sprite::Enemy1, 
				(Aliens::positions[i].x), (Aliens::positions[i].y));
		}
		for(uint32_t i = 0; i < Rockets::positions.getMaxIdx(); ++i) {
			if(Rockets::positions[i].y < 0.f) {
				Rockets::positions.erase(i);
			}
			Rockets::positions[i].y -= static_cast<int>(cShipSpeed * deltaTime);
			engine.drawSprite(
				Engine::Sprite::Rocket, 
				(Rockets::positions[i].x), (Rockets::positions[i].y));

		}
		

		const char message[] = "Good Luck!";
		engine.drawText(
			message, 
			(Engine::CanvasWidth - (sizeof(message) - 1) * Engine::FontWidth) / 2, 
			(Engine::CanvasHeight - Engine::FontRowHeight) / 2); 
	}
}

