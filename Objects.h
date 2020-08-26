#pragma once

#include "Engine.h"
#include "vector.h"
#include "Render.h"

constexpr double cShipSpeed = 200.f;
constexpr double cEnemySpeed = 100.f;
constexpr double cShootInterval = 0.5f;
constexpr int cMaxWidthForSprite = (Engine::CanvasWidth - Engine::SpriteSize);

struct Entities
{
	Array<Renderable> array;
	// used to store frame independent increments to avoid multiple casts
	double accum = 0.f;
	double speed = cShipSpeed;
	void AccumDelta(double deltaTime) {
		if(accum > 1.f) {
			accum = 0;
		}
		accum += (deltaTime * speed);
	}
	void Destroy(uint32_t idx) {
		array.erase(idx);
	}
	void Clear() {
		array.clear();
	}
};

struct Aliens : public Entities
{
	// store coordinates of next enemy to create
	int nextX;
	int nextY;
	
	// state to determine movement of enemies
	bool goRight = true;
	bool goDown = false;
	bool flipped = false;
	
	// state to change sprite
	bool altSprite = false;
	Aliens() {
		speed = cEnemySpeed;
		Reset();
	}
	void AddNew() {
		constexpr int cPad = 8;
		if(nextX >= cMaxWidthForSprite - (Engine::SpriteSize*cPad)) {
			nextX = 0;
			nextY += Engine::SpriteSize;
			altSprite = !altSprite;
		}
		Engine::Sprite sprite = Engine::Sprite::Enemy1;
		if(altSprite) {
			sprite = Engine::Sprite::Enemy2;
		} 
		
		array.push(Renderable(vec2(nextX, nextY), sprite));
		nextX += Engine::SpriteSize;
	}
	
	void Reset() {
		nextX = 0;
		nextY = Engine::SpriteSize;
	}
	
	void CreateArmy() {
		Reset();
		for(int i = 0; i < 32; ++i) {
			Aliens::AddNew();
		}
	}
} aliens;


struct Rockets : public Entities
{
	void AddNew(vec2 position) {
		array.push(Renderable(position, Engine::Sprite::Rocket));
	}
} rockets;

struct Bombs : public Entities
{
	void AddNew(vec2 position) {
		array.push(Renderable(position, Engine::Sprite::Bomb));
	}
} bombs;

struct Player
{
	double accum = 0.f;
	Renderable render;
	void AccumDelta(double deltaTime) {
		if(accum > 1.f) {
			accum = 0;
		}
		accum += (deltaTime * cShipSpeed);
	}
	int health = 3;
	int score = 0;
	void Reset()
	{
		health = 3;
		score = 0;
		render.position.x = (Engine::CanvasWidth - Engine::SpriteSize) / 2;
		render.position.y = (Engine::CanvasHeight - Engine::SpriteSize); 
	}
	Player() {
		render.sprite = Engine::Sprite::Player;
		Reset();	
	}
} player;


