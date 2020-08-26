#pragma once

#include "Engine.h"
#include "vector.h"

constexpr double cShipSpeed = 200.f;
constexpr double cEnemySpeed = 100.f;
constexpr double cShootInterval = 0.5f;
constexpr int cMaxWidthForSprite = (Engine::CanvasWidth - Engine::SpriteSize);

namespace RenderQueue
{
	//batched similar sprites together
	Array<vec2> positions[static_cast<int>(Engine::Sprite::Count)];
	Array<vec2>& get(Engine::Sprite idx) {
		return positions[static_cast<int>(idx)];
	}	
	void Add(Engine::Sprite sprite, vec2 position) {
		positions[static_cast<int>(sprite)].push(position);
	}
};

struct Renderable
{
	vec2 position;
	Engine::Sprite sprite;
	Renderable(vec2 _position = vec2(), Engine::Sprite _sprite = Engine::Sprite::Player) : position(_position), sprite(_sprite) {}
};

struct Entities
{
	Array<Renderable> array;
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
	int nextX;
	int nextY;
	bool goRight = true;
	bool goDown = false;
	bool flipped = false;
	bool altSprite = false;
	Aliens() {
		speed = cEnemySpeed;
		Reset();
	}
	void AddNew()
	{
		if(nextX >= cMaxWidthForSprite - (Engine::SpriteSize*8)) {
			nextX = 0;
			nextY += Engine::SpriteSize;
			altSprite = !altSprite;
		}
		Engine::Sprite sprite = Engine::Sprite::Enemy1;
		if(altSprite) {
			sprite = Engine::Sprite::Enemy2;
		} 
		
		//in place
		array.push(Renderable(vec2(nextX, nextY), sprite));
		nextX += Engine::SpriteSize;
	}
	
	void Reset()
	{
		nextX = 0;
		nextY = Engine::SpriteSize;
	}
	
	void CreateArmy()
	{
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
	vec2 position;
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
		position.x = (Engine::CanvasWidth - Engine::SpriteSize) / 2;
		position.y = (Engine::CanvasHeight - Engine::SpriteSize); 
	}
} player;


