#include <cstdio>
#include <vector>
#include <algorithm>
#include <stdlib.h>
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

char text1[16]="Current Score:";
char text2[16]="Current Lives:";
char score[128];
char lives[128];
void renderUI(Engine& engine)
{
	sprintf_s(score, 128, "%s%d", text1,Player::score);
	engine.drawText(
		score, 
		0, 
		0);
	sprintf_s(lives, 128, "%s%d", text2,Player::health);
	engine.drawText(
		lives, 
		(Engine::CanvasWidth - (strlen(lives)) * Engine::FontWidth), 
		0);
}

void render(Engine& engine)
{
	//static_assert(static_cast<int>(Engine::Sprite::Player) == 0);
	for(int i = static_cast<int>(Engine::Sprite::Player); i < static_cast<int>(Engine::Sprite::Count); ++i) {
		while(RenderQueue::positions[i].getMaxIdx() > 0) {
			vec2 data = RenderQueue::positions[i][0];
			engine.drawSprite(static_cast<Engine::Sprite>(i), data.x, data.y);
			RenderQueue::positions[i].erase(0);
		}
	}
	renderUI(engine);
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

void update(int intAccum, double &bombTimer, double timestamp)
{
	for(uint32_t i = 0; i < Rockets::rocketArray.getMaxIdx(); ++i) {
		vec2 position = Rockets::rocketArray[i].position;
		if (position.y < 0.f) {
			Rockets::Destroy(i);
			position = Rockets::rocketArray[i].position;
		}
		position.y -= intAccum;
		Rockets::rocketArray[i].position=position;
		RenderQueue::Add(Rockets::rocketArray[i].sprite, position);
	}
	int minAlienX = Engine::CanvasWidth;
	int maxAlienX = 0;
	int multiplier = static_cast<int>(Aliens::goRight) * 2 - 1;
	for(uint32_t i = 0; i < Aliens::alienArray.getMaxIdx(); ++i) {
		vec2 position = Aliens::alienArray[i].position;
		for(uint32_t j = 0; j < Rockets::rocketArray.getMaxIdx(); ++j) {
			vec2 rocketPosition = Rockets::rocketArray[j].position;
			if(collision(rocketPosition, position)) {
				Rockets::Destroy(j);
				Aliens::Destroy(i);
				position = Aliens::alienArray[i].position;
				Player::score++;
			}
		}
		position.x += multiplier * static_cast<int>(Aliens::accum);
		Aliens::alienArray[i].position=position;
		maxAlienX = max(position.x, maxAlienX);
		minAlienX = min(position.x, minAlienX);
		
		if(bombTimer < timestamp && (rand()%20 < 2)) {
			Bombs::AddNew(position);
			bombTimer=timestamp + 3.0f;
		}
		
		RenderQueue::Add(Aliens::alienArray[i].sprite, position);

	}
	
	if(maxAlienX >= cMaxWidthForSprite) {
		Aliens::goRight = false;
		
	}
	if(minAlienX <= 0) {
		Aliens::goRight = true;
	}
	
	for(uint32_t i = 0; i < Bombs::bombArray.getMaxIdx(); ++i) {
		vec2 position = Bombs::bombArray[i].position;
		position.y += intAccum;
		Bombs::bombArray[i].position = position;
		
		RenderQueue::Add(Bombs::bombArray[i].sprite, position);
	}
}

void EngineMain()
{
	Engine engine;

	vec2 playerPosition;
	playerPosition.x = (Engine::CanvasWidth - Engine::SpriteSize) / 2;
	playerPosition.y = (Engine::CanvasHeight - Engine::SpriteSize); 
	
	double oldTimestamp = engine.getStopwatchElapsedSeconds();
	
	double shootTimer = 0.0f;
	double bombTimer = 0.0f;
	double accum = 0.0f;

	Aliens::CreateArmy();
	while (engine.startFrame())
	{
		double timestamp = engine.getStopwatchElapsedSeconds();
		
		double deltaTime = timestamp - oldTimestamp;
		accum += (cShipSpeed * deltaTime);
		int intAccum = static_cast<int>(accum);
		oldTimestamp = timestamp;
		Engine::PlayerInput keys = engine.getPlayerInput();
		Aliens::AccumDelta(deltaTime);
		control(keys, playerPosition, intAccum, shootTimer, timestamp);
		RenderQueue::Add(Engine::Sprite::Player, playerPosition);
		render(engine);
		update(intAccum, bombTimer, timestamp);
		
		if(accum >= 1.f) {
			accum = 0;
		}		
	}
}

