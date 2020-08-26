#include <cstdio>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include "Engine.h"
#include "Array.h"
#include "Objects.h"
#include "Render.h"
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

inline void Control(Engine::PlayerInput keys, vec2 &position, double &shootTimer, double timestamp)
{
	if (keys.left) {
		position.x -= static_cast<int>(player.accum);
		position.x = max(0, position.x);
	}
	if (keys.right) {
		position.x += static_cast<int>(player.accum);
		position.x = min(cMaxWidthForSprite, position.x);
	}
	if (keys.fire && shootTimer < timestamp) {
		rockets.AddNew(vec2(position.x, position.y - (Engine::SpriteSize)));
		shootTimer = timestamp + cShootInterval;
	}
}

void AccumDelta(double deltaTime)
{
	rockets.AccumDelta(deltaTime);
	aliens.AccumDelta(deltaTime);
	bombs.AccumDelta(deltaTime);
	player.AccumDelta(deltaTime);
}

void UpdateProjectiles()
{
	int delta = static_cast<int>(rockets.accum);
	for(uint32_t i = 0; i <rockets.array.getMaxIdx(); ++i) {
		vec2 position = rockets.array[i].position;
		if (position.y < 0.f) {
			rockets.Destroy(i);
			position = rockets.array[i].position;
		}
		position.y -= delta;
		rockets.array[i].position=position;
		RenderQueue::Add(rockets.array[i].sprite, position);
	}
	delta = static_cast<int>(bombs.accum);
	for(uint32_t i = 0; i < bombs.array.getMaxIdx(); ++i) {
		vec2 position = bombs.array[i].position;
		if (position.y > Engine::CanvasHeight) {
			bombs.Destroy(i);
			position = bombs.array[i].position;
		}
		if(collision(player.position, position)) {
			bombs.Destroy(i);
			player.health--;
			position = bombs.array[i].position;
		}
		position.y += delta;
		bombs.array[i].position = position;
		
		RenderQueue::Add(bombs.array[i].sprite, position);
	}
}

void Update(double &bombTimer, double timestamp)
{
	int minAlienX = Engine::CanvasWidth;
	int maxAlienX = 0;
	int multiplier = static_cast<int>(aliens.goRight) * 2 - 1;
	int delta = static_cast<int>(aliens.accum);
	for(uint32_t i = 0; i < aliens.array.getMaxIdx(); ++i) {
		vec2 position = aliens.array[i].position;
		for(uint32_t j = 0; j < rockets.array.getMaxIdx(); ++j) {
			vec2 rocketPosition = rockets.array[j].position;
			if(collision(rocketPosition, position)) {
				rockets.Destroy(j);
				aliens.Destroy(i);
				position = aliens.array[i].position;
				player.score++;
			}
		}
		position.x += multiplier * delta;
		position.y += Engine::SpriteSize * static_cast<int>(aliens.goDown);
		aliens.array[i].position=position;
		maxAlienX = max(position.x, maxAlienX);
		minAlienX = min(position.x, minAlienX);
		
		if(bombTimer < timestamp && (rand()%20 < 2)) {
			bombs.AddNew(position);
			bombTimer=timestamp + cShootInterval;
		}
		
		RenderQueue::Add(aliens.array[i].sprite, position);
	}
	if(aliens.array.getMaxIdx() == 0) {
		aliens.CreateArmy();
	}
	aliens.goDown = false;
	if(maxAlienX >= cMaxWidthForSprite) {
		aliens.goRight = false;
		if(aliens.flipped == false) {
			aliens.goDown = true;
			aliens.flipped=true;
		}
		
	}
	if(minAlienX <= 0) {
		aliens.goRight = true;
		aliens.flipped = false;
	}
	UpdateProjectiles();
}

inline void InitializeGame()
{
	aliens.array.clear();
	rockets.array.clear();
	bombs.array.clear();
	player.Reset();
	aliens.CreateArmy();
}

void EngineMain()
{
	Engine engine;
	
	double oldTimestamp = engine.getStopwatchElapsedSeconds();
	
	double shootTimer = 0.0f;
	double bombTimer = 0.0f;

	InitializeGame();
	eGameState currentState = eGameState::InGame;
	while (engine.startFrame())
	{
		double timestamp = engine.getStopwatchElapsedSeconds();
		double deltaTime = timestamp - oldTimestamp;
		oldTimestamp = timestamp;
		Engine::PlayerInput keys = engine.getPlayerInput();		
		RenderQueue::Render(engine);
		RenderQueue::RenderUI(engine, player.score, player.health);
		
		switch(currentState) {
			case eGameState::InGame:
			AccumDelta(deltaTime);
			Control(keys, player.position, shootTimer, timestamp);
			RenderQueue::Add(Engine::Sprite::Player, player.position);
			Update(bombTimer, timestamp);
			if(player.health <= 0) {
				currentState = eGameState::Lost;
			}

			break;
			case eGameState::Lost:
			RenderQueue::RenderDeath(engine, player.score);
			if(keys.fire) {
				InitializeGame();
				currentState = eGameState::InGame;
			}
			break;
		}
	}
}

