#include <cstdio>
#include <vector>
#include <algorithm>
#include "Engine.h"
#include "Array.h"
#include "Objects.h"
#include "vector.h"

using namespace std;

enum class eGameState : uint8_t {
	InGame,
	Lost
};


inline bool collision(vec2 a, vec2 b)
{
	bool xAxisCollision = (a.x < (b.x + Engine::SpriteSize)) && (b.x < (a.x + Engine::SpriteSize));
	bool yAxisCollision = (a.y < (b.y + Engine::SpriteSize)) && (b.y < (a.y + Engine::SpriteSize));
	return xAxisCollision && yAxisCollision;
}

void render(Engine& engine)
{
	//static_assert(static_cast<int>(Engine::Sprite::Player) == 0);
	for(int i = static_cast<int>(Engine::Sprite::Player); i < static_cast<int>(Engine::Sprite::Count); ++i) {
		for(uint32_t j = 0 ; j < Positions::positions[i].getMaxIdx(); ++j) {
			vec2 data = Positions::positions[i][j];
			engine.drawSprite(static_cast<Engine::Sprite>(i), data.x, data.y);
		}
	}
}

void control(Engine::PlayerInput keys, vec2 &position, int intAccum, double &shootTimer, double timestamp)
{
	if (keys.left) {
		position.x -= intAccum;
		position.x = max(0, position.x);
	}
	if (keys.right) {
		position.x += intAccum;
		position.x = min(cMaxWidthForSprite, position.x);
	}
	if (keys.fire && shootTimer < timestamp) {
		Rockets::AddNew(vec2(position.x, position.y - (Engine::SpriteSize)));
		shootTimer = timestamp + 0.5f;
	}
}

void update(int intAccum)
{
	for(uint32_t i = 0; i < Rockets::rocketArray.getMaxIdx(); ++i) {
		vec2 position = Rockets::rocketArray[i].getPosition();
		position.y -= intAccum;
		Rockets::rocketArray[i].updatePosition(position);
	}
	int minAlienX = Engine::CanvasWidth;
	int maxAlienX = 0;
	int multiplier = static_cast<int>(Aliens::goRight) * 2 - 1;
	for(uint32_t i = 0; i < Aliens::alienArray.getMaxIdx(); ++i) {
		vec2 position = Aliens::alienArray[i].getPosition();
		position.x += multiplier * intAccum;
		Aliens::alienArray[i].updatePosition(position);
		maxAlienX = max(position.x, maxAlienX);
		minAlienX = min(position.x, minAlienX);
	}
	if(maxAlienX >= cMaxWidthForSprite) {
		Aliens::goRight = false;
	}
	if(minAlienX <= 0) {
		Aliens::goRight = true;
	}
	
}


void EngineMain()
{
	Engine engine;

	vec2 start;
	start.x = (Engine::CanvasWidth - Engine::SpriteSize) / 2;
	start.y = (Engine::CanvasHeight - Engine::SpriteSize); 
	
	double oldTimestamp = engine.getStopwatchElapsedSeconds();
	
	double shootTimer = 0.0f;
	double accum = 0.0f;

	Aliens::CreateArmy();
	uint32_t id = Positions::get(Engine::Sprite::Player).push(start);
	vec2 &playerPosition = Positions::get(Engine::Sprite::Player)[id];
	while (engine.startFrame())
	{
		double timestamp = engine.getStopwatchElapsedSeconds();
		
		double deltaTime = timestamp - oldTimestamp;
		accum += (cShipSpeed * deltaTime);
		int intAccum = static_cast<int>(accum);
		oldTimestamp = timestamp;
		Engine::PlayerInput keys = engine.getPlayerInput();

		control(keys, playerPosition, intAccum, shootTimer, timestamp);
		render(engine);
		update(intAccum);

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

