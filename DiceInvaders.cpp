#include <cstdio>
#include <vector>
#include <algorithm>
#include "Engine.h"
#include "Array.h"

using namespace std;

constexpr double cShipSpeed = 200.f;
constexpr double cEnemySpeed = 100.f;
constexpr int cMaxWidthForSprite = (Engine::CanvasWidth - Engine::SpriteSize);

enum class eGameState : uint8_t {
	InGame,
	Lost
};

struct RenderData
{
	vec2 position;
	Engine::Sprite sprite;
};

namespace Rockets
{
	Array positions;
};

namespace Bombs
{
	Array positions;
};

namespace Player
{
	vec2 position;
	int health = 3;
};

namespace Aliens
{
	Array positions;
	int nextX = 0;
	int nextY = 0;
	bool goRight = true;
	void AddNew()
	{
		if(nextX >= cMaxWidthForSprite - Engine::SpriteSize) {
			nextX = 0;
			nextY += Engine::SpriteSize;
		}
		positions.push(nextX, nextY);
		Bombs::positions.push(nextX, nextY);
		nextX += Engine::SpriteSize;
		
	}
	
	void CreateArmy()
	{
		for(int i = 0; i < 32; ++i) {
			Aliens::AddNew();
		}
	}
};

inline bool collision(vec2 a, vec2 b)
{
	bool xAxisCollision = (a.x < (b.x + Engine::SpriteSize)) && (b.x < (a.x + Engine::SpriteSize));
	bool yAxisCollision = (a.y < (b.y + Engine::SpriteSize)) && (b.y < (a.y + Engine::SpriteSize));
	return xAxisCollision && yAxisCollision;
}


void EngineMain()
{
	Engine engine;

	vec2 playerPosition;
	playerPosition.x = (Engine::CanvasWidth - Engine::SpriteSize) / 2;
	playerPosition.y = (Engine::CanvasHeight - Engine::SpriteSize); 
	
	double oldTimestamp = engine.getStopwatchElapsedSeconds();
	
	double shootTimer = 0.0f;
	double accum = 0.0f;

	Aliens::CreateArmy();
	uint32_t score = 0;
	
	while (engine.startFrame())
	{
		double timestamp = engine.getStopwatchElapsedSeconds();
		
		double deltaTime = timestamp - oldTimestamp;
		accum += (cShipSpeed * deltaTime);
		int intAccum = static_cast<int>(accum);
		oldTimestamp = timestamp;
		Engine::PlayerInput keys = engine.getPlayerInput();
		if (keys.left) {
			playerPosition.x -= intAccum;
			playerPosition.x = max(0, playerPosition.x);
		}
		if (keys.right) {
			playerPosition.x += intAccum;
			playerPosition.x = min(cMaxWidthForSprite, playerPosition.x);
		}
		
		if (keys.fire && shootTimer < timestamp) {
			Rockets::positions.push(playerPosition.x, playerPosition.y - (Engine::SpriteSize));
			shootTimer = timestamp + 0.5f;
		}
		
		//group up all the draw calls
		engine.drawSprite(
			Engine::Sprite::Player, 
			(playerPosition.x), (playerPosition.y));
			
		int maxAlienX = 0;
		int minAlienX = Engine::CanvasWidth;
		for(uint32_t i = 0; i < Aliens::positions.getMaxIdx(); ++i) {
			// double for loop is not too bad since both of these should fit in a cache line
			for(uint32_t j = 0; j < Rockets::positions.getMaxIdx(); ++j) {
				if(collision(Aliens::positions[i], Rockets::positions[j])) {
					Aliens::positions.erase(i);
					Rockets::positions.erase(j);
					score++;
					break;
				}
			}
			
			//convert 0,1 to -1, 1
			int multiplier = static_cast<int>(Aliens::goRight) * 2 - 1;
			Aliens::positions[i].x += intAccum * multiplier;
			
			maxAlienX = max(Aliens::positions[i].x, maxAlienX);
			minAlienX = min(Aliens::positions[i].x, minAlienX);
			engine.drawSprite(
				Engine::Sprite::Enemy1, 
				(Aliens::positions[i].x), (Aliens::positions[i].y));
		}
		for(uint32_t i = 0; i < Rockets::positions.getMaxIdx(); ++i) {
			if(Rockets::positions[i].y < 0.f) {
				Rockets::positions.erase(i);
			}
			Rockets::positions[i].y -= intAccum;
			engine.drawSprite(
				Engine::Sprite::Rocket, 
				(Rockets::positions[i].x), (Rockets::positions[i].y));
		}
		for(uint32_t i = 0; i < Bombs::positions.getMaxIdx(); ++i) {
			if(Bombs::positions[i].y > Engine::CanvasHeight) {
				Bombs::positions.erase(i);
			}
			Bombs::positions[i].y += intAccum;
			engine.drawSprite(
				Engine::Sprite::Bomb, 
				(Bombs::positions[i].x), (Bombs::positions[i].y));			
		}
		if(maxAlienX >= cMaxWidthForSprite) {
			Aliens::goRight = false;
		}
		if(minAlienX <= 0) {
			Aliens::goRight = true;
		}
		

		const char message[] = "Current Score:";
		engine.drawText(
			message, 
			0, 
			0); 
			
		if(accum > 1.f) {
			accum = 0;
		}		
	}
}

